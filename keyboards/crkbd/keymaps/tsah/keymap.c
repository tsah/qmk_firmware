#include QMK_KEYBOARD_H
#include "tsah.h"

// Corne v4.1 (split 3x6+3 with 2 extra inner keys per side). The layers are
// shared with the 3w6 Pro and live in users/tsah/tsah.h (the 36-key core).
// This wrapper places that core on the Corne and fills in the extras:
//   - outer pinky column: unused (KC_NO)
//   - extra inner keys:   Vol- / Mute (top), Vol+ / PScr (home) on the base
//                         layers, transparent on the others
//
//   ┌────┬─────────────────┬────┐   ┌────┬─────────────────┬────┐
//   │ NO │   core row 1    │ x1 │   │ x2 │   core row 1    │ NO │
//   │ NO │   core row 2    │ x3 │   │ x4 │   core row 2    │ NO │
//   │ NO │   core row 3    │         │   core row 3    │ NO │
//   └────┴─────┬───────────┤         ├───────────┬─────┴────┘
//              │  thumbs   │         │  thumbs   │

// clang-format off
#define LAYOUT_corne_tsah( \
    x1, x2, x3, x4, \
    L00, L01, L02, L03, L04,   R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,   R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,   R20, R21, R22, R23, R24, \
              T0,  T1,  T2,    T3,  T4,  T5 \
) LAYOUT_split_3x6_3_ex2( \
    KC_NO, L00, L01, L02, L03, L04, x1,   x2, R00, R01, R02, R03, R04, KC_NO, \
    KC_NO, L10, L11, L12, L13, L14, x3,   x4, R10, R11, R12, R13, R14, KC_NO, \
    KC_NO, L20, L21, L22, L23, L24,           R20, R21, R22, R23, R24, KC_NO, \
                     T0,  T1,  T2,            T3,  T4,  T5 \
)
// Expand the layer macro before it is split into arguments.
#define LAYOUT_tsah(extras, ...) LAYOUT_corne_tsah_expand(extras, __VA_ARGS__)
#define LAYOUT_corne_tsah_expand(...) LAYOUT_corne_tsah(__VA_ARGS__)

#define CORNE_BASE_EXTRAS KC_VOLD, KC_MUTE, KC_VOLU, KC_PSCR
#define CORNE_NO_EXTRAS   _______, _______, _______, _______

// Chordal Hold hand layout for Corne v4 (8 rows x 7 cols)
// Rows 0-3: left half, Rows 4-7: right half
// 'L' = left hand, 'R' = right hand, '*' = either (thumbs/unused)
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = {
    {'L', 'L', 'L', 'L', 'L', 'L', 'L'}, // row 0: left top
    {'L', 'L', 'L', 'L', 'L', 'L', 'L'}, // row 1: left mid
    {'L', 'L', 'L', 'L', 'L', 'L', '*'}, // row 2: left bot
    {'*', '*', '*', '*', '*', '*', '*'}, // row 3: left thumbs
    {'R', 'R', 'R', 'R', 'R', 'R', 'R'}, // row 4: right top
    {'R', 'R', 'R', 'R', 'R', 'R', 'R'}, // row 5: right mid
    {'R', 'R', 'R', 'R', 'R', 'R', '*'}, // row 6: right bot
    {'*', '*', '*', '*', '*', '*', '*'}, // row 7: right thumbs
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_tsah(CORNE_BASE_EXTRAS, TSAH_QWERTY),
    [_STURDY] = LAYOUT_tsah(CORNE_BASE_EXTRAS, TSAH_STURDY),
    [_NUM]    = LAYOUT_tsah(CORNE_NO_EXTRAS,   TSAH_NUM),
    [_NAV]    = LAYOUT_tsah(CORNE_NO_EXTRAS,   TSAH_NAV),
    [_FUN]    = LAYOUT_tsah(CORNE_NO_EXTRAS,   TSAH_FUN),
    [_MOUSE]  = LAYOUT_tsah(CORNE_NO_EXTRAS,   TSAH_MOUSE),
};
// clang-format on
