/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cartographer_ros/node.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "Eigen/Core"
#include "absl/memory/memory.h"
#include "cartographer/common/configuration_file_resolver.h"
#include "cartographer/common/lua_parameter_dictionary.h"
#include "cartographer/common/port.h"
#include "cartographer/common/time.h"
#include "cartographer/mapping/pose_graph_interface.h"
#include "cartographer/mapping/proto/submap_visualization.pb.h"
#include "cartographer/metrics/register.h"
#include "cartographer/sensor/point_cloud.h"
#include "cartographer/transform/rigid_transform.h"
#include "cartographer/transform/transform.h"
#include "cartographer_ros/metrics/family_factory.h"
#include "cartographer_ros/msg_conversion.h"
#include "cartographer_ros/sensor_bridge.h"
#include "cartographer_ros/tf_bridge.h"
#include "cartographer_ros/time_conversion.h"
#include "cartographer_ros_msgs/msg/status_code.hpp"
#include "cartographer_ros_msgs/msg/status_response.hpp"
#include "builtin_interfaces/msg/time.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
#include "glog/logging.h"
#include "nav_msgs/msg/odometry.hpp"
//#include "ros/serialization.h"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "tf2_eigen/tf2_eigen.hpp"
#include "visualization_msgs/msg/marker_array.hpp"

//以下追加
#include <fstream>
#include <iomanip>
#include <cstdio>
#include "absl/synchronization/mutex.h"
#include "cartographer/io/proto_stream.h"
#include <hiredis/hiredis.h>
#include "std_srvs/srv/trigger.hpp"
//tf pose保存のため
#include <sstream>
#include <regex>
#include "tf2/exceptions.h"

namespace cartographer_ros {

namespace carto = ::cartographer;

using carto::transform::Rigid3d;
using TrajectoryState =
    ::cartographer::mapping::PoseGraphInterface::TrajectoryState;

namespace {
// Subscribes to the 'topic' for 'trajectory_id' using the 'node_handle' and
// calls 'handler' on the 'node' to handle messages. Returns the subscriber.
template <typename MessageType>
::rclcpp::SubscriptionBase::SharedPtr SubscribeWithHandler(
    void (Node::*handler)(int, const std::string&,
                          const typename MessageType::ConstSharedPtr&),
    const int trajectory_id, const std::string& topic,
    ::rclcpp::Node::SharedPtr node_handle, Node* const node) {
  return node_handle->create_subscription<MessageType>(
      topic, rclcpp::SensorDataQoS(),
      [node, handler, trajectory_id, topic](const typename MessageType::ConstSharedPtr msg) {
            (node->*handler)(trajectory_id, topic, msg);
          });
}

std::string TrajectoryStateToString(const TrajectoryState trajectory_state) {
  switch (trajectory_state) {
    case TrajectoryState::ACTIVE:
      return "ACTIVE";
    case TrajectoryState::FINISHED:
      return "FINISHED";
    case TrajectoryState::FROZEN:
      return "FROZEN";
    case TrajectoryState::DELETED:
      return "DELETED";
  }
  return "";
}

constexpr char kRedisStateKey[] = "robot1_state";
constexpr char kRedisPoseKey[] = "robot1_pose"; //これいらないかも
constexpr char kRedisMapPoseKey[] = "robot1_map_pose";
constexpr char kRedisOdomPoseKey[] = "robot1_odom_pose";

int FindReferenceTrajectoryId(
    const std::map<int, TrajectoryState>& trajectory_states) {
  // export 時は ACTIVE trajectory を優先
  for (const auto& entry : trajectory_states) {
    if (entry.second == TrajectoryState::ACTIVE) {
      return entry.first;
    }
  }
  // 念のため fallback
  for (const auto& entry : trajectory_states) {
    if (entry.second == TrajectoryState::FROZEN ||
        entry.second == TrajectoryState::FINISHED) {
      return entry.first;
    }
  }
  return -1;
}

using HandoverSteadyClock = std::chrono::steady_clock;

double HandoverMilliseconds(const HandoverSteadyClock::duration duration) {
  return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
             duration)
      .count();
}

// Measurement-only sensor-time markers. One cartographer_ros::Node is assumed
// per process, which matches the current slam_a/slam_b container design.
std::atomic<int64_t> g_first_processed_odom_stamp_ns{0};
std::atomic<int64_t> g_last_processed_odom_stamp_ns{0};
std::atomic<int64_t> g_first_processed_scan_stamp_ns{0};
std::atomic<int64_t> g_last_processed_scan_stamp_ns{0};

int64_t RosStampToNanoseconds(const builtin_interfaces::msg::Time& stamp) {
  return static_cast<int64_t>(stamp.sec) * 1000000000LL +
         static_cast<int64_t>(stamp.nanosec);
}

void RecordProcessedSensorStamp(
    const int64_t stamp_ns, std::atomic<int64_t>* first_stamp_ns,
    std::atomic<int64_t>* last_stamp_ns) {
  if (stamp_ns <= 0) {
    return;
  }

  int64_t expected = 0;
  first_stamp_ns->compare_exchange_strong(
      expected, stamp_ns, std::memory_order_relaxed);

  // Keep the greatest processed sensor timestamp. This remains robust even if
  // callbacks arrive slightly out of timestamp order.
  int64_t observed = last_stamp_ns->load(std::memory_order_relaxed);
  while (stamp_ns > observed &&
         !last_stamp_ns->compare_exchange_weak(
             observed, stamp_ns, std::memory_order_relaxed)) {
  }
}

}  // namespace

Node::Node(
    const NodeOptions& node_options,
    std::unique_ptr<cartographer::mapping::MapBuilderInterface> map_builder,
    std::shared_ptr<tf2_ros::Buffer> tf_buffer,
    rclcpp::Node::SharedPtr node,
    const bool collect_metrics)
    : node_options_(node_options)
{
  // Reset measurement markers whenever a fresh Cartographer Node is created.
  g_first_processed_odom_stamp_ns.store(0, std::memory_order_relaxed);
  g_last_processed_odom_stamp_ns.store(0, std::memory_order_relaxed);
  g_first_processed_scan_stamp_ns.store(0, std::memory_order_relaxed);
  g_last_processed_scan_stamp_ns.store(0, std::memory_order_relaxed);

  node_ = node;
  // 追加
  tf_buffer_ = tf_buffer;

  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(node_) ;
  map_builder_bridge_.reset(new cartographer_ros::MapBuilderBridge(node_options_, std::move(map_builder), tf_buffer.get()));

  absl::MutexLock lock(&mutex_);
  if (collect_metrics) {
    metrics_registry_ = absl::make_unique<metrics::FamilyFactory>();
    carto::metrics::RegisterAllMetrics(metrics_registry_.get());
  }

  submap_list_publisher_ =
      node_->create_publisher<::cartographer_ros_msgs::msg::SubmapList>(
          kSubmapListTopic, 10);
  trajectory_node_list_publisher_ =
      node_->create_publisher<::visualization_msgs::msg::MarkerArray>(
          kTrajectoryNodeListTopic, 10);
  landmark_poses_list_publisher_ =
      node_->create_publisher<::visualization_msgs::msg::MarkerArray>(
          kLandmarkPosesListTopic, 10);
  constraint_list_publisher_ =
      node_->create_publisher<::visualization_msgs::msg::MarkerArray>(
          kConstraintListTopic, 10);
  if (node_options_.publish_tracked_pose) {
    tracked_pose_publisher_ =
        node_->create_publisher<::geometry_msgs::msg::PoseStamped>(
            kTrackedPoseTopic, 10);
  }

  scan_matched_point_cloud_publisher_ =
      node_->create_publisher<sensor_msgs::msg::PointCloud2>(
        kScanMatchedPointCloudTopic, 10);

  submap_query_server_ = node_->create_service<cartographer_ros_msgs::srv::SubmapQuery>(
      kSubmapQueryServiceName,
      std::bind(
          &Node::handleSubmapQuery, this, std::placeholders::_1, std::placeholders::_2));
  trajectory_query_server = node_->create_service<cartographer_ros_msgs::srv::TrajectoryQuery>(
      kTrajectoryQueryServiceName,
      std::bind(
          &Node::handleTrajectoryQuery, this, std::placeholders::_1, std::placeholders::_2));
  start_trajectory_server_ = node_->create_service<cartographer_ros_msgs::srv::StartTrajectory>(
      kStartTrajectoryServiceName,
      std::bind(
          &Node::handleStartTrajectory, this, std::placeholders::_1, std::placeholders::_2));
  finish_trajectory_server_ = node_->create_service<cartographer_ros_msgs::srv::FinishTrajectory>(
      kFinishTrajectoryServiceName,
      std::bind(
          &Node::handleFinishTrajectory, this, std::placeholders::_1, std::placeholders::_2));
  write_state_server_ = node_->create_service<cartographer_ros_msgs::srv::WriteState>(
      kWriteStateServiceName,
      std::bind(
          &Node::handleWriteState, this, std::placeholders::_1, std::placeholders::_2));
  get_trajectory_states_server_ = node_->create_service<cartographer_ros_msgs::srv::GetTrajectoryStates>(
      kGetTrajectoryStatesServiceName,
      std::bind(
          &Node::handleGetTrajectoryStates, this, std::placeholders::_1, std::placeholders::_2));
  read_metrics_server_ = node_->create_service<cartographer_ros_msgs::srv::ReadMetrics>(
      kReadMetricsServiceName,
      std::bind(
          &Node::handleReadMetrics, this, std::placeholders::_1, std::placeholders::_2));


  submap_list_timer_ = node_->create_wall_timer(
    std::chrono::milliseconds(int(node_options_.submap_publish_period_sec * 1000)),
    [this]() {
      PublishSubmapList();
    });
  if (node_options_.pose_publish_period_sec > 0) {
    local_trajectory_data_timer_ = node_->create_wall_timer(
      std::chrono::milliseconds(int(node_options_.pose_publish_period_sec * 1000)),
      [this]() {
        PublishLocalTrajectoryData();
      });
  }
  trajectory_node_list_timer_ = node_->create_wall_timer(
    std::chrono::milliseconds(int(node_options_.trajectory_publish_period_sec * 1000)),
    [this]() {
      PublishTrajectoryNodeList();
    });
  landmark_pose_list_timer_ = node_->create_wall_timer(
    std::chrono::milliseconds(int(node_options_.trajectory_publish_period_sec * 1000)),
    [this]() {
      PublishLandmarkPosesList();
    });
  constrain_list_timer_ = node_->create_wall_timer(
    std::chrono::milliseconds(int(kConstraintPublishPeriodSec * 1000)),
    [this]() {
      PublishConstraintList();
    });

  // ＝＝＝ コンストラクタ内にこれを追加 ＝＝＝
  export_state_service_ = node_->create_service<std_srvs::srv::Trigger>(
    "export_state_to_redis",
    [this](const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
            std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
      HandleExportStateToRedis(request, response);
    });

  import_state_service_ = node_->create_service<std_srvs::srv::Trigger>(
    "import_state_from_redis",
    [this](const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
            std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
      HandleImportStateFromRedis(request, response);
    });
}

Node::~Node() { FinishAllTrajectories(); }

bool Node::handleSubmapQuery(
    const cartographer_ros_msgs::srv::SubmapQuery::Request::SharedPtr request,
    cartographer_ros_msgs::srv::SubmapQuery::Response::SharedPtr response) {
  absl::MutexLock lock(&mutex_);
  map_builder_bridge_->HandleSubmapQuery(request, response);
  return true;
}

bool Node::handleTrajectoryQuery(
    const cartographer_ros_msgs::srv::TrajectoryQuery::Request::SharedPtr request,
    cartographer_ros_msgs::srv::TrajectoryQuery::Response::SharedPtr response) {
  absl::MutexLock lock(&mutex_);
  response->status = TrajectoryStateToStatus(
      request->trajectory_id,
      {TrajectoryState::ACTIVE, TrajectoryState::FINISHED,
       TrajectoryState::FROZEN} /* valid states */);
  if (response->status.code != cartographer_ros_msgs::msg::StatusCode::OK) {
    LOG(ERROR) << "Can't query trajectory from pose graph: "
               << response->status.message;
    return true;
  }
  map_builder_bridge_->HandleTrajectoryQuery(request, response);
  return true;
}

void Node::PublishSubmapList() {
  absl::MutexLock lock(&mutex_);
  submap_list_publisher_->publish(map_builder_bridge_->GetSubmapList(node_->now()));
}

void Node::AddExtrapolator(const int trajectory_id,
                           const TrajectoryOptions& options) {
  constexpr double kExtrapolationEstimationTimeSec = 0.001;  // 1 ms
  CHECK(extrapolators_.count(trajectory_id) == 0);
  const double gravity_time_constant =
      node_options_.map_builder_options.use_trajectory_builder_3d()
          ? options.trajectory_builder_options.trajectory_builder_3d_options()
                .imu_gravity_time_constant()
          : options.trajectory_builder_options.trajectory_builder_2d_options()
                .imu_gravity_time_constant();
  extrapolators_.emplace(
      std::piecewise_construct, std::forward_as_tuple(trajectory_id),
      std::forward_as_tuple(
          ::cartographer::common::FromSeconds(kExtrapolationEstimationTimeSec),
          gravity_time_constant));
}

void Node::AddSensorSamplers(const int trajectory_id,
                             const TrajectoryOptions& options) {
  CHECK(sensor_samplers_.count(trajectory_id) == 0);
  sensor_samplers_.emplace(
      std::piecewise_construct, std::forward_as_tuple(trajectory_id),
      std::forward_as_tuple(
          options.rangefinder_sampling_ratio, options.odometry_sampling_ratio,
          options.fixed_frame_pose_sampling_ratio, options.imu_sampling_ratio,
          options.landmarks_sampling_ratio));
}

void Node::PublishLocalTrajectoryData() {
  absl::MutexLock lock(&mutex_);
  for (const auto& entry : map_builder_bridge_->GetLocalTrajectoryData()) {
    const auto& trajectory_data = entry.second;

    auto& extrapolator = extrapolators_.at(entry.first);
    // We only publish a point cloud if it has changed. It is not needed at high
    // frequency, and republishing it would be computationally wasteful.
    if (trajectory_data.local_slam_data->time !=
        extrapolator.GetLastPoseTime()) {
      if (scan_matched_point_cloud_publisher_->get_subscription_count() > 0) {
        // TODO(gaschler): Consider using other message without time
        // information.
        carto::sensor::TimedPointCloud point_cloud;
        point_cloud.reserve(trajectory_data.local_slam_data->range_data_in_local
                                .returns.size());
        for (const cartographer::sensor::RangefinderPoint & point :
             trajectory_data.local_slam_data->range_data_in_local.returns) {
          point_cloud.push_back(cartographer::sensor::ToTimedRangefinderPoint(
              point, 0.f /* time */));
        }
        scan_matched_point_cloud_publisher_->publish(ToPointCloud2Message(
            carto::common::ToUniversal(trajectory_data.local_slam_data->time),
            node_options_.map_frame,
            carto::sensor::TransformTimedPointCloud(
                point_cloud, trajectory_data.local_to_map.cast<float>())));
      }
      extrapolator.AddPose(trajectory_data.local_slam_data->time,
                           trajectory_data.local_slam_data->local_pose);
    }

    geometry_msgs::msg::TransformStamped stamped_transform;
    // If we do not publish a new point cloud, we still allow time of the
    // published poses to advance. If we already know a newer pose, we use its
    // time instead. Since tf knows how to interpolate, providing newer
    // information is better.
    const ::cartographer::common::Time now = std::max(
        FromRos(node_->now()), extrapolator.GetLastExtrapolatedTime());
    stamped_transform.header.stamp =
        node_options_.use_pose_extrapolator
            ? ToRos(now)
            : ToRos(trajectory_data.local_slam_data->time);

    // Suppress publishing if we already published a transform at this time.
    // Due to 2020-07 changes to geometry2, tf buffer will issue warnings for
    // repeated transforms with the same timestamp.
    if (last_published_tf_stamps_.count(entry.first) &&
        last_published_tf_stamps_[entry.first] == stamped_transform.header.stamp)
      continue;
    last_published_tf_stamps_[entry.first] = stamped_transform.header.stamp;

    const Rigid3d tracking_to_local_3d =
        node_options_.use_pose_extrapolator
            ? extrapolator.ExtrapolatePose(now)
            : trajectory_data.local_slam_data->local_pose;
    const Rigid3d tracking_to_local = [&] {
      if (trajectory_data.trajectory_options.publish_frame_projected_to_2d) {
        return carto::transform::Embed3D(
            carto::transform::Project2D(tracking_to_local_3d));
      }
      return tracking_to_local_3d;
    }();

    const Rigid3d tracking_to_map =
        trajectory_data.local_to_map * tracking_to_local;

    if (trajectory_data.published_to_tracking != nullptr) {
      if (node_options_.publish_to_tf) {
        if (trajectory_data.trajectory_options.provide_odom_frame) {
          std::vector<geometry_msgs::msg::TransformStamped> stamped_transforms;

          stamped_transform.header.frame_id = node_options_.map_frame;
          stamped_transform.child_frame_id =
              trajectory_data.trajectory_options.odom_frame;
          stamped_transform.transform =
              ToGeometryMsgTransform(trajectory_data.local_to_map);
          stamped_transforms.push_back(stamped_transform);

          stamped_transform.header.frame_id =
              trajectory_data.trajectory_options.odom_frame;
          stamped_transform.child_frame_id =
              trajectory_data.trajectory_options.published_frame;
          stamped_transform.transform = ToGeometryMsgTransform(
              tracking_to_local * (*trajectory_data.published_to_tracking));
          stamped_transforms.push_back(stamped_transform);

          tf_broadcaster_->sendTransform(stamped_transforms);
        } else {
          stamped_transform.header.frame_id = node_options_.map_frame;
          stamped_transform.child_frame_id =
              trajectory_data.trajectory_options.published_frame;
          stamped_transform.transform = ToGeometryMsgTransform(
              tracking_to_map * (*trajectory_data.published_to_tracking));
          tf_broadcaster_->sendTransform(stamped_transform);
        }
      }
      if (node_options_.publish_tracked_pose) {
        ::geometry_msgs::msg::PoseStamped pose_msg;
        pose_msg.header.frame_id = node_options_.map_frame;
        pose_msg.header.stamp = stamped_transform.header.stamp;
        pose_msg.pose = ToGeometryMsgPose(tracking_to_map);
        tracked_pose_publisher_->publish(pose_msg);
      }
    }
  }
}

void Node::PublishTrajectoryNodeList() {
  if (trajectory_node_list_publisher_->get_subscription_count() > 0) {
    absl::MutexLock lock(&mutex_);
    trajectory_node_list_publisher_->publish(
        map_builder_bridge_->GetTrajectoryNodeList(node_->now()));
  }
}

void Node::PublishLandmarkPosesList() {
  if (landmark_poses_list_publisher_->get_subscription_count() > 0) {
    absl::MutexLock lock(&mutex_);
    landmark_poses_list_publisher_->publish(
        map_builder_bridge_->GetLandmarkPosesList(node_->now()));
  }
}

void Node::PublishConstraintList() {
  if (constraint_list_publisher_->get_subscription_count() > 0) {
    absl::MutexLock lock(&mutex_);
    constraint_list_publisher_->publish(map_builder_bridge_->GetConstraintList(node_->now()));
  }
}

std::set<cartographer::mapping::TrajectoryBuilderInterface::SensorId>
Node::ComputeExpectedSensorIds(const TrajectoryOptions& options) const {
  using SensorId = cartographer::mapping::TrajectoryBuilderInterface::SensorId;
  using SensorType = SensorId::SensorType;
  std::set<SensorId> expected_topics;
  // Subscribe to all laser scan, multi echo laser scan, and point cloud topics.
  for (const std::string& topic :
       ComputeRepeatedTopicNames(kLaserScanTopic, options.num_laser_scans)) {
    expected_topics.insert(SensorId{SensorType::RANGE, topic});
  }
  for (const std::string& topic : ComputeRepeatedTopicNames(
           kMultiEchoLaserScanTopic, options.num_multi_echo_laser_scans)) {
    expected_topics.insert(SensorId{SensorType::RANGE, topic});
  }
  for (const std::string& topic :
       ComputeRepeatedTopicNames(kPointCloud2Topic, options.num_point_clouds)) {
    expected_topics.insert(SensorId{SensorType::RANGE, topic});
  }
  // For 2D SLAM, subscribe to the IMU if we expect it. For 3D SLAM, the IMU is
  // required.
  if (node_options_.map_builder_options.use_trajectory_builder_3d() ||
      (node_options_.map_builder_options.use_trajectory_builder_2d() &&
       options.trajectory_builder_options.trajectory_builder_2d_options()
           .use_imu_data())) {
    expected_topics.insert(SensorId{SensorType::IMU, kImuTopic});
  }
  // Odometry is optional.
  if (options.use_odometry) {
    expected_topics.insert(SensorId{SensorType::ODOMETRY, kOdometryTopic});
  }
  // NavSatFix is optional.
  if (options.use_nav_sat) {
    expected_topics.insert(
        SensorId{SensorType::FIXED_FRAME_POSE, kNavSatFixTopic});
  }
  // Landmark is optional.
  if (options.use_landmarks) {
    expected_topics.insert(SensorId{SensorType::LANDMARK, kLandmarkTopic});
  }
  return expected_topics;
}

int Node::AddTrajectory(const TrajectoryOptions& options) {
  const std::set<cartographer::mapping::TrajectoryBuilderInterface::SensorId>
      expected_sensor_ids = ComputeExpectedSensorIds(options);
  const int trajectory_id =
      map_builder_bridge_->AddTrajectory(expected_sensor_ids, options);
  AddExtrapolator(trajectory_id, options);
  AddSensorSamplers(trajectory_id, options);
  LaunchSubscribers(options, trajectory_id);
  maybe_warn_about_topic_mismatch_timer_ = node_->create_wall_timer(
    std::chrono::milliseconds(int(kTopicMismatchCheckDelaySec * 1000)),
    [this]() {
      MaybeWarnAboutTopicMismatch();
    });
  for (const auto& sensor_id : expected_sensor_ids) {
    subscribed_topics_.insert(sensor_id.id);
  }
  return trajectory_id;
}

void Node::LaunchSubscribers(const TrajectoryOptions& options,
                             const int trajectory_id) {
  for (const std::string& topic :
       ComputeRepeatedTopicNames(kLaserScanTopic, options.num_laser_scans)) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<sensor_msgs::msg::LaserScan>(
             &Node::HandleLaserScanMessage, trajectory_id, topic, node_, this),
         topic});
  }
  for (const std::string& topic : ComputeRepeatedTopicNames(
           kMultiEchoLaserScanTopic, options.num_multi_echo_laser_scans)) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<sensor_msgs::msg::MultiEchoLaserScan>(
             &Node::HandleMultiEchoLaserScanMessage, trajectory_id, topic, node_, this),
         topic});
  }
  for (const std::string& topic :
       ComputeRepeatedTopicNames(kPointCloud2Topic, options.num_point_clouds)) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<sensor_msgs::msg::PointCloud2>(
             &Node::HandlePointCloud2Message, trajectory_id, topic, node_, this),
         topic});
  }

  // For 2D SLAM, subscribe to the IMU if we expect it. For 3D SLAM, the IMU is
  // required.
  if (node_options_.map_builder_options.use_trajectory_builder_3d() ||
      (node_options_.map_builder_options.use_trajectory_builder_2d() &&
       options.trajectory_builder_options.trajectory_builder_2d_options()
           .use_imu_data())) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<sensor_msgs::msg::Imu>(&Node::HandleImuMessage,
                                                trajectory_id, kImuTopic,
                                                node_, this),
         kImuTopic});
  }

  if (options.use_odometry) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<nav_msgs::msg::Odometry>(&Node::HandleOdometryMessage,
                                                  trajectory_id, kOdometryTopic,
                                                  node_, this),
         kOdometryTopic});
  }
  if (options.use_nav_sat) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<sensor_msgs::msg::NavSatFix>(
             &Node::HandleNavSatFixMessage, trajectory_id, kNavSatFixTopic,
             node_, this),
         kNavSatFixTopic});
  }
  if (options.use_landmarks) {
    subscribers_[trajectory_id].push_back(
        {SubscribeWithHandler<cartographer_ros_msgs::msg::LandmarkList>(
             &Node::HandleLandmarkMessage, trajectory_id, kLandmarkTopic,
             node_, this),
         kLandmarkTopic});
  }
}

bool Node::ValidateTrajectoryOptions(const TrajectoryOptions& options) {
  if (node_options_.map_builder_options.use_trajectory_builder_2d()) {
    return options.trajectory_builder_options
        .has_trajectory_builder_2d_options();
  }
  if (node_options_.map_builder_options.use_trajectory_builder_3d()) {
    return options.trajectory_builder_options
        .has_trajectory_builder_3d_options();
  }
  return false;
}

bool Node::ValidateTopicNames(const TrajectoryOptions& options) {
  for (const auto& sensor_id : ComputeExpectedSensorIds(options)) {
    const std::string& topic = sensor_id.id;
    if (subscribed_topics_.count(topic) > 0) {
      LOG(ERROR) << "Topic name [" << topic << "] is already used.";
      return false;
    }
  }
  return true;
}

cartographer_ros_msgs::msg::StatusResponse Node::TrajectoryStateToStatus(
    const int trajectory_id, const std::set<TrajectoryState>& valid_states) {
  const auto trajectory_states = map_builder_bridge_->GetTrajectoryStates();
  cartographer_ros_msgs::msg::StatusResponse status_response;

  const auto it = trajectory_states.find(trajectory_id);
  if (it == trajectory_states.end()) {
    status_response.message = "Trajectory " + std::to_string(trajectory_id) + " doesn't exist.";
    status_response.code = cartographer_ros_msgs::msg::StatusCode::NOT_FOUND;
    return status_response;
  }

  status_response.message = "Trajectory " + std::to_string(trajectory_id) + " is in '" +
    TrajectoryStateToString(it->second) + "' state.";
  status_response.code =
      valid_states.count(it->second)
          ? cartographer_ros_msgs::msg::StatusCode::OK
          : cartographer_ros_msgs::msg::StatusCode::INVALID_ARGUMENT;
  return status_response;
}

cartographer_ros_msgs::msg::StatusResponse Node::FinishTrajectoryUnderLock(
    const int trajectory_id) {
  cartographer_ros_msgs::msg::StatusResponse status_response;
  if (trajectories_scheduled_for_finish_.count(trajectory_id)) {
    status_response.message =
        "Trajectory " + std::to_string(trajectory_id) + " already pending to finish.";
    status_response.code = cartographer_ros_msgs::msg::StatusCode::OK;
    LOG(INFO) << status_response.message;
    return status_response;
  }

  // First, check if we can actually finish the trajectory.
  status_response = TrajectoryStateToStatus(
      trajectory_id, {TrajectoryState::ACTIVE} /* valid states */);
  if (status_response.code != cartographer_ros_msgs::msg::StatusCode::OK) {
    LOG(ERROR) << "Can't finish trajectory: " << status_response.message;
    return status_response;
  }

  // Shutdown the subscribers of this trajectory.
  // A valid case with no subscribers is e.g. if we just visualize states.
  if (subscribers_.count(trajectory_id)) {
    for (auto& entry : subscribers_[trajectory_id]) {
      entry.subscriber.reset();
      subscribed_topics_.erase(entry.topic);
      LOG(INFO) << "Shutdown the subscriber of [" << entry.topic << "]";
    }
    CHECK_EQ(subscribers_.erase(trajectory_id), 1);
  }
  map_builder_bridge_->FinishTrajectory(trajectory_id);
  trajectories_scheduled_for_finish_.emplace(trajectory_id);
  status_response.message =
      "Finished trajectory " + std::to_string(trajectory_id) + ".";
  status_response.code = cartographer_ros_msgs::msg::StatusCode::OK;
  return status_response;
}

bool Node::handleStartTrajectory(
    const cartographer_ros_msgs::srv::StartTrajectory::Request::SharedPtr request,
    cartographer_ros_msgs::srv::StartTrajectory::Response::SharedPtr response) {
  TrajectoryOptions trajectory_options;
  std::tie(std::ignore, trajectory_options) = LoadOptions(
      request->configuration_directory, request->configuration_basename);

  if (request->use_initial_pose) {
    const auto pose = ToRigid3d(request->initial_pose);
    if (!pose.IsValid()) {
      response->status.message =
          "Invalid pose argument. Orientation quaternion must be normalized.";
      LOG(ERROR) << response->status.message;
      response->status.code =
          cartographer_ros_msgs::msg::StatusCode::INVALID_ARGUMENT;
      return true;
    }

    // Check if the requested trajectory for the relative initial pose exists.
    response->status = TrajectoryStateToStatus(
        request->relative_to_trajectory_id,
        {TrajectoryState::ACTIVE, TrajectoryState::FROZEN,
         TrajectoryState::FINISHED} /* valid states */);
    if (response->status.code != cartographer_ros_msgs::msg::StatusCode::OK) {
      LOG(ERROR) << "Can't start a trajectory with initial pose: "
                 << response->status.message;
      return true;
    }

    ::cartographer::mapping::proto::InitialTrajectoryPose
        initial_trajectory_pose;
    initial_trajectory_pose.set_to_trajectory_id(
        request->relative_to_trajectory_id);
    *initial_trajectory_pose.mutable_relative_pose() =
        cartographer::transform::ToProto(pose);
    initial_trajectory_pose.set_timestamp(cartographer::common::ToUniversal(
        ::cartographer_ros::FromRos(rclcpp::Time(0))));
    *trajectory_options.trajectory_builder_options
         .mutable_initial_trajectory_pose() = initial_trajectory_pose;
  }

  if (!ValidateTrajectoryOptions(trajectory_options)) {
    response->status.message = "Invalid trajectory options.";
    LOG(ERROR) << response->status.message;
    response->status.code = cartographer_ros_msgs::msg::StatusCode::INVALID_ARGUMENT;
  } else if (!ValidateTopicNames(trajectory_options)) {
    response->status.message = "Topics are already used by another trajectory.";
    LOG(ERROR) << response->status.message;
    response->status.code = cartographer_ros_msgs::msg::StatusCode::INVALID_ARGUMENT;
  } else {
    response->status.message = "Success.";
    response->trajectory_id = AddTrajectory(trajectory_options);
    response->status.code = cartographer_ros_msgs::msg::StatusCode::OK;
  }
  return true;
}

void Node::StartTrajectoryWithDefaultTopics(const TrajectoryOptions& options) {
  absl::MutexLock lock(&mutex_);
  CHECK(ValidateTrajectoryOptions(options));
  AddTrajectory(options);
}

std::vector<
    std::set<cartographer::mapping::TrajectoryBuilderInterface::SensorId>>
Node::ComputeDefaultSensorIdsForMultipleBags(
    const std::vector<TrajectoryOptions>& bags_options) const {
  using SensorId = cartographer::mapping::TrajectoryBuilderInterface::SensorId;
  std::vector<std::set<SensorId>> bags_sensor_ids;
  for (size_t i = 0; i < bags_options.size(); ++i) {
    std::string prefix;
    if (bags_options.size() > 1) {
      prefix = "bag_" + std::to_string(i + 1) + "_";
    }
    std::set<SensorId> unique_sensor_ids;
    for (const auto& sensor_id : ComputeExpectedSensorIds(bags_options.at(i))) {
      unique_sensor_ids.insert(SensorId{sensor_id.type, prefix + sensor_id.id});
    }
    bags_sensor_ids.push_back(unique_sensor_ids);
  }
  return bags_sensor_ids;
}

int Node::AddOfflineTrajectory(
    const std::set<cartographer::mapping::TrajectoryBuilderInterface::SensorId>&
        expected_sensor_ids,
    const TrajectoryOptions& options) {
  absl::MutexLock lock(&mutex_);
  const int trajectory_id =
      map_builder_bridge_->AddTrajectory(expected_sensor_ids, options);
  AddExtrapolator(trajectory_id, options);
  AddSensorSamplers(trajectory_id, options);
  return trajectory_id;
}

bool Node::handleGetTrajectoryStates(
    const cartographer_ros_msgs::srv::GetTrajectoryStates::Request::SharedPtr ,
    cartographer_ros_msgs::srv::GetTrajectoryStates::Response::SharedPtr response) {

  using TrajectoryState =
      ::cartographer::mapping::PoseGraphInterface::TrajectoryState;
  absl::MutexLock lock(&mutex_);
  response->status.code = ::cartographer_ros_msgs::msg::StatusCode::OK;
  response->trajectory_states.header.stamp = node_->now();
  for (const auto& entry : map_builder_bridge_->GetTrajectoryStates()) {
    response->trajectory_states.trajectory_id.push_back(entry.first);
    switch (entry.second) {
      case TrajectoryState::ACTIVE:
        response->trajectory_states.trajectory_state.push_back(
            ::cartographer_ros_msgs::msg::TrajectoryStates::ACTIVE);
        break;
      case TrajectoryState::FINISHED:
        response->trajectory_states.trajectory_state.push_back(
            ::cartographer_ros_msgs::msg::TrajectoryStates::FINISHED);
        break;
      case TrajectoryState::FROZEN:
        response->trajectory_states.trajectory_state.push_back(
            ::cartographer_ros_msgs::msg::TrajectoryStates::FROZEN);
        break;
      case TrajectoryState::DELETED:
        response->trajectory_states.trajectory_state.push_back(
            ::cartographer_ros_msgs::msg::TrajectoryStates::DELETED);
        break;
    }
  }
  return true;
}

bool Node::handleFinishTrajectory(
    const cartographer_ros_msgs::srv::FinishTrajectory::Request::SharedPtr request,
    cartographer_ros_msgs::srv::FinishTrajectory::Response::SharedPtr response) {
  absl::MutexLock lock(&mutex_);
  response->status = FinishTrajectoryUnderLock(request->trajectory_id);
  return true;
}

bool Node::handleWriteState(
    const cartographer_ros_msgs::srv::WriteState::Request::SharedPtr request,
    cartographer_ros_msgs::srv::WriteState::Response::SharedPtr response) {
  absl::MutexLock lock(&mutex_);
  if (map_builder_bridge_->SerializeState(request->filename,
                                         request->include_unfinished_submaps)) {
    response->status.code = cartographer_ros_msgs::msg::StatusCode::OK;
    response->status.message =
        "State written to '" + request->filename + "'.";
  } else {
    response->status.code = cartographer_ros_msgs::msg::StatusCode::INVALID_ARGUMENT;
    response->status.message =
        "Failed to write '" + request->filename + "'.";
  }
  return true;
}

bool Node::handleReadMetrics(
    const cartographer_ros_msgs::srv::ReadMetrics::Request::SharedPtr,
    cartographer_ros_msgs::srv::ReadMetrics::Response::SharedPtr response) {

  absl::MutexLock lock(&mutex_);
  response->timestamp = node_->now();
  if (!metrics_registry_) {
    response->status.code = cartographer_ros_msgs::msg::StatusCode::UNAVAILABLE;
    response->status.message = "Collection of runtime metrics is not activated.";
    return true;
  }
  metrics_registry_->ReadMetrics(response);
  response->status.code = cartographer_ros_msgs::msg::StatusCode::OK;
  response->status.message = "Successfully read metrics.";
  return true;
}

void Node::FinishAllTrajectories() {
  absl::MutexLock lock(&mutex_);
  for (const auto& entry : map_builder_bridge_->GetTrajectoryStates()) {
    if (entry.second == TrajectoryState::ACTIVE) {
      const int trajectory_id = entry.first;
      CHECK_EQ(FinishTrajectoryUnderLock(trajectory_id).code,
               cartographer_ros_msgs::msg::StatusCode::OK);
    }
  }
}

bool Node::FinishTrajectory(const int trajectory_id) {
  absl::MutexLock lock(&mutex_);
  return FinishTrajectoryUnderLock(trajectory_id).code ==
         cartographer_ros_msgs::msg::StatusCode::OK;
}

void Node::RunFinalOptimization() {
  {
    for (const auto& entry : map_builder_bridge_->GetTrajectoryStates()) {
      const int trajectory_id = entry.first;
      if (entry.second == TrajectoryState::ACTIVE) {
        LOG(WARNING)
            << "Can't run final optimization if there are one or more active "
               "trajectories. Trying to finish trajectory with ID "
            << std::to_string(trajectory_id) << " now.";
        CHECK(FinishTrajectory(trajectory_id))
            << "Failed to finish trajectory with ID "
            << std::to_string(trajectory_id) << ".";
      }
    }
  }
  // Assuming we are not adding new data anymore, the final optimization
  // can be performed without holding the mutex.
  map_builder_bridge_->RunFinalOptimization();
}

void Node::HandleOdometryMessage(const int trajectory_id,
                                 const std::string& sensor_id,
                                 const nav_msgs::msg::Odometry::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).odometry_sampler.Pulse()) {
    return;
  }
  auto sensor_bridge_ptr = map_builder_bridge_->sensor_bridge(trajectory_id);
  auto odometry_data_ptr = sensor_bridge_ptr->ToOdometryData(msg);
  if (odometry_data_ptr != nullptr) {
    extrapolators_.at(trajectory_id).AddOdometryData(*odometry_data_ptr);
  }
  sensor_bridge_ptr->HandleOdometryMessage(sensor_id, msg);

  // The callback still holds mutex_. Therefore an export that acquires the
  // same mutex afterwards can safely regard this as a fully handled odometry
  // message at the Node/SensorBridge boundary.
  RecordProcessedSensorStamp(
      RosStampToNanoseconds(msg->header.stamp),
      &g_first_processed_odom_stamp_ns, &g_last_processed_odom_stamp_ns);
}

void Node::HandleNavSatFixMessage(const int trajectory_id,
                                  const std::string& sensor_id,
                                  const sensor_msgs::msg::NavSatFix::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).fixed_frame_pose_sampler.Pulse()) {
    return;
  }
  map_builder_bridge_->sensor_bridge(trajectory_id)
      ->HandleNavSatFixMessage(sensor_id, msg);
}

void Node::HandleLandmarkMessage(
    const int trajectory_id, const std::string& sensor_id,
    const cartographer_ros_msgs::msg::LandmarkList::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).landmark_sampler.Pulse()) {
    return;
  }
  map_builder_bridge_->sensor_bridge(trajectory_id)
      ->HandleLandmarkMessage(sensor_id, msg);
}

void Node::HandleImuMessage(const int trajectory_id,
                            const std::string& sensor_id,
                            const sensor_msgs::msg::Imu::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).imu_sampler.Pulse()) {
    return;
  }
  auto sensor_bridge_ptr = map_builder_bridge_->sensor_bridge(trajectory_id);
  auto imu_data_ptr = sensor_bridge_ptr->ToImuData(msg);
  if (imu_data_ptr != nullptr) {
    extrapolators_.at(trajectory_id).AddImuData(*imu_data_ptr);
  }
  sensor_bridge_ptr->HandleImuMessage(sensor_id, msg);
}

void Node::HandleLaserScanMessage(const int trajectory_id,
                                  const std::string& sensor_id,
                                  const sensor_msgs::msg::LaserScan::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).rangefinder_sampler.Pulse()) {
    return;
  }
  map_builder_bridge_->sensor_bridge(trajectory_id)
      ->HandleLaserScanMessage(sensor_id, msg);

  RecordProcessedSensorStamp(
      RosStampToNanoseconds(msg->header.stamp),
      &g_first_processed_scan_stamp_ns, &g_last_processed_scan_stamp_ns);
}

void Node::HandleMultiEchoLaserScanMessage(
    const int trajectory_id, const std::string& sensor_id,
    const sensor_msgs::msg::MultiEchoLaserScan::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).rangefinder_sampler.Pulse()) {
    return;
  }
  map_builder_bridge_->sensor_bridge(trajectory_id)
      ->HandleMultiEchoLaserScanMessage(sensor_id, msg);
}

void Node::HandlePointCloud2Message(
    const int trajectory_id, const std::string& sensor_id,
    const sensor_msgs::msg::PointCloud2::ConstSharedPtr& msg) {
  absl::MutexLock lock(&mutex_);
  if (!sensor_samplers_.at(trajectory_id).rangefinder_sampler.Pulse()) {
    return;
  }
  map_builder_bridge_->sensor_bridge(trajectory_id)
      ->HandlePointCloud2Message(sensor_id, msg);
}

void Node::SerializeState(const std::string& filename,
                          const bool include_unfinished_submaps) {
  absl::MutexLock lock(&mutex_);
  CHECK(
      map_builder_bridge_->SerializeState(filename, include_unfinished_submaps))
      << "Could not write state.";
}

void Node::LoadState(const std::string& state_filename,
                     const bool load_frozen_state) {
  absl::MutexLock lock(&mutex_);
  map_builder_bridge_->LoadState(state_filename, load_frozen_state);
}

// TODO: find ROS equivalent to ros::master::getTopics
void Node::MaybeWarnAboutTopicMismatch() {
//  ::ros::master::V_TopicInfo ros_topics;
//  ::ros::master::getTopics(ros_topics);
//  std::set<std::string> published_topics;
//  std::stringstream published_topics_string;
//  for (const auto& it : ros_topics) {
//    std::string resolved_topic = node_handle_.resolveName(it.name, false);
//    published_topics.insert(resolved_topic);
//    published_topics_string << resolved_topic << ",";
//  }
//  bool print_topics = false;
//  for (const auto& entry : subscribers_) {
//    int trajectory_id = entry.first;
//    for (const auto& subscriber : entry.second) {
//      std::string resolved_topic = node_handle_.resolveName(subscriber.topic);
//      if (published_topics.count(resolved_topic) == 0) {
//        LOG(WARNING) << "Expected topic \"" << subscriber.topic
//                     << "\" (trajectory " << trajectory_id << ")"
//                     << " (resolved topic \"" << resolved_topic << "\")"
//                     << " but no publisher is currently active.";
//        print_topics = true;
//      }
//    }
//  }
//  if (print_topics) {
//    LOG(WARNING) << "Currently available topics are: "
//                 << published_topics_string.str();
//  }
}

bool Node::LookupPoseAsJson(const std::string& parent_frame,
                            const std::string& child_frame,
                            std::string* json_out) {
  if (!tf_buffer_) {
    RCLCPP_ERROR(node_->get_logger(), "tf_buffer_ is null.");
    return false;
  }

  geometry_msgs::msg::TransformStamped tf_msg;
  try {
    tf_msg = tf_buffer_->lookupTransform(
        parent_frame, child_frame,
        tf2::TimePointZero,
        tf2::durationFromSec(0.5));
  } catch (const tf2::TransformException& e) {
    RCLCPP_ERROR(node_->get_logger(),
                 "Failed to lookup %s->%s during export: %s",
                 parent_frame.c_str(), child_frame.c_str(), e.what());
    return false;
  }

  std::ostringstream oss;
  oss << "{"
      << "\"frame_id\":\"" << parent_frame << "\","
      << "\"child_frame_id\":\"" << child_frame << "\","
      << "\"px\":" << tf_msg.transform.translation.x << ","
      << "\"py\":" << tf_msg.transform.translation.y << ","
      << "\"pz\":" << tf_msg.transform.translation.z << ","
      << "\"qx\":" << tf_msg.transform.rotation.x << ","
      << "\"qy\":" << tf_msg.transform.rotation.y << ","
      << "\"qz\":" << tf_msg.transform.rotation.z << ","
      << "\"qw\":" << tf_msg.transform.rotation.w
      << "}";

  *json_out = oss.str();
  return true;
}

bool Node::SaveCurrentPosesToRedis() {
  std::string map_pose_json;
  std::string odom_pose_json;

  if (!LookupPoseAsJson("map", "base_footprint", &map_pose_json)) {
    return false;
  }
  if (!LookupPoseAsJson("odom", "base_footprint", &odom_pose_json)) {
    return false;
  }

  const int relative_to_trajectory_id =
      FindReferenceTrajectoryId(map_builder_bridge_->GetTrajectoryStates());
  if (relative_to_trajectory_id < 0) {
    RCLCPP_ERROR(node_->get_logger(),
                 "Could not determine reference trajectory id.");
    return false;
  }

  {
    // map pose 側に relative_to_trajectory_id を追加
    const std::string suffix =
        std::string(",\"relative_to_trajectory_id\":") +
        std::to_string(relative_to_trajectory_id) + "}";
    map_pose_json.pop_back();   // remove trailing '}'
    map_pose_json += suffix;
  }

  redisContext* c = redisConnect("redis_host", 6379);
  if (c == nullptr || c->err) {
    RCLCPP_ERROR(node_->get_logger(),
                 "Failed to connect to Redis for pose save.");
    if (c) redisFree(c);
    return false;
  }

  redisReply* reply1 = static_cast<redisReply*>(
      redisCommand(c, "SET %s %b",
                   kRedisMapPoseKey,
                   map_pose_json.data(), map_pose_json.size()));

  redisReply* reply2 = static_cast<redisReply*>(
      redisCommand(c, "SET %s %b",
                   kRedisOdomPoseKey,
                   odom_pose_json.data(), odom_pose_json.size()));

  const bool ok = (reply1 != nullptr && reply2 != nullptr);

  if (reply1) freeReplyObject(reply1);
  if (reply2) freeReplyObject(reply2);
  redisFree(c);

  if (!ok) {
    RCLCPP_ERROR(node_->get_logger(), "Failed to save poses to Redis.");
    return false;
  }

  RCLCPP_INFO(node_->get_logger(),
              "Saved map pose to Redis key '%s': %s",
              kRedisMapPoseKey, map_pose_json.c_str());
  RCLCPP_INFO(node_->get_logger(),
              "Saved odom pose to Redis key '%s': %s",
              kRedisOdomPoseKey, odom_pose_json.c_str());

  return true;
}

/*bool Node::SaveCurrentPoseToRedis() {
  if (!tf_buffer_) {
    RCLCPP_ERROR(node_->get_logger(), "tf_buffer_ is null.");
    return false;
  }

  geometry_msgs::msg::TransformStamped tf_msg;
  try {
    tf_msg = tf_buffer_->lookupTransform(
        "map", "base_footprint", tf2::TimePointZero,
        tf2::durationFromSec(0.5));
  } catch (const tf2::TransformException& e) {
    RCLCPP_ERROR(node_->get_logger(),
                 "Failed to lookup map->base_footprint during export: %s",
                 e.what());
    return false;
  }

  const int relative_to_trajectory_id =
      FindReferenceTrajectoryId(map_builder_bridge_->GetTrajectoryStates());

  if (relative_to_trajectory_id < 0) {
    RCLCPP_ERROR(node_->get_logger(),
                 "Could not determine reference trajectory id.");
    return false;
  }

  std::ostringstream oss;
  oss << "{"
      << "\"frame_id\":\"map\","
      << "\"child_frame_id\":\"base_footprint\","
      << "\"px\":" << tf_msg.transform.translation.x << ","
      << "\"py\":" << tf_msg.transform.translation.y << ","
      << "\"pz\":" << tf_msg.transform.translation.z << ","
      << "\"qx\":" << tf_msg.transform.rotation.x << ","
      << "\"qy\":" << tf_msg.transform.rotation.y << ","
      << "\"qz\":" << tf_msg.transform.rotation.z << ","
      << "\"qw\":" << tf_msg.transform.rotation.w << ","
      << "\"relative_to_trajectory_id\":" << relative_to_trajectory_id
      << "}";

  const std::string pose_json = oss.str();

  redisContext* c = redisConnect("redis_host", 6379);
  if (c == nullptr || c->err) {
    RCLCPP_ERROR(node_->get_logger(), "Failed to connect to Redis for pose save.");
    if (c) redisFree(c);
    return false;
  }

  redisReply* reply = static_cast<redisReply*>(
      redisCommand(c, "SET %s %b",
                   kRedisPoseKey, pose_json.data(), pose_json.size()));

  const bool ok = (reply != nullptr);
  if (reply) freeReplyObject(reply);
  redisFree(c);

  if (!ok) {
    RCLCPP_ERROR(node_->get_logger(), "Failed to save pose JSON to Redis.");
    return false;
  }

  RCLCPP_INFO(node_->get_logger(),
              "Saved current pose to Redis key '%s': %s",
              kRedisPoseKey, pose_json.c_str());
  return true;
}*/

// Redisへのエクスポート (RAMディスク経由)
void Node::HandleExportStateToRedis(
    const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
  (void)request;

  const auto total_begin = HandoverSteadyClock::now();
  const auto mutex_wait_begin = HandoverSteadyClock::now();
  absl::MutexLock lock(&mutex_);
  const auto mutex_acquired = HandoverSteadyClock::now();

  // Sensor callbacks use the same mutex_. Capture these immediately after
  // acquiring the lock so they identify the sensor-time boundary of the state
  // that is about to be serialized. No newer Node sensor callback can run
  // until this export releases mutex_.
  const int64_t first_processed_odom_stamp_ns =
      g_first_processed_odom_stamp_ns.load(std::memory_order_relaxed);
  const int64_t export_state_odom_stamp_ns =
      g_last_processed_odom_stamp_ns.load(std::memory_order_relaxed);
  const int64_t first_processed_scan_stamp_ns =
      g_first_processed_scan_stamp_ns.load(std::memory_order_relaxed);
  const int64_t export_state_scan_stamp_ns =
      g_last_processed_scan_stamp_ns.load(std::memory_order_relaxed);

  // 1) export 時点の map pose / odom pose を保存
  const auto pose_save_begin = HandoverSteadyClock::now();
  if (!SaveCurrentPosesToRedis()) {
    const auto failure_time = HandoverSteadyClock::now();
    response->success = false;
    std::ostringstream oss;
    oss << "Failed to save current map/odom poses to Redis. "
        << "[HANDOVER_EXPORT_ERROR] failed_stage=pose_save"
        << " mutex_wait_ms=" << std::fixed << std::setprecision(3)
        << HandoverMilliseconds(mutex_acquired - mutex_wait_begin)
        << " elapsed_ms="
        << HandoverMilliseconds(failure_time - total_begin);
    response->message = oss.str();
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }
  const auto pose_save_end = HandoverSteadyClock::now();

  // 2) pbstream を従来どおり export
  const std::string ram_file = "/dev/shm/export_state.pbstream";
  const auto serialize_begin = HandoverSteadyClock::now();
  const bool write_success = map_builder_bridge_->SerializeState(ram_file, true);
  const auto serialize_end = HandoverSteadyClock::now();
  if (!write_success) {
    response->success = false;
    std::ostringstream oss;
    oss << "Failed to serialize state to RAM disk. "
        << "[HANDOVER_EXPORT_ERROR] failed_stage=serialize"
        << " mutex_wait_ms=" << std::fixed << std::setprecision(3)
        << HandoverMilliseconds(mutex_acquired - mutex_wait_begin)
        << " pose_save_ms="
        << HandoverMilliseconds(pose_save_end - pose_save_begin)
        << " serialize_ms="
        << HandoverMilliseconds(serialize_end - serialize_begin);
    response->message = oss.str();
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    std::remove(ram_file.c_str());
    return;
  }

  const auto file_read_begin = HandoverSteadyClock::now();
  std::ifstream in(ram_file, std::ios::binary);
  if (!in.is_open()) {
    response->success = false;
    response->message =
        "Failed to open serialized state file. "
        "[HANDOVER_EXPORT_ERROR] failed_stage=file_open";
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    std::remove(ram_file.c_str());
    return;
  }
  std::string state_data((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
  const bool file_read_ok = !in.bad();
  in.close();
  const auto file_read_end = HandoverSteadyClock::now();
  if (!file_read_ok) {
    response->success = false;
    response->message =
        "Failed while reading serialized state file. "
        "[HANDOVER_EXPORT_ERROR] failed_stage=file_read";
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    std::remove(ram_file.c_str());
    return;
  }

  const auto redis_connect_begin = HandoverSteadyClock::now();
  redisContext* c = redisConnect("redis_host", 6379);
  const auto redis_connect_end = HandoverSteadyClock::now();
  if (c == nullptr || c->err) {
    response->success = false;
    response->message =
        "Failed to connect to Redis. "
        "[HANDOVER_EXPORT_ERROR] failed_stage=redis_connect";
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    if (c) redisFree(c);
    std::remove(ram_file.c_str());
    return;
  }

  const auto redis_set_begin = HandoverSteadyClock::now();
  redisReply* reply = static_cast<redisReply*>(
      redisCommand(c, "SET %s %b", kRedisStateKey, state_data.data(),
                   state_data.size()));
  const auto redis_set_end = HandoverSteadyClock::now();

  const bool redis_set_ok =
      reply != nullptr && reply->type != REDIS_REPLY_ERROR;
  std::string redis_error;
  if (reply != nullptr && reply->type == REDIS_REPLY_ERROR && reply->str) {
    redis_error.assign(reply->str, static_cast<std::size_t>(reply->len));
  }
  if (reply) freeReplyObject(reply);
  redisFree(c);

  // Diagnostic build: keep the exact pbstream written to Redis so the
  // standalone component analyzer can inspect this same snapshot after the
  // service returns. Failure paths still remove the temporary file.
  if (!redis_set_ok) {
    std::remove(ram_file.c_str());
    response->success = false;
    std::ostringstream oss;
    oss << "Failed to save state to Redis. "
        << "[HANDOVER_EXPORT_ERROR] failed_stage=redis_set";
    if (!redis_error.empty()) {
      oss << " redis_error=" << redis_error;
    }
    response->message = oss.str();
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }

  const auto total_end = HandoverSteadyClock::now();

  // docker logs が利用できない起動方法でもホスト側から回収できるように、
  // 計測値を Trigger.Response.message 自体へ含める。
  std::ostringstream metrics;
  metrics << "[HANDOVER_EXPORT]"
          << " pbstream_bytes=" << state_data.size()
          << " diagnostic_pbstream_kept=1"
          << " first_processed_odom_stamp_ns=" << first_processed_odom_stamp_ns
          << " export_state_odom_stamp_ns=" << export_state_odom_stamp_ns
          << " first_processed_scan_stamp_ns=" << first_processed_scan_stamp_ns
          << " export_state_scan_stamp_ns=" << export_state_scan_stamp_ns
          << std::fixed << std::setprecision(3)
          << " mutex_wait_ms="
          << HandoverMilliseconds(mutex_acquired - mutex_wait_begin)
          << " pose_save_ms="
          << HandoverMilliseconds(pose_save_end - pose_save_begin)
          << " serialize_ms="
          << HandoverMilliseconds(serialize_end - serialize_begin)
          << " file_read_ms="
          << HandoverMilliseconds(file_read_end - file_read_begin)
          << " redis_connect_ms="
          << HandoverMilliseconds(redis_connect_end - redis_connect_begin)
          << " redis_set_ms="
          << HandoverMilliseconds(redis_set_end - redis_set_begin)
          << " total_ms="
          << HandoverMilliseconds(total_end - total_begin);

  const std::string metrics_string = metrics.str();
  response->success = true;
  response->message = metrics_string;
  RCLCPP_INFO(node_->get_logger(), "%s", metrics_string.c_str());
}

// Redisからのインポートと起動 (RAMディスク経由)
void Node::HandleImportStateFromRedis(
    const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
  (void)request;

  const auto total_begin = HandoverSteadyClock::now();
  const auto mutex_wait_begin = HandoverSteadyClock::now();
  absl::MutexLock lock(&mutex_);
  const auto mutex_acquired = HandoverSteadyClock::now();

  const auto redis_connect_begin = HandoverSteadyClock::now();
  redisContext* c = redisConnect("redis_host", 6379);
  const auto redis_connect_end = HandoverSteadyClock::now();
  if (c == nullptr || c->err) {
    response->success = false;
    response->message =
        "Failed to connect to Redis. "
        "[HANDOVER_IMPORT_ERROR] failed_stage=redis_connect";
    if (c) redisFree(c);
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }

  const auto redis_get_begin = HandoverSteadyClock::now();
  redisReply* reply = static_cast<redisReply*>(
      redisCommand(c, "GET %s", kRedisStateKey));
  const auto redis_get_end = HandoverSteadyClock::now();

  if (reply == nullptr) {
    response->success = false;
    response->message =
        "Redis GET failed. [HANDOVER_IMPORT_ERROR] failed_stage=redis_get";
    redisFree(c);
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }

  if (reply->type != REDIS_REPLY_STRING) {
    response->success = false;
    response->message =
        "No state found in Redis. "
        "[HANDOVER_IMPORT_ERROR] failed_stage=redis_get_reply";
    freeReplyObject(reply);
    redisFree(c);
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }

  const std::size_t pbstream_bytes = static_cast<std::size_t>(reply->len);
  const std::string ram_file = "/dev/shm/import_state.pbstream";

  const auto file_write_begin = HandoverSteadyClock::now();
  std::ofstream out(ram_file, std::ios::binary);
  if (!out.is_open()) {
    response->success = false;
    response->message =
        "Failed to open import state file. "
        "[HANDOVER_IMPORT_ERROR] failed_stage=file_open";
    freeReplyObject(reply);
    redisFree(c);
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }
  out.write(reply->str, reply->len);
  const bool file_write_ok = out.good();
  out.close();
  const auto file_write_end = HandoverSteadyClock::now();

  freeReplyObject(reply);
  redisFree(c);

  if (!file_write_ok) {
    response->success = false;
    response->message =
        "Failed while writing import state file. "
        "[HANDOVER_IMPORT_ERROR] failed_stage=file_write";
    std::remove(ram_file.c_str());
    RCLCPP_ERROR(node_->get_logger(), "%s", response->message.c_str());
    return;
  }

  const auto load_state_begin = HandoverSteadyClock::now();
  map_builder_bridge_->LoadState(ram_file, true);
  const auto load_state_end = HandoverSteadyClock::now();

  std::remove(ram_file.c_str());
  const auto total_end = HandoverSteadyClock::now();

  std::ostringstream metrics;
  metrics << "[HANDOVER_IMPORT]"
          << " pbstream_bytes=" << pbstream_bytes
          << std::fixed << std::setprecision(3)
          << " mutex_wait_ms="
          << HandoverMilliseconds(mutex_acquired - mutex_wait_begin)
          << " redis_connect_ms="
          << HandoverMilliseconds(redis_connect_end - redis_connect_begin)
          << " redis_get_ms="
          << HandoverMilliseconds(redis_get_end - redis_get_begin)
          << " file_write_ms="
          << HandoverMilliseconds(file_write_end - file_write_begin)
          << " load_state_ms="
          << HandoverMilliseconds(load_state_end - load_state_begin)
          << " total_ms="
          << HandoverMilliseconds(total_end - total_begin);

  const std::string metrics_string = metrics.str();
  response->success = true;
  response->message = metrics_string;
  RCLCPP_INFO(node_->get_logger(), "%s", metrics_string.c_str());
}

}  // namespace cartographer_ros