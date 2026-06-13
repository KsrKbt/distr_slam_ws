#!/usr/bin/env bash
set -eo pipefail

WORKDIR="${WORKDIR:-/root/distr_slam_ws}"
PIDDIR="${PIDDIR:-$WORKDIR/run_pids}"

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

stop_if_running "$SERVER_PID_FILE"
stop_if_running "$RVIZ_PID_FILE"
stop_if_running "$LAUNCH_PID_FILE"

echo "stack stopped"