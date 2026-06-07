#!/usr/bin/env bash
set -euo pipefail

source /opt/ros/jazzy/setup.bash
source install/setup.bash

ros2 service call /export_state_to_redis std_srvs/srv/Trigger "{}"