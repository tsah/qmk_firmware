#!/usr/bin/env bash
# Compile a tsah keymap and flash it to both halves of an RP2040 split keyboard
# by copying the UF2 onto the RPI-RP2 boot drive. Works on macOS and Linux.
#
# Usage: users/tsah/flash.sh <keyboard> [left|right|both] [--no-compile]
#   e.g. users/tsah/flash.sh keebart/3w6_pro
#        users/tsah/flash.sh crkbd/rev4_1/standard right --no-compile
#
# Alternative without this script: `qmk flash -kb <keyboard> -km tsah`
# (waits for the boot drive and copies the UF2; run it once per half).
set -euo pipefail

usage() {
    sed -n '2,10p' "$0" | sed 's/^# \{0,1\}//'
    exit 2
}

[[ $# -ge 1 ]] || usage
KB="$1"
shift
SIDES="both"
COMPILE=1
for arg in "$@"; do
    case "$arg" in
        left | right | both) SIDES="$arg" ;;
        --no-compile) COMPILE=0 ;;
        *) usage ;;
    esac
done

KM="tsah"
QMK_DIR="$(git -C "$(dirname "$0")" rev-parse --show-toplevel)"
UF2_PATH="$QMK_DIR/${KB//\//_}_${KM}.uf2"
DRIVE_LABEL="RPI-RP2"
OS="$(uname -s)"

case "$KB" in
    keebart/3w6_pro)
        BOOT_HINT="unplug it, hold its outer top-row key (Q on left / P on right), plug USB back in;
  or hold BOOT, tap RESET, release BOOT (two pinholes on the underside)."
        DONE_HINT="Connect the halves with the USB-C cable, plug USB into either half, and test."
        ;;
    crkbd/*)
        BOOT_HINT="unplug it, hold Q (left) / P (right): the top-row key SECOND from the outside, plug USB back in;
  or double-tap its reset button."
        DONE_HINT="Connect the TRRS cable, plug USB into the left half, and test."
        ;;
    *)
        BOOT_HINT="use its bootloader key, reset button, or bootmagic key."
        DONE_HINT="Reconnect the halves and test."
        ;;
esac

die() {
    echo "ERROR: $*" >&2
    exit 1
}

compile() {
    echo "Compiling $KB:$KM..."
    (cd "$QMK_DIR" && qmk compile -kb "$KB" -km "$KM")
    [[ -f "$UF2_PATH" ]] || die "UF2 not found at $UF2_PATH"
    echo "Compiled: $UF2_PATH"
}

# --- macOS: the boot drive auto-mounts at /Volumes/RPI-RP2 -----------------

mac_wait_for_drive() {
    local mount_point="/Volumes/$DRIVE_LABEL"
    until [[ -f "$mount_point/INFO_UF2.TXT" ]]; do
        sleep 0.25
    done
    printf '%s\n' "$mount_point"
}

mac_copy() {
    local mount_point="$1"
    # -X: don't copy extended attributes; some macOS versions (notably
    # Sonoma 14.0) fail to write to the RP2040 boot drive otherwise.
    if ! cp -X "$UF2_PATH" "$mount_point/"; then
        # The RP2040 reboots as soon as it has the last block, which can make
        # cp report an I/O error after a successful write.
        sleep 1
        if [[ -d "$mount_point" ]]; then
            die "copy to $mount_point failed and the drive is still mounted"
        fi
        echo "(cp reported an error but the drive ejected itself: the flash went through)"
    fi
}

mac_wait_for_disconnect() {
    local mount_point="$1"
    while [[ -d "$mount_point" ]]; do
        sleep 0.25
    done
}

# --- Linux: find the partition with lsblk, mount with udisksctl if needed ---

linux_wait_for_drive() {
    local part
    while true; do
        part="$(lsblk -prno PATH,LABEL,TYPE | awk -v label="$DRIVE_LABEL" '$2 == label && $3 == "part" { print $1; exit }')"
        if [[ -n "$part" ]]; then
            printf '%s\n' "$part"
            return 0
        fi
        sleep 0.25
    done
}

linux_find_mount_point() {
    local part="$1"
    local mount_point

    mount_point="$(lsblk -prno MOUNTPOINT "$part" | awk 'NF { print; exit }')"

    if [[ -z "$mount_point" ]] && command -v udisksctl >/dev/null 2>&1; then
        udisksctl mount -b "$part" --no-user-interaction >/dev/null 2>&1 || true
        sleep 0.5
        mount_point="$(lsblk -prno MOUNTPOINT "$part" | awk 'NF { print; exit }')"
    fi

    if [[ -n "$mount_point" ]] && ! mountpoint -q "$mount_point"; then
        mount_point=""
    fi

    if [[ -z "$mount_point" ]]; then
        local path
        for path in "/run/media/$USER/$DRIVE_LABEL" "/media/$USER/$DRIVE_LABEL"; do
            if [[ -d "$path" ]] && mountpoint -q "$path"; then
                mount_point="$path"
                break
            fi
        done
    fi

    [[ -n "$mount_point" ]] || die "could not find mounted $DRIVE_LABEL partition. Try manually: cp \"$UF2_PATH\" /path/to/$DRIVE_LABEL/"
    printf '%s\n' "$mount_point"
}

linux_wait_for_disconnect() {
    local part="$1"
    while lsblk -prno PATH "$part" >/dev/null 2>&1; do
        sleep 0.25
    done
}

# ---------------------------------------------------------------------------

wait_and_flash() {
    local side="$1"
    local part mount_point

    echo ""
    echo "=== Flash $side half ==="
    echo "Put the $side half in boot mode now:"
    echo "  $BOOT_HINT"
    echo "Waiting for $DRIVE_LABEL..."

    case "$OS" in
        Darwin)
            mount_point="$(mac_wait_for_drive)"
            echo "Detected: $mount_point"
            mac_copy "$mount_point"
            echo "$side half flashed!"
            echo "Waiting for device to reboot..."
            mac_wait_for_disconnect "$mount_point"
            ;;
        Linux)
            part="$(linux_wait_for_drive)"
            echo "Detected: $part"
            mount_point="$(linux_find_mount_point "$part")"
            cp "$UF2_PATH" "$mount_point/"
            sync
            echo "$side half flashed!"
            echo "Waiting for device to reboot..."
            linux_wait_for_disconnect "$part"
            ;;
        *)
            die "unsupported OS '$OS'; use: qmk flash -kb $KB -km $KM"
            ;;
    esac
}

if [[ $COMPILE -eq 1 ]]; then
    compile
else
    [[ -f "$UF2_PATH" ]] || die "UF2 not found at $UF2_PATH (drop --no-compile)"
fi

case "$SIDES" in
    left) wait_and_flash "LEFT" ;;
    right) wait_and_flash "RIGHT" ;;
    both)
        wait_and_flash "LEFT"
        wait_and_flash "RIGHT"
        ;;
esac

echo ""
echo "Done! $DONE_HINT"
