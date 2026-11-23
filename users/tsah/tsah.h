// SPDX-License-Identifier: GPL-2.0-or-later
// Shared layers for every keyboard with a "tsah" keymap.
//
// Each layer is the 36-key core (3x5 per side + 3 thumbs per side), written
// once here. Each keymap supplies a LAYOUT_tsah(...) wrapper that places these
// 36 keys on its own matrix and fills in its board-specific extras:
//   keebart/3w6_pro: exactly 36 keys, passed straight through.
//   crkbd (Corne):   adds the outer pinky column (unused) and the 4 extra
//                    inner keys (Vol-/Mute/Vol+/PScr on the base layers).
#pragma once

#include "quantum.h"

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
// Thumb chord:
//   Hold NUM (left mid), then hold right mid = Shift → shifted numbers !@#$%^&*()
//
// Backspace and Delete are on layers (Ferris-style):
//   NAV layer inner left thumb  = Backspace
//   NUM layer inner right thumb = Delete
//
// Home row mods (both base layers): Shift-Ctrl-Alt-Gui from pinky to index.
// Tap-hold behaviour is in users/tsah/config.h and tsah.c.

// Layer 0: QWERTY
// ┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
// │  Q   │  W   │  E   │  R   │  T   │   │  Y   │  U   │  I   │  O   │  P   │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │Sft/A │Ctl/S │Alt/D │Gui/F │  G   │   │  H   │Gui/J │Alt/K │Ctl/L │Sft/; │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  Z   │  X   │  C   │  V   │  B   │   │  N   │  M   │  ,   │  .   │  /   │
// └──────┴──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┴──────┘
//               │MOUSE │ NUM  │Space │   │Enter │ NAV  │ FUN  │
//               └──────┴──────┴──────┘   └──────┴──────┴──────┘
// clang-format off
#define TSAH_QWERTY \
    KC_Q,          KC_W,          KC_E,          KC_R,          KC_T,             KC_Y,          KC_U,          KC_I,          KC_O,          KC_P,            \
    LSFT_T(KC_A),  LCTL_T(KC_S),  LALT_T(KC_D),  LGUI_T(KC_F),  KC_G,             KC_H,          RGUI_T(KC_J),  RALT_T(KC_K),  RCTL_T(KC_L),  RSFT_T(KC_SCLN), \
    KC_Z,          KC_X,          KC_C,          KC_V,          KC_B,             KC_N,          KC_M,          KC_COMM,       KC_DOT,        KC_SLSH,         \
                                  LA_MOU,        LA_NUM,        KC_SPC,           KC_ENT,        LA_NAV,        LA_FUN

// Layer 1: Sturdy
// ┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
// │  V   │  M   │  L   │  C   │  P   │   │  X   │  F   │  O   │  U   │  J   │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │Sft/S │Ctl/T │Alt/R │Gui/D │  Y   │   │  .   │Gui/N │Alt/A │Ctl/E │Sft/I │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  Z   │  K   │  Q   │  G   │  W   │   │  B   │  H   │  '   │  ;   │  ,   │
// └──────┴──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┴──────┘
//               │MOUSE │ NUM  │Space │   │Enter │ NAV  │ FUN  │
//               └──────┴──────┴──────┘   └──────┴──────┴──────┘
#define TSAH_STURDY \
    KC_V,          KC_M,          KC_L,          KC_C,          KC_P,             KC_X,          KC_F,          KC_O,          KC_U,          KC_J,            \
    LSFT_T(KC_S),  LCTL_T(KC_T),  LALT_T(KC_R),  LGUI_T(KC_D),  KC_Y,             KC_DOT,        RGUI_T(KC_N),  RALT_T(KC_A),  RCTL_T(KC_E),  RSFT_T(KC_I),    \
    KC_Z,          KC_K,          KC_Q,          KC_G,          KC_W,             KC_B,          KC_H,          KC_QUOT,       KC_SCLN,       KC_COMM,         \
                                  LA_MOU,        LA_NUM,        KC_SPC,           KC_ENT,        LA_NAV,        LA_FUN

// Layer 2: Numbers/Symbols (hold left middle thumb)
// ┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
// │  __  │  __  │  __  │  \   │  [   │   │  ]   │  1   │  2   │  3   │  __  │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  |   │  `   │ Tab  │ Esc  │  (   │   │  )   │  4   │  5   │  6   │  0   │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │ Vol- │ Vol+ │ Play │ Mute │  {   │   │  }   │  7   │  8   │  9   │  .   │
// └──────┴──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┴──────┘
//               │  __  │██████│  __  │   │ Del  │ Shft │  __  │
//               └──────┴──────┴──────┘   └──────┴──────┴──────┘
#define TSAH_NUM \
    _______,       _______,       _______,       KC_BSLS,       KC_LBRC,          KC_RBRC,       KC_1,          KC_2,          KC_3,          _______,         \
    KC_PIPE,       KC_GRV,        KC_TAB,        KC_ESC,        KC_LPRN,          KC_RPRN,       KC_4,          KC_5,          KC_6,          KC_0,            \
    KC_VOLD,       KC_VOLU,       KC_MPLY,       KC_MUTE,       KC_LCBR,          KC_RCBR,       KC_7,          KC_8,          KC_9,          KC_DOT,          \
                                  _______,       _______,       _______,          KC_DEL,        KC_LSFT,       _______

// Layer 3: Nav/Symbols (hold right middle thumb)
// CapsWd = Caps Word: types CAPITALS (and keeps going through _ and digits)
// until the next space or other non-word key.
// ┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
// │  __  │  __  │  __  │  __  │  __  │   │ Home │ PgDn │ PgUp │ End  │  '   │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  -   │  +   │  _   │  =   │CapsWd│   │  ←   │  ↓   │  ↑   │  →   │  "   │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  __  │  __  │ PScr │  __  │  __  │   │  __  │  __  │  __  │  __  │  __  │
// └──────┴──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┴──────┘
//               │  __  │  __  │ BkSp │   │  __  │██████│  __  │
//               └──────┴──────┴──────┘   └──────┴──────┴──────┘
#define TSAH_NAV \
    _______,       _______,       _______,       _______,       _______,          KC_HOME,       KC_PGDN,       KC_PGUP,       KC_END,        KC_QUOT,         \
    KC_MINS,       KC_PLUS,       KC_UNDS,       KC_EQL,        CW_TOGG,          KC_LEFT,       KC_DOWN,       KC_UP,         KC_RGHT,       KC_DQT,          \
    _______,       _______,       KC_PSCR,       _______,       _______,          _______,       _______,       _______,       _______,       _______,         \
                                  _______,       _______,       KC_BSPC,          _______,       _______,       _______

// Layer 4: F-Keys + System (hold right outer thumb)
// Boot = reboot the USB-connected half into its bootloader (for flashing).
// ┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
// │TO(0) │ TOG  │ MODE │  __  │  __  │   │  __  │  F1  │  F2  │  F3  │ F10  │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │TO(1) │ HUE+ │ SAT+ │ VAL+ │  __  │   │  __  │  F4  │  F5  │  F6  │ F11  │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │ Boot │ HUE- │ SAT- │ VAL- │  __  │   │  __  │  F7  │  F8  │  F9  │ F12  │
// └──────┴──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┴──────┘
//               │  __  │  __  │  __  │   │  __  │  __  │██████│
//               └──────┴──────┴──────┘   └──────┴──────┴──────┘
#define TSAH_FUN \
    TO(_QWERTY),   RM_TOGG,       RM_NEXT,       _______,       _______,          _______,       KC_F1,         KC_F2,         KC_F3,         KC_F10,          \
    TO(_STURDY),   RM_HUEU,       RM_SATU,       RM_VALU,       _______,          _______,       KC_F4,         KC_F5,         KC_F6,         KC_F11,          \
    QK_BOOT,       RM_HUED,       RM_SATD,       RM_VALD,       _______,          _______,       KC_F7,         KC_F8,         KC_F9,         KC_F12,          \
                                  _______,       _______,       _______,          _______,       _______,       _______

// Layer 5: Mouse (hold left outer thumb)
// Movement sits on HJKL like the NAV arrows.
// ┌──────┬──────┬──────┬──────┬──────┐   ┌──────┬──────┬──────┬──────┬──────┐
// │  __  │  __  │  __  │  __  │  __  │   │  __  │ WH_D │ WH_U │  __  │  __  │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  __  │  __  │  __  │  __  │  __  │   │ MS_L │ MS_D │ MS_U │ MS_R │ WH_R │
// ├──────┼──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┼──────┤
// │  __  │  __  │  __  │  __  │  __  │   │  __  │ WH_L │ BTN1 │ BTN3 │ BTN2 │
// └──────┴──────┼──────┼──────┼──────┤   ├──────┼──────┼──────┼──────┴──────┘
//               │██████│  __  │  __  │   │ BTN1 │ BTN2 │  __  │
//               └──────┴──────┴──────┘   └──────┴──────┴──────┘
#define TSAH_MOUSE \
    _______,       _______,       _______,       _______,       _______,          _______,       MS_WHLD,       MS_WHLU,       _______,       _______,         \
    _______,       _______,       _______,       _______,       _______,          MS_LEFT,       MS_DOWN,       MS_UP,         MS_RGHT,       MS_WHLR,         \
    _______,       _______,       _______,       _______,       _______,          _______,       MS_WHLL,       MS_BTN1,       MS_BTN3,       MS_BTN2,         \
                                  _______,       _______,       _______,          MS_BTN1,       MS_BTN2,       _______
// clang-format on
