// SPDX-License-Identifier: GPL-2.0-or-later
// Shared code for every keyboard with a "tsah" keymap.
#include "quantum.h"

#ifdef CHORDAL_HOLD
// macOS Cmd shortcuts whose letter sits on the same hand as the Cmd home row
// key (e.g. Cmd on F + W). Chordal Hold would type "fw"; for these letters a
// nested press (letter pressed and released while Cmd is held) is Cmd+letter
// instead. A roll (Cmd key released first) still types both letters.
static bool is_cmd_shortcut_letter(uint16_t keycode) {
    switch (get_tap_keycode(keycode)) {
        case KC_A: // select all
        case KC_C: // copy
        case KC_F: // find
        case KC_L: // location bar
        case KC_N: // new
        case KC_O: // open
        case KC_P: // print / command palette
        case KC_Q: // quit
        case KC_R: // reload
        case KC_S: // save
        case KC_T: // new tab
        case KC_V: // paste
        case KC_W: // close
        case KC_X: // cut
        case KC_Z: // undo
            return true;
    }
    return false;
}

static bool is_gui_mod_tap(uint16_t keycode) {
    return IS_QK_MOD_TAP(keycode) && (QK_MOD_TAP_GET_MODS(keycode) & 0x0F) == MOD_LGUI;
}

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    if (is_gui_mod_tap(tap_hold_keycode) && is_cmd_shortcut_letter(other_keycode)) {
        return true;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}
#endif // CHORDAL_HOLD

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
    // Shift is exempt from Flow Tap: it is used straight after letters for
    // punctuation (? ! : " ) and camelCase, and the recorder showed Flow Tap
    // eating those ("mac?" typed as "maca/").
    if (is_shift_mod_tap(keycode)) {
        return 0;
    }
    return FLOW_TAP_TERM;
}
#endif // FLOW_TAP_TERM
