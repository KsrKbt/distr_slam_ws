#!/usr/bin/env bash
set -euo pipefail

#source /opt/ros/jazzy/setup.bash
#source install/setup.bash

CONFIG_DIR="$(ros2 pkg prefix cartographer_ros)/share/cartographer_ros/configuration_files"

until ros2 service list | grep -q "/start_trajectory"; do
  sleep 1
done

ros2 service call /start_trajectory cartographer_ros_msgs/srv/StartTrajectory \
"{configuration_directory: '$CONFIG_DIR', configuration_basename: 'my_robot.lua', use_initial_pose: false, initial_pose: {position: {x: 0.0, y: 0.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, relative_to_trajectory_id: 0}"

#curl -s -X POST http://127.0.0.1:80/enable_input