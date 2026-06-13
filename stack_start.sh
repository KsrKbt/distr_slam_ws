#!/usr/bin/env bash
set -eo pipefail

WORKDIR="${WORKDIR:-/root/distr_slam_ws}"
LOGDIR="${LOGDIR:-$WORKDIR/run_logs}"
PIDDIR="${PIDDIR:-$WORKDIR/run_pids}"

mkdir -p "$LOGDIR" "$PIDDIR"

source /opt/ros/jazzy/setup.bash
source "$WORKDIR/install/setup.bash"

LAUNCH_LOG="$LOGDIR/cartographer_launch.log"
RVIZ_LOG="$LOGDIR/rviz2.log"
SERVER_LOG="$LOGDIR/server.log"

LAUNCH_PID_FILE="$PIDDIR/cartographer_launch.pid"
RVIZ_PID_FILE="$PIDDIR/rviz2.pid"
SERVER_PID_FILE="$PIDDIR/server.pid"

CONFIG_DIR="$WORKDIR/install/cartographer_ros/share/cartographer_ros/configuration_files"

echo "[1/5] start cartographer launch"
nohup ros2 launch cartographer_ros my_robot_handover.launch.py use_sim_time:=true \
  > "$LAUNCH_LOG" 2>&1 &
echo $! > "$LAUNCH_PID_FILE"

echo "[2/5] wait /start_trajectory service"
for i in $(seq 1 60); do
  if ros2 service list | grep -q "^/start_trajectory$"; then
    break
  fi
  sleep 1
done

if ! ros2 service list | grep -q "^/start_trajectory$"; then
  echo "ERROR: /start_trajectory not available"
  exit 1
fi

echo "[3/5] start rviz2"
nohup rviz2 > "$RVIZ_LOG" 2>&1 &
echo $! > "$RVIZ_PID_FILE"

echo "[4/5] initial start_trajectory"
ros2 service call /start_trajectory cartographer_ros_msgs/srv/StartTrajectory \
"{configuration_directory: '$CONFIG_DIR', configuration_basename: 'my_robot.lua', use_initial_pose: false, initial_pose: {position: {x: 0.0, y: 0.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, relative_to_trajectory_id: 0}"

echo "[5/5] start server"
nohup python3 "$WORKDIR/server_rpp_compatible.py" > "$SERVER_LOG" 2>&1 &
echo $! > "$SERVER_PID_FILE"

echo "stack started"