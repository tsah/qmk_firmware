// Copyright 2023 Danny Nguyen (@nooges)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define _BASE 0
#define _COLEMAK 1
#define _NUMBERS 2
#define _NAV 3
#define _SYMBOLS 4


enum custom_keycodes {
    NUMBERS = SAFE_RANGE,
    NAV,
    SYMBOLS,
    MACRO_LAYER_TOGGLE_LAYOUT,
    MACRO_LAYER_TOGGLE_LAYOUT_LANGUAGE
};

enum combo_events {
    COMBO_MACRO_LAYER_TOGGLE_LAYOUT_LANGUAGE,
    COMBO_MACRO_LAYER_TOGGLE_LAYOUT,
    COMBO_LENGTH  // This represents the number of combos
};

const uint16_t PROGMEM combo_macro_layer_toggle_layout_language[] = {KC_Q, KC_W, COMBO_END};
const uint16_t PROGMEM combo_macro_layer_toggle_layout[] = {KC_Z, KC_X, COMBO_END};

// Create an array of combos
combo_t key_combos[COMBO_LENGTH] = {
    [COMBO_MACRO_LAYER_TOGGLE_LAYOUT_LANGUAGE] = COMBO(combo_macro_layer_toggle_layout_language, MACRO_LAYER_TOGGLE_LAYOUT_LANGUAGE),
    [COMBO_MACRO_LAYER_TOGGLE_LAYOUT] = COMBO(combo_macro_layer_toggle_layout, MACRO_LAYER_TOGGLE_LAYOUT),
};

static bool in_colemak = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______,  _______,    _______,    _______,    _______,    _______,                               _______,    _______,    _______,    _______,    _______,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, LSFT_T(KC_A), LCTL_T(KC_S), LALT_T(KC_D), LGUI_T(KC_F), KC_G,          KC_H,    RGUI_T(KC_J), RALT_T(KC_K), RCTL_T(KC_L), RSFT_T(KC_SCLN), _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    _______,          _______,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                     _______, NUMBERS, KC_SPC,                    KC_ENT,    NAV,   _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_COLEMAK] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______,  _______, _______, _______, _______, _______,                               _______,    _______,    _______,    _______,    _______,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                               KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, LSFT_T(KC_A), LCTL_T(KC_R), LALT_T(KC_S), LGUI_T(KC_T), KC_G,          KC_M,    RGUI_T(KC_N), RALT_T(KC_E), RCTL_T(KC_I), RSFT_T(KC_O), _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    _______,          _______,  KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                     _______, NUMBERS, KC_SPC,                    KC_ENT,    NAV,   _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_NUMBERS] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______,  _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, KC_LBRC,                            KC_RBRC,    KC_1,    KC_2,    KC_3,    KC_0,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_ESC, KC_TAB,   KC_LT,   KC_GT,   KC_LPRN,                            KC_RPRN, KC_4,     KC_5,    KC_6,   KC_0, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, KC_GRV, KC_LCBR, _______,          _______, KC_RCBR,  KC_7,    KC_8,    KC_9,    KC_GRV,    _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                    KC_DEL,  _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_NAV] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______,  _______, _______, _______, _______,                           _______,  _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                            KC_HOME,  KC_PGDN, KC_PGUP, KC_END, KC_QUOT, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_MINS, KC_PLUS, KC_UNDS, KC_EQL, KC_UNDS,                             KC_LEFT,  KC_DOWN, KC_UP, KC_RIGHT, KC_DQT, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_MSTP, KC_MPLY, KC_VOLD, KC_PGDN, KC_MINS, KC_LPRN,          _______, KC_0,    _______,  _______, KC_DLR, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, KC_BSPC,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_SYMBOLS] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, KC_EXLM, KC_AT, KC_HASH, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_PIPE, KC_BSLS, LCTL(KC_BSLS), KC_QUOT, LSFT(KC_QUOT),                 _______, KC_DLR, KC_PERC, KC_CIRC, KC_RPRN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, KC_AMPR, KC_ASTR, KC_LPRN, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  )
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case NUMBERS:
      if (record->event.pressed) {
        layer_on(_NUMBERS);
        update_tri_layer(_NUMBERS, _NAV, _SYMBOLS);
      } else {
        layer_off(_NUMBERS);
        update_tri_layer(_NUMBERS, _NAV, _SYMBOLS);
      }
      return false;
      break;
    case NAV:
      if (record->event.pressed) {
        layer_on(_NAV);
        update_tri_layer(_NUMBERS, _NAV, _SYMBOLS);
      } else {
        layer_off(_NAV);
        update_tri_layer(_NUMBERS, _NAV, _SYMBOLS);
      }
      return false;
      break;
    case SYMBOLS:
      if (record->event.pressed) {
        layer_on(_SYMBOLS);
      } else {
        layer_off(_SYMBOLS);
      }
      return false;
      break;
    case MACRO_LAYER_TOGGLE_LAYOUT:
      if (record->event.pressed) {
        if (in_colemak) {
          layer_off(_COLEMAK);
          in_colemak = false;
        } else {
          layer_on(_COLEMAK);
          in_colemak = true;
        }
      }
      return false;
      break;
    case MACRO_LAYER_TOGGLE_LAYOUT_LANGUAGE:
      if (record->event.pressed) {
        if (in_colemak) {
          layer_off(_COLEMAK);
          in_colemak = false;
        } else {
          layer_on(_COLEMAK);
          in_colemak = true;
        }
        register_code(KC_LCTL);
        register_code(KC_LALT);
        register_code(KC_LGUI);
        register_code(KC_SPC);
        unregister_code(KC_SPC);
        unregister_code(KC_LGUI);
        unregister_code(KC_LALT);
        unregister_code(KC_LCTL);
      }
      return false;
      break;
  }
  return true;
}
