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

stop_if_running() {
  local pid_file="$1"
  if [ -f "$pid_file" ]; then
    local pid
    pid="$(cat "$pid_file")"
    if kill -0 "$pid" 2>/dev/null; then
      kill "$pid" || true
      sleep 2
      if kill -0 "$pid" 2>/dev/null; then
        kill -9 "$pid" || true
      fi
    fi
    rm -f "$pid_file"
  fi
}

echo "[1/8] export state to redis"
ros2 service call /export_state_to_redis std_srvs/srv/Trigger "{}"

echo "[2/8] stop server, rviz2, cartographer launch"
stop_if_running "$SERVER_PID_FILE"
stop_if_running "$RVIZ_PID_FILE"
stop_if_running "$LAUNCH_PID_FILE"

sleep 2

echo "[3/8] restart cartographer launch"
nohup ros2 launch cartographer_ros my_robot_handover.launch.py use_sim_time:=true \
  > "$LAUNCH_LOG" 2>&1 &
echo $! > "$LAUNCH_PID_FILE"

echo "[4/8] wait services"
for i in $(seq 1 60); do
  if ros2 service list | grep -q "^/import_state_from_redis$" && \
     ros2 service list | grep -q "^/start_trajectory$"; then
    break
  fi
  sleep 1
done

if ! ros2 service list | grep -q "^/import_state_from_redis$"; then
  echo "ERROR: /import_state_from_redis not available"
  exit 1
fi

if ! ros2 service list | grep -q "^/start_trajectory$"; then
  echo "ERROR: /start_trajectory not available"
  exit 1
fi

echo "[5/8] restart rviz2"
nohup rviz2 > "$RVIZ_LOG" 2>&1 &
echo $! > "$RVIZ_PID_FILE"

echo "[6/8] import state"
ros2 service call /import_state_from_redis std_srvs/srv/Trigger "{}"

echo "[7/8] start server"
nohup python3 "$WORKDIR/server_rpp_compatible.py" > "$SERVER_LOG" 2>&1 &
echo $! > "$SERVER_PID_FILE"

sleep 2

echo "[8/8] start trajectory from corrected pose"
python3 "$WORKDIR/start_trajectory.py"

echo "handover restart completed"