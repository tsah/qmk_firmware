// SPDX-License-Identifier: GPL-2.0-or-later
// Shared config for every keyboard with a "tsah" keymap (QMK picks up
// users/<keymap name>/ automatically). Board-specific settings (RGB defaults,
// chordal_hold_layout) stay in each keymap.
#pragma once

// Home row mods, "timeless" style (after urob's ZMK config), mapped to QMK:
//
//   ZMK (urob)                          QMK equivalent here
//   flavor = "balanced"              -> PERMISSIVE_HOLD
//   positional hold-tap (opposite    -> CHORDAL_HOLD + chordal_hold_layout in
//     hand + thumbs trigger holds)      each keymap (thumbs are '*')
//   hold-trigger-on-release          -> Chordal Hold's same-hand multi-mod
//                                       exception (Ctrl+Shift+... works)
//   require-prior-idle-ms = 150      -> FLOW_TAP_TERM 150 (see tsah.c)
//   tapping-term-ms = 280            -> TAPPING_TERM 280
//   quick-tap-ms = 175               -> QUICK_TAP_TERM 175
//
// Every tap-hold key in these keymaps is a home row mod (thumbs are plain
// MO()), so global terms are fine. See REPORT-hrm-review.md for tuning.
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

// Long tapping term: with Permissive + Chordal Hold, intentional mods are
// decided by key order (nested opposite-hand press), not by time, so the term
// mainly guards against slow/lingering taps being read as holds.
#define TAPPING_TERM 280
#define QUICK_TAP_TERM 175

// A home row mod pressed within 150 ms of the previous alpha key is a tap.
#define FLOW_TAP_TERM 150

// Mouse key settings — comfortable speed/accuracy trade-off
// assuming a screen refresh rate of 60 Hz or higher
#define MOUSEKEY_INTERVAL 16
#define MOUSEKEY_TIME_TO_MAX 40
#define MOUSEKEY_DELAY 100
#define MOUSEKEY_WHEEL_DELAY 100
#define MOUSEKEY_WHEEL_INTERVAL 50
#define MOUSEKEY_WHEEL_TIME_TO_MAX 100
