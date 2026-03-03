#!/usr/bin/env bash
set -euo pipefail

KB="crkbd/rev4_1/standard"
KM="tsah"
UF2="crkbd_rev4_1_standard_tsah.uf2"
QMK_DIR="$(git -C "$(dirname "$0")" rev-parse --show-toplevel)"
UF2_PATH="$QMK_DIR/$UF2"
DRIVE_LABEL="RPI-RP2"

compile() {
    echo "Compiling..."
    qmk compile -kb "$KB" -km "$KM"
    if [[ ! -f "$UF2_PATH" ]]; then
        echo "ERROR: UF2 not found at $UF2_PATH"
        exit 1
    fi
    echo "Compiled: $UF2_PATH"
}

wait_for_boot_drive() {
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

find_mount_point() {
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

    if [[ -z "$mount_point" ]]; then
        echo "ERROR: Could not find mounted $DRIVE_LABEL partition"
        echo "Try manually: cp \"$UF2_PATH\" /path/to/$DRIVE_LABEL/"
        exit 1
    fi

    printf '%s\n' "$mount_point"
}

wait_for_disconnect() {
    local part="$1"
    while lsblk -prno PATH "$part" >/dev/null 2>&1; do
        sleep 0.25
    done
}

wait_and_flash() {
    local side="$1"
    local part
    local mount_point

    echo ""
    echo "=== Flash $side half ==="
    echo "Put the $side half in boot mode now."
    echo "Waiting for $DRIVE_LABEL..."

    part="$(wait_for_boot_drive)"
    echo "Detected: $part"

    mount_point="$(find_mount_point "$part")"

    cp "$UF2_PATH" "$mount_point/"
    sync
    echo "$side half flashed!"

    echo "Waiting for device to reboot..."
    wait_for_disconnect "$part"
}

compile
wait_and_flash "LEFT"
wait_and_flash "RIGHT"

echo ""
echo "Done! Connect TRRS cable, plug USB into left half, and test."
