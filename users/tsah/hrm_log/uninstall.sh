#!/usr/bin/env bash
# Stop the HRM recorder and delete it. Pass --keep-logs to leave ~/hrm-logs.
set -euo pipefail
LABEL="com.tsah.hrm-log"
launchctl bootout "gui/$(id -u)/$LABEL" 2>/dev/null || true
rm -f "$HOME/Library/LaunchAgents/$LABEL.plist"
rm -rf "$HOME/Library/Application Support/hrm-log"
if [[ "${1:-}" != "--keep-logs" ]]; then
    rm -rf "$HOME/hrm-logs"
    echo "Recorder removed and logs deleted."
else
    echo "Recorder removed; logs kept in $HOME/hrm-logs."
fi
