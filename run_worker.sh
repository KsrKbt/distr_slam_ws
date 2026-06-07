#!/usr/bin/env bash
set -euo pipefail

source /opt/ros/jazzy/setup.bash
source install/setup.bash

ros2 launch cartographer_ros my_robot_handover.launch.py > /tmp/cartographer.log 2>&1 &
python3 server_rpp_compatible.py > /tmp/server.log 2>&1 &

wait -n