// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from cartographer_ros_msgs:srv/SetSlamState.idl
// generated code does not contain a copyright notice

#ifndef CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__SET_SLAM_STATE__BUILDER_HPP_
#define CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__SET_SLAM_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "cartographer_ros_msgs/srv/detail/set_slam_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace cartographer_ros_msgs
{

namespace srv
{

namespace builder
{

class Init_SetSlamState_Request_load_frozen_state
{
public:
  explicit Init_SetSlamState_Request_load_frozen_state(::cartographer_ros_msgs::srv::SetSlamState_Request & msg)
  : msg_(msg)
  {}
  ::cartographer_ros_msgs::srv::SetSlamState_Request load_frozen_state(::cartographer_ros_msgs::srv::SetSlamState_Request::_load_frozen_state_type arg)
  {
    msg_.load_frozen_state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::cartographer_ros_msgs::srv::SetSlamState_Request msg_;
};

class Init_SetSlamState_Request_ram_disk_path
{
public:
  Init_SetSlamState_Request_ram_disk_path()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetSlamState_Request_load_frozen_state ram_disk_path(::cartographer_ros_msgs::srv::SetSlamState_Request::_ram_disk_path_type arg)
  {
    msg_.ram_disk_path = std::move(arg);
    return Init_SetSlamState_Request_load_frozen_state(msg_);
  }

private:
  ::cartographer_ros_msgs::srv::SetSlamState_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::cartographer_ros_msgs::srv::SetSlamState_Request>()
{
  return cartographer_ros_msgs::srv::builder::Init_SetSlamState_Request_ram_disk_path();
}

}  // namespace cartographer_ros_msgs


namespace cartographer_ros_msgs
{

namespace srv
{

namespace builder
{

class Init_SetSlamState_Response_status
{
public:
  Init_SetSlamState_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::cartographer_ros_msgs::srv::SetSlamState_Response status(::cartographer_ros_msgs::srv::SetSlamState_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return std::move(msg_);
  }

private:
  ::cartographer_ros_msgs::srv::SetSlamState_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::cartographer_ros_msgs::srv::SetSlamState_Response>()
{
  return cartographer_ros_msgs::srv::builder::Init_SetSlamState_Response_status();
}

}  // namespace cartographer_ros_msgs

#endif  // CARTOGRAPHER_ROS_MSGS__SRV__DETAIL__SET_SLAM_STATE__BUILDER_HPP_
