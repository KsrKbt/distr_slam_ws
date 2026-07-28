#!/usr/bin/env python3
"""Start a Cartographer trajectory from a gateway-computed corrected pose.

The /start_trajectory service is awaited before requesting the corrected pose,
which minimizes the age of the odometry sample used for the correction.
The gateway already returns current_odom_stamp_ns; this script records it in a
metadata JSON file so the host-side evaluation script can calculate pose age at
trajectory start and at gateway switch.
"""
from __future__ import annotations

import json
import os
import sys
import time
import urllib.error
import urllib.request
from pathlib import Path

import rclpy
from cartographer_ros_msgs.srv import StartTrajectory
from rclpy.node import Node


class StartTrajectoryFromGatewayPose(Node):
    def __init__(self) -> None:
        super().__init__("start_trajectory_from_gateway_pose")

        self.gateway_url = os.getenv("GATEWAY_URL", "http://127.0.0.1:8080")
        self.robot_id = os.getenv("ROBOT_ID", "robot1")
        self.service_name = os.getenv("START_TRAJECTORY_SERVICE", "/start_trajectory")
        self.metadata_path = Path(
            os.getenv(
                "CORRECTED_POSE_METADATA_PATH",
                "/tmp/condB_corrected_pose_metadata.json",
            )
        )

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

    def wait_for_start_service(self) -> None:
        self.get_logger().info(f"Waiting for service '{self.service_name}'...")
        if not self.client.wait_for_service(timeout_sec=10.0):
            raise RuntimeError(
                f"Service '{self.service_name}' is not available after waiting."
            )

    def fetch_corrected_pose(self) -> dict:
        url = f"{self.gateway_url}/robots/{self.robot_id}/compute_corrected_pose"
        req = urllib.request.Request(
            url=url,
            method="POST",
            data=b"{}",
            headers={"Content-Type": "application/json"},
        )

        request_wall_ns = time.time_ns()
        self.get_logger().info(f"Requesting corrected pose from {url}")
        try:
            with urllib.request.urlopen(req, timeout=5.0) as resp:
                payload = resp.read().decode("utf-8")
                pose = json.loads(payload)
        except urllib.error.HTTPError as exc:
            body = exc.read().decode("utf-8", errors="replace")
            raise RuntimeError(
                f"Gateway returned HTTP {exc.code}: {body}"
            ) from exc
        except urllib.error.URLError as exc:
            raise RuntimeError(f"Failed to call gateway: {exc}") from exc

        response_wall_ns = time.time_ns()
        pose["corrected_pose_request_wall_ns"] = request_wall_ns
        pose["corrected_pose_response_wall_ns"] = response_wall_ns
        return pose

    def write_metadata(self, pose: dict, call_wall_ns: int | None = None) -> None:
        metadata = {
            "robot_id": self.robot_id,
            "current_odom_stamp_ns": int(pose.get("current_odom_stamp_ns", 0) or 0),
            "px": float(pose["px"]),
            "py": float(pose["py"]),
            "pz": float(pose["pz"]),
            "qx": float(pose["qx"]),
            "qy": float(pose["qy"]),
            "qz": float(pose["qz"]),
            "qw": float(pose["qw"]),
            "relative_to_trajectory_id": int(pose["relative_to_trajectory_id"]),
            "corrected_pose_request_wall_ns": int(
                pose.get("corrected_pose_request_wall_ns", 0) or 0
            ),
            "corrected_pose_response_wall_ns": int(
                pose.get("corrected_pose_response_wall_ns", 0) or 0
            ),
            "start_trajectory_call_wall_ns": int(call_wall_ns or 0),
        }
        self.metadata_path.parent.mkdir(parents=True, exist_ok=True)
        tmp = self.metadata_path.with_name(self.metadata_path.name + ".tmp")
        tmp.write_text(json.dumps(metadata, sort_keys=True), encoding="utf-8")
        os.replace(tmp, self.metadata_path)

    def call_start_trajectory(self, pose_dict: dict):
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

        odom_stamp_ns = int(pose_dict.get("current_odom_stamp_ns", 0) or 0)
        self.get_logger().info(
            "Calling /start_trajectory with gateway corrected pose: "
            f"pos=({req.initial_pose.position.x:.6f}, "
            f"{req.initial_pose.position.y:.6f}, "
            f"{req.initial_pose.position.z:.6f}), "
            f"quat=({req.initial_pose.orientation.x:.6f}, "
            f"{req.initial_pose.orientation.y:.6f}, "
            f"{req.initial_pose.orientation.z:.6f}, "
            f"{req.initial_pose.orientation.w:.6f}), "
            f"relative_to_trajectory_id={req.relative_to_trajectory_id}, "
            f"corrected_pose_odom_stamp_ns={odom_stamp_ns}"
        )

        call_wall_ns = time.time_ns()
        self.write_metadata(pose_dict, call_wall_ns=call_wall_ns)
        print(f"metric corrected_pose_odom_stamp_ns={odom_stamp_ns}", flush=True)
        print(
            "metric corrected_pose_request_to_call_wall_ns="
            f"{call_wall_ns - int(pose_dict.get('corrected_pose_request_wall_ns', call_wall_ns))}",
            flush=True,
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
        # Wait first, then obtain the freshest possible corrected pose.
        node.wait_for_start_service()
        pose = node.fetch_corrected_pose()
        response = node.call_start_trajectory(pose)

        print("start_trajectory response:")
        print(f"  status.code: {response.status.code}")
        print(f"  status.message: {response.status.message}")
        print(f"  trajectory_id: {response.trajectory_id}")

        if response.status.code != 0:
            return 1
        return 0
    except Exception as exc:
        node.get_logger().error(str(exc))
        return 1
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    sys.exit(main())