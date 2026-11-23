// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "tsah.h"

// Keebart 3w6 Pro (split 3x5+3). The layers are shared with the Corne and
// live in users/tsah/tsah.h; this board has exactly the 36-key core.

// clang-format off
#define LAYOUT_tsah(...) LAYOUT_split_3x5_3(__VA_ARGS__)

// Chordal Hold hand layout for the 3w6 Pro (8 rows x 5 cols)
// Rows 0-3: left half, Rows 4-7: right half
// 'L' = left hand, 'R' = right hand, '*' = either (thumbs/unused)
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    {'L', 'L', 'L', 'L', 'L'}, // row 0: left top
    {'L', 'L', 'L', 'L', 'L'}, // row 1: left mid
    {'L', 'L', 'L', 'L', 'L'}, // row 2: left bot
    {'*', '*', '*', '*', '*'}, // row 3: left thumbs
    {'R', 'R', 'R', 'R', 'R'}, // row 4: right top
    {'R', 'R', 'R', 'R', 'R'}, // row 5: right mid
    {'R', 'R', 'R', 'R', 'R'}, // row 6: right bot
    {'*', '*', '*', '*', '*'}, // row 7: right thumbs
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_tsah(TSAH_QWERTY),
    [_STURDY] = LAYOUT_tsah(TSAH_STURDY),
    [_NUM]    = LAYOUT_tsah(TSAH_NUM),
    [_NAV]    = LAYOUT_tsah(TSAH_NAV),
    [_FUN]    = LAYOUT_tsah(TSAH_FUN),
    [_MOUSE]  = LAYOUT_tsah(TSAH_MOUSE),
};
// clang-format on
