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

LAUNCH_PGID_FILE="$PIDDIR/cartographer_launch.pgid"
RVIZ_PGID_FILE="$PIDDIR/rviz2.pgid"
SERVER_PGID_FILE="$PIDDIR/server.pgid"

CONFIG_DIR="$WORKDIR/install/cartographer_ros/share/cartographer_ros/configuration_files"

spawn_group() {
  local pgid_file="$1"
  local log_file="$2"
  shift 2
  setsid bash -lc "$*" > "$log_file" 2>&1 &
  local leader_pid=$!
  echo "$leader_pid" > "$pgid_file"
}

echo "[1/5] start cartographer launch"
spawn_group "$LAUNCH_PGID_FILE" "$LAUNCH_LOG" \
  "source /opt/ros/jazzy/setup.bash && source \"$WORKDIR/install/setup.bash\" && ros2 launch cartographer_ros my_robot_handover.launch.py use_sim_time:=true"

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
spawn_group "$RVIZ_PGID_FILE" "$RVIZ_LOG" \
  "source /opt/ros/jazzy/setup.bash && source \"$WORKDIR/install/setup.bash\" && rviz2"

echo "[4/5] initial start_trajectory"
ros2 service call /start_trajectory cartographer_ros_msgs/srv/StartTrajectory \
"{configuration_directory: '$CONFIG_DIR', configuration_basename: 'my_robot.lua', use_initial_pose: false, initial_pose: {position: {x: 0.0, y: 0.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, relative_to_trajectory_id: 0}"

echo "[5/5] start server"
spawn_group "$SERVER_PGID_FILE" "$SERVER_LOG" \
  "source /opt/ros/jazzy/setup.bash && source \"$WORKDIR/install/setup.bash\" && python3 \"$WORKDIR/server_rpp_compatible.py\""

echo "stack started"
