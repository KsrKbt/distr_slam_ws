#!/usr/bin/env bash
set -eo pipefail

WORKDIR="${WORKDIR:-/root/distr_slam_ws}"
PIDDIR="${PIDDIR:-$WORKDIR/run_pids}"

LAUNCH_PGID_FILE="$PIDDIR/cartographer_launch.pgid"
RVIZ_PGID_FILE="$PIDDIR/rviz2.pgid"
SERVER_PGID_FILE="$PIDDIR/server.pgid"

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

stop_group "$SERVER_PGID_FILE"
stop_group "$RVIZ_PGID_FILE"
stop_group "$LAUNCH_PGID_FILE"

echo "stack stopped"
