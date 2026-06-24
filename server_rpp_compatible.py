#!/usr/bin/env python3
import os
import threading
import time
import logging

import rclpy
from rclpy.node import Node
from flask import Flask, request, jsonify

from sensor_msgs.msg import LaserScan, JointState
from nav_msgs.msg import Odometry
from tf2_msgs.msg import TFMessage
from geometry_msgs.msg import TransformStamped, Vector3, Quaternion, Transform, Point
from std_msgs.msg import Header
from rclpy.qos import (
    QoSProfile,
    ReliabilityPolicy,
    HistoryPolicy,
    DurabilityPolicy,
    qos_profile_sensor_data,
)
from builtin_interfaces.msg import Time

app = Flask(__name__)
ros_node = None

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
server_log = logging.getLogger('server')
server_log.setLevel(logging.INFO)


def env_bool(name: str, default: bool) -> bool:
    value = os.getenv(name)
    if value is None:
        return default
    return value.strip().lower() in ('1', 'true', 'yes', 'on')


class ROS2PublisherNode(Node):
    def __init__(self):
        super().__init__('data_publisher_node')

        qos_profile_tf = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.VOLATILE,
        )
        qos_profile_tf_static = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
        )
        joint_qos = QoSProfile(
            reliability=ReliabilityPolicy.BEST_EFFORT,
            history=HistoryPolicy.KEEP_LAST,
            depth=20,
            durability=DurabilityPolicy.VOLATILE,
        )

        self.scan_pub = self.create_publisher(LaserScan, '/scan', qos_profile_sensor_data)
        self.odom_pub = self.create_publisher(Odometry, '/odom', qos_profile_sensor_data)
        self.tf_pub = self.create_publisher(TFMessage, '/tf', qos_profile_tf)
        self.tf_static_pub = self.create_publisher(TFMessage, '/tf_static', qos_profile_tf_static)
        self.joint_states_pub = self.create_publisher(JointState, '/joint_states', joint_qos)

        self.accept_external_tf_static = env_bool('ACCEPT_EXTERNAL_TF_STATIC', False)
        self.forward_joint_states = env_bool('FORWARD_JOINT_STATES', False)

        self.get_logger().info(
            f"accept_external_tf_static={self.accept_external_tf_static}, "
            f"forward_joint_states={self.forward_joint_states}"
        )

    def dict_to_header(self, header_dict):
        return Header(
            stamp=Time(sec=int(header_dict['stamp']['sec']), nanosec=int(header_dict['stamp']['nanosec'])),
            frame_id=header_dict['frame_id'],
        )

    def dict_to_vector3(self, vector_dict):
        return Vector3(
            x=float(vector_dict['x']),
            y=float(vector_dict['y']),
            z=float(vector_dict['z']),
        )

    def dict_to_point(self, point_dict):
        return Point(
            x=float(point_dict['x']),
            y=float(point_dict['y']),
            z=float(point_dict['z']),
        )

    def dict_to_quaternion(self, quat_dict):
        return Quaternion(
            x=float(quat_dict['x']),
            y=float(quat_dict['y']),
            z=float(quat_dict['z']),
            w=float(quat_dict['w']),
        )

    def dict_to_transform(self, transform_dict):
        return Transform(
            translation=self.dict_to_vector3(transform_dict['translation']),
            rotation=self.dict_to_quaternion(transform_dict['rotation']),
        )

    def dict_to_transform_stamped(self, transform_stamped_dict):
        return TransformStamped(
            header=self.dict_to_header(transform_stamped_dict['header']),
            child_frame_id=transform_stamped_dict['child_frame_id'],
            transform=self.dict_to_transform(transform_stamped_dict['transform']),
        )

    def publish_tf_message(self, transforms):
        if not transforms:
            return
        self.tf_pub.publish(TFMessage(transforms=transforms))

    def publish_scan(self, scan_dict):
        msg = LaserScan()
        msg.header = self.dict_to_header(scan_dict['header'])
        msg.angle_min = float(scan_dict['angle_min'])
        msg.angle_max = float(scan_dict['angle_max'])
        msg.angle_increment = float(scan_dict['angle_increment'])
        msg.time_increment = float(scan_dict['time_increment'])
        msg.scan_time = float(scan_dict['scan_time'])
        msg.range_min = float(scan_dict['range_min'])
        msg.range_max = float(scan_dict['range_max'])
        msg.ranges = [float(x) for x in scan_dict['ranges']]
        msg.intensities = [float(x) for x in scan_dict['intensities']]
        self.scan_pub.publish(msg)

    def publish_odom(self, odom_dict):
        msg = Odometry()
        msg.header = self.dict_to_header(odom_dict['header'])
        msg.child_frame_id = odom_dict['child_frame_id']

        pose_dict = odom_dict['pose']['pose']
        msg.pose.pose.position = self.dict_to_point(pose_dict['position'])
        msg.pose.pose.orientation = self.dict_to_quaternion(pose_dict['orientation'])
        msg.pose.covariance = [float(x) for x in odom_dict['pose']['covariance']]

        twist_dict = odom_dict['twist']['twist']
        msg.twist.twist.linear = self.dict_to_vector3(twist_dict['linear'])
        msg.twist.twist.angular = self.dict_to_vector3(twist_dict['angular'])
        msg.twist.covariance = [float(x) for x in odom_dict['twist']['covariance']]

        self.odom_pub.publish(msg)

        tf = TransformStamped()
        tf.header = msg.header
        tf.child_frame_id = msg.child_frame_id
        tf.transform.translation.x = float(msg.pose.pose.position.x)
        tf.transform.translation.y = float(msg.pose.pose.position.y)
        tf.transform.translation.z = float(msg.pose.pose.position.z)
        tf.transform.rotation = msg.pose.pose.orientation
        self.publish_tf_message([tf])

    def publish_tf_static(self, tf_dict):
        transforms = [self.dict_to_transform_stamped(t) for t in tf_dict['transforms']]
        if transforms:
            self.tf_static_pub.publish(TFMessage(transforms=transforms))

    def publish_joint_states(self, joint_states_dict):
        names = list(joint_states_dict.get('name', []))
        positions = [float(x) for x in joint_states_dict.get('position', [])]
        velocities = [float(x) for x in joint_states_dict.get('velocity', [])]
        efforts = [float(x) for x in joint_states_dict.get('effort', [])]

        if not names:
            server_log.info('Dropping empty joint_states.')
            return
        if positions and len(positions) != len(names):
            server_log.warning(
                f"Dropping joint_states because len(position)={len(positions)} != len(name)={len(names)}"
            )
            return
        if velocities and len(velocities) != len(names):
            server_log.warning(
                f"Dropping joint_states because len(velocity)={len(velocities)} != len(name)={len(names)}"
            )
            return
        if efforts and len(efforts) != len(names):
            server_log.warning(
                f"Dropping joint_states because len(effort)={len(efforts)} != len(name)={len(names)}"
            )
            return

        msg = JointState()
        msg.header = self.dict_to_header(joint_states_dict['header'])
        msg.name = names
        msg.position = positions
        msg.velocity = velocities
        msg.effort = efforts
        self.joint_states_pub.publish(msg)


def spin_ros():
    while rclpy.ok():
        rclpy.spin_once(ros_node)
        time.sleep(0.001)


def process_message(msg):
    topic = msg['topic']
    payload = msg['payload']

    if topic == 'scan':
        server_log.info('received scan')
        ros_node.publish_scan(payload)
    elif topic == 'odom':
        server_log.info('received odom')
        ros_node.publish_odom(payload)
    elif topic == 'joint_states':
        if ros_node.forward_joint_states:
            server_log.info('received joint_states')
            ros_node.publish_joint_states(payload)
    elif topic == 'tf':
        server_log.info('Ignoring external tf because dynamic TF is rebuilt from /odom.')
    elif topic == 'tf_static':
        if ros_node.accept_external_tf_static:
            ros_node.publish_tf_static(payload)
        else:
            server_log.info('Ignoring tf_static because robot_state_publisher owns static TF.')


@app.route('/healthz', methods=['GET'])
def healthz():
    return jsonify({
        'accept_external_tf_static': ros_node.accept_external_tf_static,
        'forward_joint_states': ros_node.forward_joint_states,
        'dynamic_tf_source': 'odometry',
    }), 200


@app.route('/receive_data', methods=['POST'])
def receive_data():
    try:
        if not request.is_json:
            return jsonify({'error': 'Unsupported Media Type'}), 415

        data = request.get_json(silent=True)
        if not data:
            return jsonify({'error': 'No data received'}), 400

        if 'messages' in data:
            for msg in data['messages']:
                process_message(msg)
        else:
            if 'scan' in data:
                process_message({'topic': 'scan', 'payload': data['scan']})
            if 'odom' in data:
                process_message({'topic': 'odom', 'payload': data['odom']})
            if 'joint_states' in data:
                process_message({'topic': 'joint_states', 'payload': data['joint_states']})
            if 'tf' in data:
                process_message({'topic': 'tf', 'payload': data['tf']})
            if 'tf_static' in data:
                process_message({'topic': 'tf_static', 'payload': data['tf_static']})

        return jsonify({'status': 'success'}), 200
    except Exception as e:
        server_log.error(f'Error in receive_data: {e}')
        return jsonify({'error': str(e)}), 500


def main():
    global ros_node
    rclpy.init()
    ros_node = ROS2PublisherNode()

    ros_thread = threading.Thread(target=spin_ros, daemon=True)
    ros_thread.start()

    port = int(os.getenv('SERVER_PORT', '80'))
    print(f'Starting server on port {port}...')
    app.run(host='0.0.0.0', port=port, debug=False, threaded=True)

    ros_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
