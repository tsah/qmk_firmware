#include QMK_KEYBOARD_H

// Layer definitions
enum layers {
    _QWERTY,
    _STURDY,
    _NUM,
    _NAV,
    _FUN,
    _MOUSE,
};

// Layer key aliases for readability
#define LA_NUM MO(_NUM)
#define LA_NAV MO(_NAV)
#define LA_FUN MO(_FUN)
#define LA_MOU MO(_MOUSE)

// Thumb philosophy:
// Each layer has a dedicated thumb key — no tri-layer, no order dependence.
//
// Middle thumbs are the primary layer keys:
//   Left middle  = MO(_NUM)    — numbers, symbols, brackets
//   Right middle = MO(_NAV)    — navigation, arrows, symbols
//
// Outer thumbs are secondary layer keys (less comfortable = less frequent):
//   Left outer   = MO(_MOUSE)  — mouse movement, clicks, scroll
//   Right outer  = MO(_FUN)    — F-keys, RGB, system controls
//
// Inner thumbs: Space (left), Enter (right) — always available
//
// Thumb combos (order matters):
//   Hold NUM (left mid), tap right mid = Shift → shifted numbers !@#$%^&*()
//
// Backspace and Delete are on layers (Ferris-style):
//   NAV layer inner left thumb  = Backspace
//   NUM layer inner right thumb = Delete

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

    // Layer 0: QWERTY
    // ┌──────┬──────┬──────┬──────┬──────┬──────┬────┐   ┌────┬──────┬──────┬──────┬──────┬──────┬──────┐
    // │  NO  │  Q   │  W   │  E   │  R   │  T   │ V- │   │Mute│  Y   │  U   │  I   │  O   │  P   │  NO  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┤   ├────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  NO  │Sft/A │Ctl/S │Alt/D │Gui/F │  G   │ V+ │   │PScr│  H   │Gui/J │Alt/K │Ctl/L │Sft/; │  NO  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┘   └────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  NO  │  Z   │  X   │  C   │  V   │  B   │            │  N   │  M   │  ,   │  .   │  /   │  NO  │
    // └──────┴──────┴──────┼──────┼──────┼──────┤            ├──────┼──────┼──────┼──────┴──────┴──────┘
    //                       │MOUSE │ NUM  │Space │            │Enter │ NAV  │ FUN  │
    //                       └──────┴──────┴──────┘            └──────┴──────┴──────┘
    [_QWERTY] = LAYOUT_split_3x6_3_ex2(KC_NO, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_VOLD, KC_MUTE, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_NO, KC_NO, LSFT_T(KC_A), LCTL_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), KC_G, KC_VOLU, KC_PSCR, KC_H, RGUI_T(KC_J), RALT_T(KC_K), RCTL_T(KC_L), RSFT_T(KC_SCLN), KC_NO, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_NO, LA_MOU, LA_NUM, KC_SPC, KC_ENT, LA_NAV, LA_FUN),

    // Layer 1: Sturdy
    [_STURDY] = LAYOUT_split_3x6_3_ex2(KC_NO, KC_V, KC_M, KC_L, KC_C, KC_P, KC_VOLD, KC_MUTE, KC_X, KC_F, KC_O, KC_U, KC_J, KC_NO, KC_NO, LSFT_T(KC_S), LCTL_T(KC_T), LALT_T(KC_R), LGUI_T(KC_D), KC_Y, KC_VOLU, KC_PSCR, KC_DOT, RGUI_T(KC_N), RALT_T(KC_A), RCTL_T(KC_E), RSFT_T(KC_I), KC_NO, KC_NO, KC_Z, KC_K, KC_Q, KC_G, KC_W, KC_B, KC_H, KC_QUOT, KC_SCLN, KC_COMM, KC_NO, LA_MOU, LA_NUM, KC_SPC, KC_ENT, LA_NAV, LA_FUN),

    // Layer 2: Numbers/Symbols (hold left middle thumb)
    // ┌──────┬──────┬──────┬──────┬──────┬──────┬────┐   ┌────┬──────┬──────┬──────┬──────┬──────┬──────┐
    // │  __  │  __  │  __  │  __  │  \   │  [   │ __ │   │ __ │  ]   │  1   │  2   │  3   │  __  │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┤   ├────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │  |   │  `   │ Tab  │ Esc  │  (   │ __ │   │ __ │  )   │  4   │  5   │  6   │  0   │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┘   └────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │  __  │  __  │ Play │ Mute │  {   │            │  }   │  7   │  8   │  9   │  .   │  __  │
    // └──────┴──────┴──────┼──────┼──────┼──────┤            ├──────┼──────┼──────┼──────┴──────┴──────┘
    //                       │  __  │██████│  __  │            │ Del  │ Shft │  __  │
    //                       └──────┴──────┴──────┘            └──────┴──────┴──────┘
    [_NUM] = LAYOUT_split_3x6_3_ex2(_______, _______, _______, _______, KC_BSLS, KC_LBRC, _______, _______, KC_RBRC, KC_1, KC_2, KC_3, _______, _______, _______, KC_PIPE, KC_GRV, KC_TAB, KC_ESC, KC_LPRN, _______, _______, KC_RPRN, KC_4, KC_5, KC_6, KC_0, _______, _______, _______, _______, KC_MPLY, KC_MUTE, KC_LCBR, KC_RCBR, KC_7, KC_8, KC_9, KC_DOT, _______, _______, _______, _______, KC_DEL, KC_LSFT, _______),

    // Layer 3: Nav/Symbols (hold right middle thumb)
    // ┌──────┬──────┬──────┬──────┬──────┬──────┬────┐   ┌────┬──────┬──────┬──────┬──────┬──────┬──────┐
    // │  __  │  __  │  __  │  __  │  __  │  __  │ __ │   │ __ │ Home │ PgDn │ PgUp │ End  │  '   │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┤   ├────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │  -   │  +   │  _   │  =   │  __  │ __ │   │ __ │  ←   │  ↓   │  ↑   │  →   │  "   │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┘   └────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │  __  │  __  │  __  │  __  │  __  │            │  __  │  __  │  __  │  __  │  __  │  __  │
    // └──────┴──────┴──────┼──────┼──────┼──────┤            ├──────┼──────┼──────┼──────┴──────┴──────┘
    //                       │  __  │  __  │ BkSp │            │  __  │██████│  __  │
    //                       └──────┴──────┴──────┘            └──────┴──────┴──────┘
    [_NAV] = LAYOUT_split_3x6_3_ex2(_______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_QUOT, _______, _______, KC_MINS, KC_PLUS, KC_UNDS, KC_EQL, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_DQT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BSPC, _______, _______, _______),

    // Layer 4: F-Keys + System (hold right outer thumb)
    // ┌──────┬──────┬──────┬──────┬──────┬──────┬────┐   ┌────┬──────┬──────┬──────┬──────┬──────┬──────┐
    // │TO(0) │  __  │ TOG  │ MODE │  __  │  __  │ __ │   │ __ │  __  │  F1  │  F2  │  F3  │ F10  │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┤   ├────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │TO(1) │  __  │ HUE+ │ SAT+ │ VAL+ │  __  │ __ │   │ __ │  __  │  F4  │  F5  │  F6  │ F11  │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┘   └────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │  __  │ HUE- │ SAT- │ VAL- │  __  │            │  __  │  F7  │  F8  │  F9  │ F12  │  __  │
    // └──────┴──────┴──────┼──────┼──────┼──────┤            ├──────┼──────┼──────┼──────┴──────┴──────┘
    //                       │  __  │  __  │  __  │            │  __  │  __  │██████│
    //                       └──────┴──────┴──────┘            └──────┴──────┴──────┘
    [_FUN] = LAYOUT_split_3x6_3_ex2(TO(_QWERTY), _______, RM_TOGG, RM_NEXT, _______, _______, _______, _______, _______, KC_F1, KC_F2, KC_F3, KC_F10, _______, TO(_STURDY), _______, RM_HUEU, RM_SATU, RM_VALU, _______, _______, _______, _______, KC_F4, KC_F5, KC_F6, KC_F11, _______, _______, _______, RM_HUED, RM_SATD, RM_VALD, _______, _______, KC_F7, KC_F8, KC_F9, KC_F12, _______, _______, _______, _______, _______, _______, _______),

    // Layer 5: Mouse (hold left outer thumb)
    // ┌──────┬──────┬──────┬──────┬──────┬──────┬────┐   ┌────┬──────┬──────┬──────┬──────┬──────┬──────┐
    // │  __  │  __  │ MS_U │  __  │  __  │  __  │ __ │   │ __ │  __  │ WH_D │ WH_U │  __  │  __  │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┤   ├────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │ MS_L │ MS_D │ MS_R │  __  │  __  │ __ │   │ __ │ WH_L │ BTN1 │ BTN3 │ BTN2 │ WH_R │  __  │
    // ├──────┼──────┼──────┼──────┼──────┼──────┼────┘   └────┼──────┼──────┼──────┼──────┼──────┼──────┤
    // │  __  │  __  │  __  │  __  │  __  │  __  │            │  __  │  __  │  __  │  __  │  __  │  __  │
    // └──────┴──────┴──────┼──────┼──────┼──────┤            ├──────┼──────┼──────┼──────┴──────┴──────┘
    //                       │██████│  __  │  __  │            │ BTN1 │ BTN2 │  __  │
    //                       └──────┴──────┴──────┘            └──────┴──────┴──────┘
    [_MOUSE] = LAYOUT_split_3x6_3_ex2(_______, _______, MS_UP, _______, _______, _______, _______, _______, _______, MS_WHLD, MS_WHLU, _______, _______, _______, _______, MS_LEFT, MS_DOWN, MS_RGHT, _______, _______, _______, _______, MS_WHLL, MS_BTN1, MS_BTN3, MS_BTN2, MS_WHLR, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MS_BTN1, MS_BTN2, _______),
};
