// SPDX-License-Identifier: GPL-2.0-or-later
// Shared config for every keyboard with a "tsah" keymap (QMK picks up
// users/<keymap name>/ automatically). Board-specific settings (RGB defaults,
// chordal_hold_layout) stay in each keymap.
#pragma once

// Home row mods. Tuned against the misfire tsah actually gets: an intended
// mod coming out as a letter (not stray mods while typing). So instead of
// urob's full "timeless" recipe (long term + 150 ms prior-idle, which trade
// mod reliability for typing safety) this keeps the term short and uses only
// the parts that help:
//
//   PERMISSIVE_HOLD   opposite-hand key pressed AND released inside the mod
//                     hold -> mod (ZMK "balanced")
//   CHORDAL_HOLD      same-hand chords type letters, except the Cmd shortcut
//                     allow-list in tsah.c (get_chordal_hold)
//   FLOW_TAP_TERM     only keys pressed < 100 ms after a letter are forced to
//                     be letters: fast typing stays safe, deliberate
//                     shortcuts (normally after a short pause) are untouched
//
// Every tap-hold key in these keymaps is a home row mod (thumbs are plain
// MO()), so global terms are fine. See REPORT-hrm-review.md for tuning.
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

#define TAPPING_TERM 200
#define QUICK_TAP_TERM 175

#define FLOW_TAP_TERM 100

// Mouse key settings — comfortable speed/accuracy trade-off
// assuming a screen refresh rate of 60 Hz or higher
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_TIME_TO_MAX 40
#define MOUSEKEY_DELAY 100
#define MOUSEKEY_WHEEL_DELAY 100
#define MOUSEKEY_WHEEL_INTERVAL 50
#define MOUSEKEY_WHEEL_TIME_TO_MAX 100
