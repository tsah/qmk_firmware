// SPDX-License-Identifier: GPL-2.0-or-later
// Home row mod recorder: built only with TSAH_HRM_LOG=yes.
//
// Prints one line per key event to the QMK console, for
// users/tsah/hrm_log/analyze.py. Privacy: letters typed normally are NOT
// recorded, only which hand and the timing. The actual keycode is printed only
// while Ctrl/Alt/Cmd is held (shortcuts) and for a handful of editing keys.
//
// Line format (times in ms, t = when the key physically moved):
//   HRM <t> <P|R> <id> mt <hand> <mod> <tap|hold> <decision delay> <mods>
//   HRM <t> <P|R> <id> k  <hand> <token> <mods>
// id: an anonymous per-press counter, repeated on the matching release, so
// presses and releases can be paired without logging key positions.
// token: '-' (a letter or symbol, hidden), 'lyr' (layer key), or a keycode
// name/hex. mods: active modifier bits (hex) before this event.
#include "quantum.h"

static char hand_of(keyrecord_t *record) {
#ifdef CHORDAL_HOLD
    return chordal_hold_handedness(record->event.key);
#else
    return '?';
#endif
}

static const char *mod_name(uint16_t keycode) {
    switch (QK_MOD_TAP_GET_MODS(keycode) & 0x0F) {
        case MOD_LSFT:
            return "sft";
        case MOD_LCTL:
            return "ctl";
        case MOD_LALT:
            return "alt";
        case MOD_LGUI:
            return "gui";
    }
    return "mix";
}

static const char *named_key(uint16_t keycode) {
    switch (keycode) {
        case KC_BSPC:
            return "bspc";
        case KC_DEL:
            return "del";
        case KC_SPC:
            return "spc";
        case KC_ENT:
            return "ent";
        case KC_TAB:
            return "tab";
        case KC_ESC:
            return "esc";
    }
    return NULL;
}

static uint8_t press_id[MATRIX_ROWS][MATRIX_COLS];
static uint8_t next_id;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!IS_KEYEVENT(record->event)) {
        return true;
    }
    // Reconstruct a 32-bit timestamp of the physical event (event.time is a
    // 16-bit ms counter and may be older than now if the key was buffered).
    const uint16_t age  = TIMER_DIFF_16(timer_read(), record->event.time);
    const uint32_t t    = timer_read32() - age;
    const uint8_t  mods = get_mods() | get_oneshot_mods();
    const char     pr   = record->event.pressed ? 'P' : 'R';
    const char     hand = hand_of(record);
    const keypos_t key  = record->event.key;
    if (key.row >= MATRIX_ROWS || key.col >= MATRIX_COLS) {
        return true;
    }
    if (record->event.pressed) {
        press_id[key.row][key.col] = ++next_id;
    }
    const uint8_t id = press_id[key.row][key.col];

    if (IS_QK_MOD_TAP(keycode)) {
        uprintf("HRM %lu %c %u mt %c %s %s %u %02X\n", (unsigned long)t, pr, id, hand, mod_name(keycode), record->tap.count ? "tap" : "hold", age, mods);
    } else if (IS_QK_MOMENTARY(keycode) || IS_QK_LAYER_TAP(keycode) || IS_QK_TO(keycode)) {
        uprintf("HRM %lu %c %u k %c lyr %02X\n", (unsigned long)t, pr, id, hand, mods);
    } else if (named_key(keycode)) {
        uprintf("HRM %lu %c %u k %c %s %02X\n", (unsigned long)t, pr, id, hand, named_key(keycode), mods);
    } else if (mods & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI)) {
        uprintf("HRM %lu %c %u k %c %04X %02X\n", (unsigned long)t, pr, id, hand, keycode, mods);
    } else {
        uprintf("HRM %lu %c %u k %c - %02X\n", (unsigned long)t, pr, id, hand, mods);
    }
    return true;
}
