// SPDX-License-Identifier: GPL-2.0-or-later
// Shared config for every keyboard with a "tsah" keymap (QMK picks up
// users/<keymap name>/ automatically). Board-specific settings (RGB defaults,
// chordal_hold_layout) stay in each keymap.
#pragma once

// Home row mods: Chordal Hold + Permissive Hold
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

// Tap-hold settings (matching Ferris tuning)
#define TAPPING_TERM 200
#define QUICK_TAP_TERM 175

// Mouse key settings — comfortable speed/accuracy trade-off
// assuming a screen refresh rate of 60 Hz or higher
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_TIME_TO_MAX 40
#define MOUSEKEY_DELAY 100
#define MOUSEKEY_WHEEL_DELAY 100
#define MOUSEKEY_WHEEL_INTERVAL 50
#define MOUSEKEY_WHEEL_TIME_TO_MAX 100
