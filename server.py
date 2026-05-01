import rclpy
from rclpy.node import Node
from flask import Flask, request, jsonify
import threading
import time
from sensor_msgs.msg import LaserScan
from nav_msgs.msg import Odometry
from tf2_msgs.msg import TFMessage
from geometry_msgs.msg import TransformStamped, Vector3, Quaternion, Transform, Point
from std_msgs.msg import Header
from rclpy.qos import QoSProfile, ReliabilityPolicy, HistoryPolicy, DurabilityPolicy
from builtin_interfaces.msg import Time
import logging

app = Flask(__name__)
ros_node = None

# Werkzeugのデフォルトログを無効化
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
server_log = logging.getLogger("server")
server_log.setLevel(logging.INFO)

class ROS2PublisherNode(Node):
    def __init__(self):
        super().__init__('data_publisher_node')
        
        qos_profile = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.VOLATILE
        )
        qos_profile_tf_static = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            history=HistoryPolicy.KEEP_LAST,
            depth=10,
            durability=DurabilityPolicy.TRANSIENT_LOCAL
        )
        
        self.scan_pub = self.create_publisher(LaserScan, '/scan', qos_profile)
        self.odom_pub = self.create_publisher(Odometry, '/odom', qos_profile)
        self.tf_pub = self.create_publisher(TFMessage, '/tf', qos_profile)
        self.tf_static_pub = self.create_publisher(TFMessage, '/tf_static', qos_profile_tf_static)

    def dict_to_header(self, header_dict):
        return Header(
            stamp=Time(sec=header_dict['stamp']['sec'], nanosec=header_dict['stamp']['nanosec']),
            frame_id=header_dict['frame_id']
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
            rotation=self.dict_to_quaternion(transform_dict['rotation'])
        )

    def dict_to_transform_stamped(self, transform_stamped_dict):
        return TransformStamped(
            header=self.dict_to_header(transform_stamped_dict['header']),
            child_frame_id=transform_stamped_dict['child_frame_id'],
            transform=self.dict_to_transform(transform_stamped_dict['transform'])
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

    def publish_tf(self, tf_dict, is_static=False):
        msg = TFMessage(transforms=[
            self.dict_to_transform_stamped(t) for t in tf_dict['transforms']
        ])
        if is_static:
            self.tf_static_pub.publish(msg)
        else:
            self.tf_pub.publish(msg)

def spin_ros():
    while rclpy.ok():
        rclpy.spin_once(ros_node)
        time.sleep(0.001)

@app.route('/receive_data', methods=['POST'])
def receive_data():
    try:
        if request.content_type != 'application/json':
            return jsonify({"error": "Unsupported Media Type"}), 415
        
        data = request.get_json()
        if not data:
            return jsonify({"error": "No data received"}), 400

        if 'scan' in data:
            ros_node.publish_scan(data['scan'])
        if 'odom' in data:
            ros_node.publish_odom(data['odom'])
        if 'tf' in data:
            ros_node.publish_tf(data['tf'], is_static=False)
        if 'tf_static' in data:
            ros_node.publish_tf(data['tf_static'], is_static=True)

        return jsonify({"status": "success"}), 200
    except Exception as e:
        server_log.error(f"Error in receive_data: {e}")
        return jsonify({"error": str(e)}), 500

def main():
    global ros_node
    rclpy.init()
    ros_node = ROS2PublisherNode()

    ros_thread = threading.Thread(target=spin_ros, daemon=True)
    ros_thread.start()

    print("Starting server on port 80...")
    app.run(host='0.0.0.0', port=80, debug=False)

    ros_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()