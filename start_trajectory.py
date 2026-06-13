#!/usr/bin/env python3
import json
import os
import socket
import sys
from dataclasses import dataclass

import rclpy
from rclpy.node import Node
from cartographer_ros_msgs.srv import StartTrajectory


class RedisProtocolError(RuntimeError):
    pass


class SimpleRedisClient:
    def __init__(self, host: str, port: int, timeout: float = 5.0) -> None:
        self.host = host
        self.port = port
        self.timeout = timeout

    def _encode_command(self, *parts: str) -> bytes:
        out = [f"*{len(parts)}\r\n".encode("utf-8")]
        for part in parts:
            data = part.encode("utf-8")
            out.append(f"${len(data)}\r\n".encode("utf-8"))
            out.append(data + b"\r\n")
        return b"".join(out)

    def _read_line(self, sock_file) -> bytes:
        line = sock_file.readline()
        if not line:
            raise RedisProtocolError("Connection closed by Redis server.")
        if not line.endswith(b"\r\n"):
            raise RedisProtocolError(f"Malformed Redis line: {line!r}")
        return line[:-2]

    def get(self, key: str):
        with socket.create_connection((self.host, self.port), timeout=self.timeout) as sock:
            sock.sendall(self._encode_command("GET", key))
            sock_file = sock.makefile("rb")
            prefix = sock_file.read(1)
            if not prefix:
                raise RedisProtocolError("Empty response from Redis.")

            if prefix == b"$":
                length = int(self._read_line(sock_file))
                if length == -1:
                    return None
                data = sock_file.read(length)
                trailer = sock_file.read(2)
                if trailer != b"\r\n":
                    raise RedisProtocolError("Malformed bulk string terminator.")
                return data.decode("utf-8")
            if prefix == b"-":
                message = self._read_line(sock_file).decode("utf-8", errors="replace")
                raise RedisProtocolError(f"Redis error: {message}")
            if prefix == b"+":
                return self._read_line(sock_file).decode("utf-8", errors="replace")

            raise RedisProtocolError(f"Unsupported Redis response prefix: {prefix!r}")


@dataclass
class SavedPose:
    px: float
    py: float
    pz: float
    qx: float
    qy: float
    qz: float
    qw: float
    relative_to_trajectory_id: int


class StartTrajectoryFromSavedPose(Node):
    def __init__(self) -> None:
        super().__init__('start_trajectory_from_saved_pose')

        self.redis_host = os.getenv('REDIS_HOST', 'redis_host')
        self.redis_port = int(os.getenv('REDIS_PORT', '6379'))
        self.redis_key = os.getenv('REDIS_POSE_KEY', 'robot1_pose')
        self.service_name = os.getenv('START_TRAJECTORY_SERVICE', '/start_trajectory')

        default_config_dir = (
            '/root/distr_slam_ws/install/cartographer_ros/'
            'share/cartographer_ros/configuration_files'
        )
        self.configuration_directory = os.getenv(
            'CARTOGRAPHER_CONFIG_DIR', default_config_dir
        )
        self.configuration_basename = os.getenv(
            'CARTOGRAPHER_CONFIG_BASENAME', 'my_robot.lua'
        )

        self.client = self.create_client(StartTrajectory, self.service_name)

    def load_saved_pose(self) -> SavedPose:
        self.get_logger().info(
            f"Loading saved pose from Redis host={self.redis_host} "
            f"port={self.redis_port} key={self.redis_key}"
        )
        r = SimpleRedisClient(
            host=self.redis_host,
            port=self.redis_port,
            timeout=5.0,
        )
        raw = r.get(self.redis_key)
        if raw is None:
            raise RuntimeError(f"No saved pose found in Redis key '{self.redis_key}'.")

        try:
            data = json.loads(raw)
        except json.JSONDecodeError as e:
            raise RuntimeError(
                f"Saved pose in Redis key '{self.redis_key}' is not valid JSON: {e}"
            ) from e

        required = [
            'px', 'py', 'pz', 'qx', 'qy', 'qz', 'qw', 'relative_to_trajectory_id'
        ]
        missing = [k for k in required if k not in data]
        if missing:
            raise RuntimeError(
                f"Saved pose JSON is missing required keys: {', '.join(missing)}"
            )

        return SavedPose(
            px=float(data['px']),
            py=float(data['py']),
            pz=float(data['pz']),
            qx=float(data['qx']),
            qy=float(data['qy']),
            qz=float(data['qz']),
            qw=float(data['qw']),
            relative_to_trajectory_id=int(data['relative_to_trajectory_id']),
        )

    def call_start_trajectory(self, saved_pose: SavedPose) -> StartTrajectory.Response:
        self.get_logger().info(
            f"Waiting for service '{self.service_name}'..."
        )
        if not self.client.wait_for_service(timeout_sec=10.0):
            raise RuntimeError(
                f"Service '{self.service_name}' is not available after waiting."
            )

        req = StartTrajectory.Request()
        req.configuration_directory = self.configuration_directory
        req.configuration_basename = self.configuration_basename
        req.use_initial_pose = True
        req.initial_pose.position.x = saved_pose.px
        req.initial_pose.position.y = saved_pose.py
        req.initial_pose.position.z = saved_pose.pz
        req.initial_pose.orientation.x = saved_pose.qx
        req.initial_pose.orientation.y = saved_pose.qy
        req.initial_pose.orientation.z = saved_pose.qz
        req.initial_pose.orientation.w = saved_pose.qw
        req.relative_to_trajectory_id = saved_pose.relative_to_trajectory_id

        self.get_logger().info(
            "Calling /start_trajectory with saved pose: "
            f"pos=({saved_pose.px:.6f}, {saved_pose.py:.6f}, {saved_pose.pz:.6f}), "
            f"quat=({saved_pose.qx:.6f}, {saved_pose.qy:.6f}, "
            f"{saved_pose.qz:.6f}, {saved_pose.qw:.6f}), "
            f"relative_to_trajectory_id={saved_pose.relative_to_trajectory_id}"
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
    node = StartTrajectoryFromSavedPose()
    try:
        saved_pose = node.load_saved_pose()
        response = node.call_start_trajectory(saved_pose)

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


if __name__ == '__main__':
    sys.exit(main())
