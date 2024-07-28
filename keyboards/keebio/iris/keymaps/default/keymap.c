// Copyright 2023 Danny Nguyen (@nooges)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "print.h"

#define _BASE 0
#define _ALT_LAYOUT 1
#define _NUMBERS 2
#define _NAV 3
#define _MODS 4


enum custom_keycodes {
    NUMBERS = SAFE_RANGE,
    NAV,
    SYMBOLS,
    M_LAYER_TOGGLE_LAYOUT,
    M_LAYER_TOGGLE_LAYOUT_LANGUAGE,
    M_UP_DIR,
    M_THE,
    M_MENT,
    M_UEN,
    M_ENT,
    M_ION,
    M_ON
};

// Combos
// Right mods
const uint16_t PROGMEM c_jgui[] = {MO(_NAV), KC_J, COMBO_END};
const uint16_t PROGMEM c_kalt[] = {MO(_NAV), KC_K, COMBO_END};
const uint16_t PROGMEM c_lctrl[] = {MO(_NAV), KC_L, COMBO_END};
const uint16_t PROGMEM c_clnsft[] = {MO(_NAV), KC_SCLN, COMBO_END};

// Left mods
const uint16_t PROGMEM c_fgui[] = {MO(_NUMBERS), KC_F, COMBO_END};
const uint16_t PROGMEM c_dalt[] = {MO(_NUMBERS), KC_D, COMBO_END};
const uint16_t PROGMEM c_sctrl[] = {MO(_NUMBERS), KC_S, COMBO_END};
const uint16_t PROGMEM c_asft[] = {MO(_NUMBERS), KC_A, COMBO_END};

// home row combos

const uint16_t PROGMEM c_jk[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM c_df[] = {KC_D, KC_F, COMBO_END};

combo_t key_combos[] = {
    COMBO(c_jgui, OSM(MOD_RGUI)),
    COMBO(c_kalt, OSM(MOD_RALT)),
    COMBO(c_lctrl, OSM(MOD_RCTL)),
    COMBO(c_clnsft, OSM(MOD_RSFT)),

    COMBO(c_fgui, OSM(MOD_LGUI)),
    COMBO(c_dalt, OSM(MOD_LALT)),
    COMBO(c_sctrl, OSM(MOD_LCTL)),
    COMBO(c_asft, OSM(MOD_LSFT)),

    COMBO(c_jk, KC_ENT),
    COMBO(c_df, KC_SPC),

};

static bool in_alt_layout = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______,  _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                               KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
 _______, LSFT_T(KC_Z), LCTL_T(KC_X), LALT_T(KC_C), LGUI_T(KC_V), KC_B, _______,     _______, KC_N, RGUI_T(KC_M), RALT_T(KC_COMM), RCTL_T(KC_DOT), RSFT_T(KC_SLSH), _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                     _______, MO(_NUMBERS), LT(_MODS, KC_SPC),   LT(_MODS, KC_ENT),  MO(_NAV),   _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_ALT_LAYOUT] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______,  _______, _______, _______, _______, _______,                               _______,    _______,    _______,    _______,    _______,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_V,    KC_M,    KC_L,    KC_C,    KC_P,                               KC_X,    KC_F,    KC_O,    KC_U,    KC_J,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_S,     KC_T,    KC_R,    KC_D,    KC_Y,                               KC_DOT,  KC_N,    KC_A,    KC_E,    KC_I, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
  _______, LSFT_T(KC_Z), LCTL_T(KC_K), LALT_T(KC_Q), LGUI_T(KC_G), KC_W, _______,  _______,KC_B, RGUI_T(KC_H), RALT_T(KC_QUOT), RCTL_T(KC_SCLN), RSFT_T(KC_COMM), _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, MO(_NUMBERS), KC_SPC,               KC_ENT, MO(_NAV), _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_NUMBERS] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______,  _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, TO(_BASE), TO(_ALT_LAYOUT), _______, KC_BSLS, KC_LBRC,                  KC_RBRC,  KC_1,    KC_2,    KC_3,    _______,    _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______,  KC_PIPE, KC_GRV,   KC_TAB, KC_ESC,  KC_LPRN,                           KC_RPRN,  KC_4,    KC_5,    KC_6,    KC_0, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, KC_RSFT,  KC_LCBR, _______,     _______, KC_RCBR,  KC_7,    KC_8,    KC_9,    _______,    _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                    KC_DEL,  KC_RSFT, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_NAV] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______,  _______, _______, _______, _______,                           _______,  _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______,   _______, _______,  _______,                          KC_HOME,  KC_PGDN, KC_PGUP, KC_END, KC_QUOT, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, KC_MINS, KC_PLUS, KC_UNDS, KC_EQL, KC_UNDS,                             KC_LEFT,  KC_DOWN, KC_UP, KC_RIGHT, KC_DQT, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______,    _______,  _______, _______, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, KC_LSFT, KC_BSPC,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  ),

  [_MODS] = LAYOUT(
  //┌────────┬────────┬────────┬────────┬────────┬────────┐                          ┌────────┬────────┬────────┬────────┬────────┬────────┐
     _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______,                            _______, KC_EXLM, KC_AT, KC_HASH, _______, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┤                          ├────────┼────────┼────────┼────────┼────────┼────────┤
     _______, OSM(MOD_LSFT), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_LGUI), LSFT(KC_QUOT),  _______, OSM(MOD_RGUI), OSM(MOD_RALT), OSM(MOD_RCTL), KC_RPRN, _______,
  //├────────┼────────┼────────┼────────┼────────┼────────┼────────┐        ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
     _______, _______, _______, _______, _______, _______, _______,          _______, _______, KC_AMPR, KC_ASTR, KC_LPRN, _______, _______,
  //└────────┴────────┴────────┴───┬────┴───┬────┴───┬────┴───┬────┘        └───┬────┴───┬────┴───┬────┴───┬────┴────────┴────────┴────────┘
                                    _______, _______, _______,                   _______, _______, _______
                                // └────────┴────────┴────────┘                 └────────┴────────┴────────┘
  )
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case M_LAYER_TOGGLE_LAYOUT:
      if (record->event.pressed) {
        if (in_alt_layout) {
          layer_off(_ALT_LAYOUT);
          in_alt_layout = false;
        } else {
          layer_on(_ALT_LAYOUT);
          in_alt_layout = true;
        }
      }
      return false;
      break;
    case M_LAYER_TOGGLE_LAYOUT_LANGUAGE:
      if (record->event.pressed) {
        if (in_alt_layout) {
          layer_off(_ALT_LAYOUT);
          in_alt_layout = false;
        } else {
          layer_on(_ALT_LAYOUT);
          in_alt_layout = true;
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
    case M_UP_DIR: SEND_STRING("./"); break;
    case M_THE: SEND_STRING("the"); break;
    case M_MENT: SEND_STRING("ment"); break;
    case M_UEN: SEND_STRING("uen"); break;
    case M_ENT: SEND_STRING("ent"); break;
    case M_ION: SEND_STRING("ion"); break;
    case M_ON: SEND_STRING("on"); break;
  }
  return true;
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    /* bool shifted = (mods & MOD_MASK_SHIFT);  // Was Shift held? */
    /* bool ctrl = (mods & MOD_MASK_CTRL); */
    /* bool alt = (mods & MOD_MASK_ALT); */
    /* bool GUI = (mods & MOD_MASK_GUI); */
    print("Hello, world");
    switch (keycode) {
        case KC_A: return KC_O;
        case KC_C: return KC_Y;
        case KC_D: return KC_Y;
        case KC_E: return KC_U;
        case KC_G: return KC_Y;
        case KC_I: return M_ON;
        case KC_L: return KC_K;
        case KC_M: return M_ENT;
        case KC_N: return M_ION;
        case KC_O: return KC_A;
        case KC_P: return KC_Y;
        case KC_Q: return M_UEN;
        case KC_R: return KC_L;
        case KC_S: return KC_K;
        case KC_T: return M_MENT;
        case KC_U: return KC_E;
        case KC_Y: return KC_P;
        case KC_SPC: return M_THE;
        case KC_DOT: return M_UP_DIR;
    }

    return KC_TRNS;
}
