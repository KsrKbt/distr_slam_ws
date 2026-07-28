#!/usr/bin/env python3
import os
import sys

import rclpy
from rclpy.node import Node
from cartographer_ros_msgs.srv import StartTrajectory


class StartTrajectoryNoCorrectedPose(Node):
    def __init__(self) -> None:
        super().__init__('start_trajectory_no_corrected_pose')
        self.service_name = os.getenv('START_TRAJECTORY_SERVICE', '/start_trajectory')
        default_config_dir = (
            '/root/distr_slam_ws/install/cartographer_ros/'
            'share/cartographer_ros/configuration_files'
        )
        self.configuration_directory = os.getenv('CARTOGRAPHER_CONFIG_DIR', default_config_dir)
        self.configuration_basename = os.getenv('CARTOGRAPHER_CONFIG_BASENAME', 'my_robot.lua')
        self.relative_to_trajectory_id = int(os.getenv('RELATIVE_TO_TRAJECTORY_ID', '0'))
        self.client = self.create_client(StartTrajectory, self.service_name)

    def call_start_trajectory(self):
        self.get_logger().info(f"Waiting for service '{self.service_name}'...")
        if not self.client.wait_for_service(timeout_sec=10.0):
            raise RuntimeError(f"Service '{self.service_name}' is not available after waiting.")

        req = StartTrajectory.Request()
        req.configuration_directory = self.configuration_directory
        req.configuration_basename = self.configuration_basename

        # Condition A: no corrected pose.
        # Cartographer decides the initial pose without using gateway-corrected map/odom pose.
        req.use_initial_pose = False
        req.initial_pose.position.x = 0.0
        req.initial_pose.position.y = 0.0
        req.initial_pose.position.z = 0.0
        req.initial_pose.orientation.x = 0.0
        req.initial_pose.orientation.y = 0.0
        req.initial_pose.orientation.z = 0.0
        req.initial_pose.orientation.w = 1.0
        req.relative_to_trajectory_id = self.relative_to_trajectory_id

        self.get_logger().info(
            'Calling /start_trajectory WITHOUT corrected pose: '
            f'use_initial_pose={req.use_initial_pose}, '
            f'relative_to_trajectory_id={req.relative_to_trajectory_id}'
        )
        future = self.client.call_async(req)
        rclpy.spin_until_future_complete(self, future, timeout_sec=20.0)
        if not future.done():
            raise RuntimeError('Timed out waiting for /start_trajectory response.')
        result = future.result()
        if result is None:
            raise RuntimeError('Service call returned no result.')
        return result


def main() -> int:
    rclpy.init()
    node = StartTrajectoryNoCorrectedPose()
    try:
        response = node.call_start_trajectory()
        print('start_trajectory response:')
        print(f'  status.code: {response.status.code}')
        print(f'  status.message: {response.status.message}')
        print(f'  trajectory_id: {response.trajectory_id}')
        return 0 if response.status.code == 0 else 1
    except Exception as e:
        node.get_logger().error(str(e))
        return 1
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    sys.exit(main())
