#include QMK_KEYBOARD_H

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_5x6(
        KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                         KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_BSPC,
        KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                         KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,
        KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                         KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
        KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                         KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,KC_BSLS,
                         KC_LBRC,KC_RBRC,                                                       KC_PLUS, KC_EQL,
                                         RAISE,KC_SPC,                        KC_ENT, LOWER,
                                         KC_TAB,KC_HOME,                         KC_END,  KC_DEL,
                                         KC_BSPC, KC_GRV,                        KC_LGUI, KC_LALT
    ),

  [_QWERTY] = LAYOUT_5x6(
     KC_ESC , KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                         KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_GRV,
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                         KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_MINS,
     KC_PGUP, SFT_T(KC_A)  , CTL_T(KC_S)  , LALT_T(KC_D)  , GUI_T(KC_F) , KC_G  , KC_H  , GUI_T(KC_J)  , RALT_T(KC_K)  , RCTL_T(KC_L)  ,RSFT_T(KC_SCLN),KC_QUOT,
     KC_PGDN, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  ,                         KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,KC_BSLASH,
                      KC_LBRC,KC_RBRC,                                                       KC_PLUS, KC_EQL,
                                      RAISE,KC_SPC,                        KC_ENT, LOWER,
                                      KC_DEL,  KC_BSPC,                    KC_TAB,KC_ESC,                         
                                      KC_QUOT, KC_GRV,                        KC_EQL, KC_PLUS
  ),

  [_LOWER] = LAYOUT_5x6(
  _______, _______, _______, _______, _______, _______,                 _______, _______, _______, _______, _______, _______,
  _______, KC_PSCR,   KC_WH_U, KC_MS_U, KC_WH_D,  KC_WH_U,              KC_PGUP, KC_HOME, KC_UP, KC_END, KC_DEL, _______,
  _______, KC_BTN3, KC_MS_L, KC_MS_D, KC_MS_R,  KC_LGUI,                 KC_PGDN, KC_LEFT, KC_DOWN,   KC_RIGHT, KC_BSPC, _______,
  _______, KC_BTN1, KC_BTN5, KC_BTN2, KC_BTN3,  KC_WH_D,                  KC_CAPS, _______,  KC_INS,  KC_TAB,  _______, _______,
            KC_GRV,  KC_TAB,                                                _______, _______,
                               	             _______, _______,                  _______, _______,
                                                       _______, _______,        _______, _______,
                                                       _______, _______,        _______, _______
  ),

  [_RAISE] = LAYOUT_5x6(
       KC_F12 , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 ,                        KC_F6  , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,
       _______,KC_F1, KC_F2, KC_F3, KC_F4,     KC_LBRC,                        KC_RBRC,KC_1,KC_2,KC_3 ,KC_MINS,KC_MUTE,
       _______,KC_F5,KC_F6  ,KC_F7,KC_F8,KC_LPRN,                        KC_RPRN,KC_4,KC_5,KC_6,KC_PLUS,KC_VOLU,
       _______,KC_F9,KC_F10,KC_F11,KC_F12,KC_LCBR,                        KC_RCBR,KC_7,KC_8,KC_9,KC_0,KC_VOLD,
                                               _______,_______,            KC_ASTR ,KC_SLSH,
                                               _______,_______,            KC_EQL,KC_RSFT,
                                               _______,_______,            _______,_______,
                                               _______,_______,            _______,_______
  ),
};
