# 空のtransformerがたまに混ざるバージョン
import os
import rclpy
from rclpy.node import Node
from flask import Flask, request, jsonify
import threading
import time
from sensor_msgs.msg import LaserScan, JointState
from nav_msgs.msg import Odometry
from tf2_msgs.msg import TFMessage
from geometry_msgs.msg import TransformStamped, Vector3, Quaternion, Transform, Point
from std_msgs.msg import Header
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy, DurabilityPolicy
from builtin_interfaces.msg import Time
import logging

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


def env_csv(name: str, default: str) -> list[str]:
    value = os.getenv(name, default)
    return [item.strip() for item in value.split(',') if item.strip()]


class ROS2PublisherNode(Node):
    def __init__(self):
        super().__init__('data_publisher_node')

        qos_profile = QoSProfile(
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

        self.scan_pub = self.create_publisher(LaserScan, '/scan', qos_profile)
        self.odom_pub = self.create_publisher(Odometry, '/odom', qos_profile)
        self.tf_pub = self.create_publisher(TFMessage, '/tf', qos_profile)
        self.tf_static_pub = self.create_publisher(TFMessage, '/tf_static', qos_profile_tf_static)
        self.joint_states_pub = self.create_publisher(JointState, '/joint_states', qos_profile)

        self.enable_input_control = env_bool('ENABLE_INPUT_CONTROL', False)
        self.input_enabled = env_bool('INPUT_ENABLED_AT_STARTUP', True)
        self.accept_external_tf_static = env_bool('ACCEPT_EXTERNAL_TF_STATIC', False)
        self.forward_joint_states = env_bool('FORWARD_JOINT_STATES', False)

        # TF filtering policy
        self.filter_tf = env_bool('FILTER_TF', True)
        self.allowed_tf_edges = set()
        for item in env_csv('ALLOWED_DYNAMIC_TF', 'odom>base_footprint,odom>base_link'):
            if '>' in item:
                parent, child = item.split('>', 1)
                self.allowed_tf_edges.add((parent.strip(), child.strip()))

        # Static edges should never come from external /tf when robot_state_publisher owns them.
        self.static_like_edges = {
            ('base_footprint', 'base_link'),
            ('base_link', 'base_scan'),
            ('base_link', 'imu_link'),
            ('base_link', 'caster_back'),
            ('base_link', 'caster_back_link'),
            ('base_link', 'wheel_left_link'),
            ('base_link', 'wheel_right_link'),
        }

        self.drop_tf_with_parent_frames = set(env_csv('DROP_TF_PARENT_FRAMES', 'map'))
        self.drop_tf_with_child_frames = set(env_csv('DROP_TF_CHILD_FRAMES', 'map'))

        self.get_logger().info(
            'enable_input_control=%s, input_enabled=%s, accept_external_tf_static=%s, '
            'filter_tf=%s, allowed_tf_edges=%s' % (
                self.enable_input_control,
                self.input_enabled,
                self.accept_external_tf_static,
                self.filter_tf,
                sorted(self.allowed_tf_edges),
            )
        )

    def dict_to_header(self, header_dict):
        return Header(
            stamp=Time(sec=header_dict['stamp']['sec'], nanosec=header_dict['stamp']['nanosec']),
            frame_id=header_dict['frame_id'],
        )

    def dict_to_vector3(self, vector_dict):
        return Vector3(x=vector_dict['x'], y=vector_dict['y'], z=vector_dict['z'])

    def dict_to_point(self, point_dict):
        return Point(x=point_dict['x'], y=point_dict['y'], z=point_dict['z'])

    def dict_to_quaternion(self, quat_dict):
        return Quaternion(x=quat_dict['x'], y=quat_dict['y'], z=quat_dict['z'], w=quat_dict['w'])

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

    def publish_scan(self, scan_dict):
        msg = LaserScan()
        msg.header = self.dict_to_header(scan_dict['header'])
        msg.angle_min = scan_dict['angle_min']
        msg.angle_max = scan_dict['angle_max']
        msg.angle_increment = scan_dict['angle_increment']
        msg.time_increment = scan_dict['time_increment']
        msg.scan_time = scan_dict['scan_time']
        msg.range_min = scan_dict['range_min']
        msg.range_max = scan_dict['range_max']
        msg.ranges = scan_dict['ranges']
        msg.intensities = scan_dict['intensities']
        self.scan_pub.publish(msg)

    def publish_odom(self, odom_dict):
        msg = Odometry()
        msg.header = self.dict_to_header(odom_dict['header'])
        msg.child_frame_id = odom_dict['child_frame_id']

        pose_dict = odom_dict['pose']['pose']
        msg.pose.pose.position = self.dict_to_point(pose_dict['position'])
        msg.pose.pose.orientation = self.dict_to_quaternion(pose_dict['orientation'])
        msg.pose.covariance = odom_dict['pose']['covariance']

        twist_dict = odom_dict['twist']['twist']
        msg.twist.twist.linear = self.dict_to_vector3(twist_dict['linear'])
        msg.twist.twist.angular = self.dict_to_vector3(twist_dict['angular'])
        msg.twist.covariance = odom_dict['twist']['covariance']

        self.odom_pub.publish(msg)

        # Rebuild dynamic TF from odometry:
        # header.frame_id -> child_frame_id
        tf = TransformStamped()
        tf.header = msg.header
        tf.child_frame_id = msg.child_frame_id
        tf.transform.translation.x = msg.pose.pose.position.x
        tf.transform.translation.y = msg.pose.pose.position.y
        tf.transform.translation.z = msg.pose.pose.position.z
        tf.transform.rotation = msg.pose.pose.orientation

        self.tf_pub.publish(TFMessage(transforms=[tf]))

    def _tf_allowed(self, transform_dict):
        parent = transform_dict['header']['frame_id']
        child = transform_dict['child_frame_id']
        edge = (parent, child)

        if parent in self.drop_tf_with_parent_frames or child in self.drop_tf_with_child_frames:
            return False, f'dropped map-related TF {parent}>{child}'

        if edge in self.static_like_edges:
            return False, f'dropped static-like TF {parent}>{child}'

        if self.filter_tf and edge not in self.allowed_tf_edges:
            return False, f'dropped non-whitelisted TF {parent}>{child}'

        return True, ''

    def publish_tf(self, tf_dict, is_static=False):
        if is_static:
            msg = TFMessage(transforms=[
                self.dict_to_transform_stamped(t) for t in tf_dict['transforms']
            ])
            self.tf_static_pub.publish(msg)
        else:
            # Ignore external /tf.
            server_log.info('Ignoring external tf because dynamic TF is rebuilt from /odom.')

    def publish_joint_states(self, joint_states_dict):
        names = list(joint_states_dict.get('name', []))
        positions = list(joint_states_dict.get('position', []))
        velocities = list(joint_states_dict.get('velocity', []))
        efforts = list(joint_states_dict.get('effort', []))

        # 空データは流さない
        if not names:
            server_log.info('Dropping empty joint_states.')
            return

        # 長さ不整合は危険なので落とす
        if positions and len(positions) != len(names):
            server_log.warning(
                f'Dropping joint_states because len(position)={len(positions)} != len(name)={len(names)}'
            )
            return
        if velocities and len(velocities) != len(names):
            server_log.warning(
                f'Dropping joint_states because len(velocity)={len(velocities)} != len(name)={len(names)}'
            )
            return
        if efforts and len(efforts) != len(names):
            server_log.warning(
                f'Dropping joint_states because len(effort)={len(efforts)} != len(name)={len(names)}'
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


@app.route('/healthz', methods=['GET'])
def healthz():
    return jsonify({
        'enable_input_control': ros_node.enable_input_control,
        'input_enabled': ros_node.input_enabled,
        'accept_external_tf_static': ros_node.accept_external_tf_static,
        'dynamic_tf_source': 'odometry',
        'forward_joint_states': ros_node.forward_joint_states,
    }), 200


@app.route('/enable_input', methods=['POST'])
def enable_input():
    ros_node.input_enabled = True
    return jsonify({'status': 'enabled'}), 200


@app.route('/disable_input', methods=['POST'])
def disable_input():
    ros_node.input_enabled = False
    return jsonify({'status': 'disabled'}), 200


@app.route('/receive_data', methods=['POST'])
def receive_data():
    try:
        if ros_node.enable_input_control and not ros_node.input_enabled:
            return jsonify({'error': 'receiver disabled'}), 503

        if not request.is_json:
            return jsonify({'error': 'Unsupported Media Type'}), 415

        data = request.get_json(silent=True)
        if not data:
            return jsonify({'error': 'No data received'}), 400

        if 'scan' in data:
            ros_node.publish_scan(data['scan'])
        if 'odom' in data:
            ros_node.publish_odom(data['odom'])
        if 'joint_states' in data:
            if ros_node.forward_joint_states:
                ros_node.publish_joint_states(data['joint_states'])
            else:
                server_log.info('Ignoring joint_states because FORWARD_JOINT_STATES is false.')
        if 'tf' in data:
            server_log.info('Ignoring external tf because dynamic TF is rebuilt from /odom.')
        if 'tf_static' in data:
            if ros_node.accept_external_tf_static:
                ros_node.publish_tf(data['tf_static'], is_static=True)
            else:
                server_log.info('Ignoring tf_static because robot_state_publisher owns static TF.')

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
    app.run(host='0.0.0.0', port=port, debug=False)

    ros_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
