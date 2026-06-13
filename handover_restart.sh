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

spawn_group() {
  local pgid_file="$1"
  local log_file="$2"
  shift 2
  setsid bash -lc "$*" > "$log_file" 2>&1 &
  local leader_pid=$!
  echo "$leader_pid" > "$pgid_file"
}

stop_group() {
  local pgid_file="$1"
  if [ -f "$pgid_file" ]; then
    local pgid
    pgid="$(cat "$pgid_file")"
    if kill -0 "$pgid" 2>/dev/null; then
      kill -TERM -- "-$pgid" 2>/dev/null || true
      sleep 3
      if kill -0 "$pgid" 2>/dev/null; then
        kill -KILL -- "-$pgid" 2>/dev/null || true
      fi
    fi
    rm -f "$pgid_file"
  fi
}

echo "[1/8] export state"
ros2 service call /export_state_to_redis std_srvs/srv/Trigger "{}"

echo "[2/8] stop server, rviz2, cartographer"
stop_group "$SERVER_PGID_FILE"
stop_group "$RVIZ_PGID_FILE"
stop_group "$LAUNCH_PGID_FILE"

sleep 2

echo "[3/8] restart cartographer"
spawn_group "$LAUNCH_PGID_FILE" "$LAUNCH_LOG" \
  "source /opt/ros/jazzy/setup.bash && source \"$WORKDIR/install/setup.bash\" && ros2 launch cartographer_ros my_robot_handover.launch.py use_sim_time:=true"

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
spawn_group "$RVIZ_PGID_FILE" "$RVIZ_LOG" \
  "source /opt/ros/jazzy/setup.bash && source \"$WORKDIR/install/setup.bash\" && rviz2"

echo "[6/8] import state"
ros2 service call /import_state_from_redis std_srvs/srv/Trigger "{}"

echo "[7/8] start server"
spawn_group "$SERVER_PGID_FILE" "$SERVER_LOG" \
  "source /opt/ros/jazzy/setup.bash && source \"$WORKDIR/install/setup.bash\" && python3 \"$WORKDIR/server_rpp_compatible.py\""

sleep 2

echo "[8/8] start trajectory from corrected pose"
python3 "$WORKDIR/start_trajectory.py"

echo "handover restart completed"
