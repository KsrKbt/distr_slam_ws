#!/usr/bin/env python3
import json
import os
import sys
import urllib.request
import urllib.error

import rclpy
from rclpy.node import Node
from cartographer_ros_msgs.srv import StartTrajectory


class StartTrajectoryFromGatewayPose(Node):
    def __init__(self) -> None:
        super().__init__("start_trajectory_from_gateway_pose")

        self.gateway_url = os.getenv("GATEWAY_URL", "http://127.0.0.1:8080")
        self.robot_id = os.getenv("ROBOT_ID", "robot1")
        self.service_name = os.getenv("START_TRAJECTORY_SERVICE", "/start_trajectory")

        default_config_dir = (
            "/root/distr_slam_ws/install/cartographer_ros/"
            "share/cartographer_ros/configuration_files"
        )
        self.configuration_directory = os.getenv(
            "CARTOGRAPHER_CONFIG_DIR", default_config_dir
        )
        self.configuration_basename = os.getenv(
            "CARTOGRAPHER_CONFIG_BASENAME", "my_robot.lua"
        )

        self.client = self.create_client(StartTrajectory, self.service_name)

    def fetch_corrected_pose(self):
        url = f"{self.gateway_url}/robots/{self.robot_id}/compute_corrected_pose"
        req = urllib.request.Request(
            url=url,
            method="POST",
            data=b"{}",
            headers={"Content-Type": "application/json"},
        )

        self.get_logger().info(f"Requesting corrected pose from {url}")
        try:
            with urllib.request.urlopen(req, timeout=5.0) as resp:
                payload = resp.read().decode("utf-8")
                return json.loads(payload)
        except urllib.error.HTTPError as e:
            body = e.read().decode("utf-8", errors="replace")
            raise RuntimeError(f"Gateway returned HTTP {e.code}: {body}") from e

    def call_start_trajectory(self, pose_dict):
        self.get_logger().info(f"Waiting for service '{self.service_name}'...")
        if not self.client.wait_for_service(timeout_sec=10.0):
            raise RuntimeError(
                f"Service '{self.service_name}' is not available after waiting."
            )

        req = StartTrajectory.Request()
        req.configuration_directory = self.configuration_directory
        req.configuration_basename = self.configuration_basename
        req.use_initial_pose = True
        req.initial_pose.position.x = float(pose_dict["px"])
        req.initial_pose.position.y = float(pose_dict["py"])
        req.initial_pose.position.z = float(pose_dict["pz"])
        req.initial_pose.orientation.x = float(pose_dict["qx"])
        req.initial_pose.orientation.y = float(pose_dict["qy"])
        req.initial_pose.orientation.z = float(pose_dict["qz"])
        req.initial_pose.orientation.w = float(pose_dict["qw"])
        req.relative_to_trajectory_id = int(pose_dict["relative_to_trajectory_id"])

        self.get_logger().info(
            "Calling /start_trajectory with gateway corrected pose: "
            f"pos=({req.initial_pose.position.x:.6f}, "
            f"{req.initial_pose.position.y:.6f}, "
            f"{req.initial_pose.position.z:.6f}), "
            f"quat=({req.initial_pose.orientation.x:.6f}, "
            f"{req.initial_pose.orientation.y:.6f}, "
            f"{req.initial_pose.orientation.z:.6f}, "
            f"{req.initial_pose.orientation.w:.6f}), "
            f"relative_to_trajectory_id={req.relative_to_trajectory_id}"
        )

        future = self.client.call_async(req)
        rclpy.spin_until_future_complete(self, future, timeout_sec=20.0)

        if not future.done():
            raise RuntimeError("Timed out waiting for /start_trajectory response.")

        result = future.result()
        if result is None:
            raise RuntimeError("Service call returned no result.")
        return result


def main() -> int:
    rclpy.init()
    node = StartTrajectoryFromGatewayPose()
    try:
        pose = node.fetch_corrected_pose()
        response = node.call_start_trajectory(pose)

        print("start_trajectory response:")
        print(f"  status.code: {response.status.code}")
        print(f"  status.message: {response.status.message}")
        print(f"  trajectory_id: {response.trajectory_id}")

        if response.status.code != 0:
            return 1
        return 0
    except Exception as e:
        node.get_logger().error(str(e))
        return 1
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    sys.exit(main())
