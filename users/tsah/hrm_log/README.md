# Home row mod recorder

Records home row mod decisions on the keyboard for a few days, to find out why
intended mods come out as letters (or the reverse), then tunes the settings.

**Privacy:** normal typing is logged as *hand + timing only*, never letters.
Keycodes are logged only while Ctrl/Alt/Cmd is held (shortcuts) and for
Backspace/Delete/Space/Enter/Tab/Esc. Only the QMK keyboard is recorded (it is
read through its QMK debug console), never the laptop keyboard.

1. Flash the recording firmware (normal builds are unaffected):
   `qmk compile -kb keebart/3w6_pro -km tsah -e TSAH_HRM_LOG=yes`, then flash.
2. Start the macOS recorder (a LaunchAgent; survives logout/reboot):
   `users/tsah/hrm_log/install.sh "2026-11-25 18:00" "2026-11-30 12:00"`
   It stops recording at the first time and, as a fail-safe, deletes the logs
   and removes itself at the second.
3. Analyse: `users/tsah/hrm_log/analyze.py ~/hrm-logs/*.log`
4. Clean up: `users/tsah/hrm_log/uninstall.sh` (deletes logs too), then flash a
   normal build.

Log line format: see `users/tsah/hrm_log.c`.
