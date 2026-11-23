#!/usr/bin/env bash
# Install the HRM recorder as a macOS LaunchAgent.
# Usage: install.sh "<record until>" "<purge at>"   (dates as "YYYY-MM-DD HH:MM", local time)
set -euo pipefail

[[ "$(uname -s)" == Darwin ]] || { echo "macOS only" >&2; exit 1; }
[[ $# -eq 2 ]] || { sed -n '2,3p' "$0"; exit 2; }

LABEL="com.tsah.hrm-log"
LOG_DIR="$HOME/hrm-logs"
INSTALL_DIR="$HOME/Library/Application Support/hrm-log"
PLIST="$HOME/Library/LaunchAgents/$LABEL.plist"
QMK="$(command -v qmk)"
PYTHON="$(head -1 "$QMK" | sed 's/^#!//')"   # the python qmk itself runs on
RECORD_UNTIL="$(date -j -f '%Y-%m-%d %H:%M' "$1" +%s)"
PURGE_AT="$(date -j -f '%Y-%m-%d %H:%M' "$2" +%s)"

mkdir -p "$INSTALL_DIR" "$LOG_DIR" "$(dirname "$PLIST")"
cp "$(dirname "$0")/logger.py" "$INSTALL_DIR/logger.py"

cat > "$PLIST" <<PLIST
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key><string>$LABEL</string>
    <key>ProgramArguments</key>
    <array>
        <string>$PYTHON</string>
        <string>$INSTALL_DIR/logger.py</string>
        <string>--qmk</string><string>$QMK</string>
        <string>--log-dir</string><string>$LOG_DIR</string>
        <string>--install-dir</string><string>$INSTALL_DIR</string>
        <string>--plist</string><string>$PLIST</string>
        <string>--label</string><string>$LABEL</string>
        <string>--record-until</string><string>$RECORD_UNTIL</string>
        <string>--purge-at</string><string>$PURGE_AT</string>
    </array>
    <key>EnvironmentVariables</key>
    <dict>
        <key>PATH</key><string>$(dirname "$QMK"):/usr/bin:/bin:/usr/sbin:/sbin:/opt/homebrew/bin</string>
        <key>HOME</key><string>$HOME</string>
    </dict>
    <key>RunAtLoad</key><true/>
    <key>KeepAlive</key><true/>
    <key>ThrottleInterval</key><integer>30</integer>
    <key>StandardErrorPath</key><string>$INSTALL_DIR/logger.err</string>
</dict>
</plist>
PLIST

launchctl bootout "gui/$(id -u)/$LABEL" 2>/dev/null || true
launchctl bootstrap "gui/$(id -u)" "$PLIST"
echo "Installed $LABEL: recording until $1, auto-delete at $2. Logs: $LOG_DIR"
