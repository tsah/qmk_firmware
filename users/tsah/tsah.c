// SPDX-License-Identifier: GPL-2.0-or-later
// Shared code for every keyboard with a "tsah" keymap.
#include "quantum.h"

#ifdef FLOW_TAP_TERM
// Keys that take part in Flow Tap ("require prior idle"), both as the
// previous key and as the home row mod being pressed. QMK's default list
// plus KC_QUOT, which is an alpha-area key on the Sturdy base layer.
bool is_flow_tap_key(uint16_t keycode) {
    if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
        return false; // A hotkey is in progress: let further mods chord.
    }
    switch (get_tap_keycode(keycode)) {
        case KC_SPC:
        case KC_A ... KC_Z:
        case KC_DOT:
        case KC_COMM:
        case KC_SCLN:
        case KC_SLSH:
        case KC_QUOT:
            return true;
    }
    return false;
}

static bool is_shift_mod_tap(uint16_t keycode) {
    return IS_QK_MOD_TAP(keycode) && (QK_MOD_TAP_GET_MODS(keycode) & 0x0F) == MOD_LSFT;
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    if (!is_flow_tap_key(keycode) || !is_flow_tap_key(prev_keycode)) {
        return 0;
    }
    // Capitalising the first letter of a word: Space then a quick Shift hold
    // must still shift. Mid-word (previous key a letter) Flow Tap applies,
    // which is where lingering-pinky Shift misfires happen.
    if (is_shift_mod_tap(keycode) && get_tap_keycode(prev_keycode) == KC_SPC) {
        return 0;
    }
    return FLOW_TAP_TERM;
}
#endif // FLOW_TAP_TERM
