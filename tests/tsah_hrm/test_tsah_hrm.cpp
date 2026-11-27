#include "keyboard_report_util.hpp"
#include "keycode.h"
#include "test_common.hpp"
#include "action_tapping.h"
#include "test_fixture.hpp"
#include "test_keymap_key.hpp"

using testing::_;
using testing::InSequence;

class TsahHrm : public TestFixture {};

// Mid-word roll: N (right), then 50 ms later Ctrl/S (left) with another
// right-hand key nested inside it. Flow Tap must make A a tap ("sns", not Ctrl+N).
TEST_F(TsahHrm, flow_tap_mid_word_is_tap) {
    TestDriver driver;
    InSequence s;
    auto n     = KeymapKey(0, 6, 1, KC_N);
    auto sft_a = KeymapKey(0, 1, 1, LCTL_T(KC_S));
    set_keymap({n, sft_a});

    EXPECT_REPORT(driver, (KC_N));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(n);
    idle_for(50);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_S));
    sft_a.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_S, KC_N));
    EXPECT_REPORT(driver, (KC_S));
    tap_key(n);
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    sft_a.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// "mac?": Shift/A pressed 60 ms after a letter, "/" nested inside it -> "?".
TEST_F(TsahHrm, shift_right_after_letter_still_shifts) {
    TestDriver driver;
    InSequence s;
    auto c     = KeymapKey(0, 3, 2, KC_C);
    auto slsh  = KeymapKey(0, 9, 2, KC_SLSH);
    auto sft_a = KeymapKey(0, 0, 1, LSFT_T(KC_A));
    set_keymap({c, slsh, sft_a});

    EXPECT_REPORT(driver, (KC_C));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(c);
    idle_for(60);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    sft_a.press();
    idle_for(70);
    slsh.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_REPORT(driver, (KC_LSFT, KC_SLSH));
    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_EMPTY_REPORT(driver);
    slsh.release();
    run_one_scan_loop();
    sft_a.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Same timing, but the previous key is Space: Shift is exempt so a fast
// capital at the start of a word still works.
TEST_F(TsahHrm, shift_right_after_space_still_shifts) {
    TestDriver driver;
    InSequence s;
    auto spc   = KeymapKey(0, 2, 3, KC_SPC);
    auto n     = KeymapKey(0, 6, 1, KC_N);
    auto sft_a = KeymapKey(0, 0, 1, LSFT_T(KC_A));
    set_keymap({spc, n, sft_a});

    EXPECT_REPORT(driver, (KC_SPC));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(spc);
    idle_for(50);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    sft_a.press();
    idle_for(30);
    n.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_REPORT(driver, (KC_LSFT, KC_N));
    EXPECT_REPORT(driver, (KC_LSFT));
    n.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    sft_a.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// The Space exemption is Shift-only: Gui/F right after Space is still a tap.
TEST_F(TsahHrm, gui_right_after_space_is_tap) {
    TestDriver driver;
    InSequence s;
    auto spc   = KeymapKey(0, 2, 3, KC_SPC);
    auto n     = KeymapKey(0, 6, 1, KC_N);
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    set_keymap({spc, n, gui_f});

    EXPECT_REPORT(driver, (KC_SPC));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(spc);
    idle_for(50);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_F));
    gui_f.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_F, KC_N));
    EXPECT_REPORT(driver, (KC_F));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(n);
    gui_f.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// After idle, an opposite-hand nested press is a hold well inside the term
// (Permissive Hold): Cmd+N.
TEST_F(TsahHrm, idle_then_opposite_hand_nested_is_hold) {
    TestDriver driver;
    InSequence s;
    auto n     = KeymapKey(0, 6, 1, KC_N);
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    set_keymap({n, gui_f});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    gui_f.press();
    idle_for(40);
    n.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_REPORT(driver, (KC_LGUI, KC_N));
    EXPECT_REPORT(driver, (KC_LGUI));
    n.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    gui_f.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// A shortcut 120 ms after the last letter (outside the 100 ms Flow Tap term)
// is still a mod: typing a word then Cmd+N.
TEST_F(TsahHrm, shortcut_120ms_after_letter_is_hold) {
    TestDriver driver;
    InSequence s;
    auto e     = KeymapKey(0, 2, 0, KC_E);
    auto n     = KeymapKey(0, 6, 1, KC_N);
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    set_keymap({e, n, gui_f});

    EXPECT_REPORT(driver, (KC_E));
    EXPECT_EMPTY_REPORT(driver);
    tap_key(e);
    idle_for(120);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    gui_f.press();
    idle_for(30);
    n.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_REPORT(driver, (KC_LGUI, KC_N));
    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_EMPTY_REPORT(driver);
    n.release();
    run_one_scan_loop();
    gui_f.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Same-hand Cmd shortcut on the allow-list: Cmd/F with W nested -> Cmd+W.
TEST_F(TsahHrm, same_hand_cmd_shortcut_nested_is_hold) {
    TestDriver driver;
    InSequence s;
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    auto w     = KeymapKey(0, 1, 0, KC_W);
    set_keymap({gui_f, w});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    gui_f.press();
    idle_for(40);
    w.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_REPORT(driver, (KC_LGUI, KC_W));
    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_EMPTY_REPORT(driver);
    w.release();
    run_one_scan_loop();
    gui_f.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Same, where the letter is itself a home row mod: Cmd/F + Ctrl/S nested -> Cmd+S.
TEST_F(TsahHrm, same_hand_cmd_s_on_home_row_mod) {
    TestDriver driver;
    InSequence s;
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    auto ctl_s = KeymapKey(0, 1, 1, LCTL_T(KC_S));
    set_keymap({gui_f, ctl_s});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    gui_f.press();
    idle_for(40);
    ctl_s.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_REPORT(driver, (KC_LGUI, KC_S));
    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_EMPTY_REPORT(driver);
    ctl_s.release();
    run_one_scan_loop();
    gui_f.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// A same-hand roll through an allow-listed letter still types letters:
// F down, W down, F up, W up -> "fw".
TEST_F(TsahHrm, same_hand_cmd_shortcut_roll_is_tap) {
    TestDriver driver;
    InSequence s;
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    auto w     = KeymapKey(0, 1, 0, KC_W);
    set_keymap({gui_f, w});

    idle_for(500);
    EXPECT_REPORT(driver, (KC_F));
    EXPECT_REPORT(driver, (KC_F, KC_W));
    EXPECT_REPORT(driver, (KC_W));
    EXPECT_EMPTY_REPORT(driver);
    gui_f.press();
    idle_for(40);
    w.press();
    idle_for(20);
    gui_f.release();
    idle_for(20);
    w.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Letters not on the allow-list keep the opposite-hand rule: Gui/F with E
// nested types "fe" (e.g. "few").
TEST_F(TsahHrm, same_hand_non_shortcut_nested_is_tap) {
    TestDriver driver;
    InSequence s;
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    auto e     = KeymapKey(0, 2, 0, KC_E);
    set_keymap({gui_f, e});

    idle_for(500);
    EXPECT_REPORT(driver, (KC_F));
    EXPECT_REPORT(driver, (KC_F, KC_E));
    EXPECT_REPORT(driver, (KC_F));
    EXPECT_EMPTY_REPORT(driver);
    gui_f.press();
    idle_for(40);
    tap_key(e);
    gui_f.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Same-hand roll inside the term stays a tap (Chordal Hold): "as", not Shift+S.
TEST_F(TsahHrm, same_hand_nested_is_tap) {
    TestDriver driver;
    InSequence s;
    auto sft_a = KeymapKey(0, 0, 1, LSFT_T(KC_A));
    auto d     = KeymapKey(0, 2, 1, KC_D);
    set_keymap({sft_a, d});

    idle_for(500);
    EXPECT_REPORT(driver, (KC_A));
    EXPECT_REPORT(driver, (KC_A, KC_D));
    EXPECT_REPORT(driver, (KC_A));
    EXPECT_EMPTY_REPORT(driver);
    sft_a.press();
    idle_for(40);
    tap_key(d);
    sft_a.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Two same-hand mods + opposite-hand key: Ctrl+Shift+N still works.
TEST_F(TsahHrm, same_hand_multi_mod_chord) {
    TestDriver driver;
    InSequence s;
    auto sft_a = KeymapKey(0, 0, 1, LSFT_T(KC_A));
    auto ctl_s = KeymapKey(0, 1, 1, LCTL_T(KC_S));
    auto n     = KeymapKey(0, 6, 1, KC_N);
    set_keymap({sft_a, ctl_s, n});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    sft_a.press();
    idle_for(20);
    ctl_s.press();
    idle_for(20);
    n.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LSFT));
    EXPECT_REPORT(driver, (KC_LSFT, KC_LCTL));
    EXPECT_REPORT(driver, (KC_LSFT, KC_LCTL, KC_N));
    EXPECT_REPORT(driver, (KC_LSFT, KC_LCTL));
    n.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LCTL));
    EXPECT_EMPTY_REPORT(driver);
    sft_a.release();
    run_one_scan_loop();
    ctl_s.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Opt keybind allow-list: right Opt/K with L nested (same hand) -> Opt+L.
TEST_F(TsahHrm, same_hand_opt_keybind_nested_is_hold) {
    TestDriver driver;
    InSequence s;
    auto alt_k = KeymapKey(0, 7, 1, RALT_T(KC_K));
    auto l     = KeymapKey(0, 8, 1, KC_L);
    set_keymap({alt_k, l});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    alt_k.press();
    idle_for(40);
    l.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_RALT));
    EXPECT_REPORT(driver, (KC_RALT, KC_L));
    EXPECT_REPORT(driver, (KC_RALT));
    EXPECT_EMPTY_REPORT(driver);
    l.release();
    run_one_scan_loop();
    alt_k.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Left Opt/D with Q nested (same hand) -> Opt+Q (close window).
TEST_F(TsahHrm, same_hand_left_opt_q_is_hold) {
    TestDriver driver;
    InSequence s;
    auto alt_d = KeymapKey(0, 2, 1, LALT_T(KC_D));
    auto q     = KeymapKey(0, 0, 0, KC_Q);
    set_keymap({alt_d, q});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    alt_d.press();
    idle_for(40);
    q.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LALT));
    EXPECT_REPORT(driver, (KC_LALT, KC_Q));
    EXPECT_REPORT(driver, (KC_LALT));
    EXPECT_EMPTY_REPORT(driver);
    q.release();
    run_one_scan_loop();
    alt_d.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Not on the Opt list (I): same-hand nested stays letters.
TEST_F(TsahHrm, same_hand_opt_non_keybind_is_tap) {
    TestDriver driver;
    InSequence s;
    auto alt_k = KeymapKey(0, 7, 1, RALT_T(KC_K));
    auto i     = KeymapKey(0, 7, 0, KC_I);
    set_keymap({alt_k, i});

    idle_for(500);
    EXPECT_REPORT(driver, (KC_K));
    EXPECT_REPORT(driver, (KC_K, KC_I));
    EXPECT_REPORT(driver, (KC_K));
    EXPECT_EMPTY_REPORT(driver);
    alt_k.press();
    idle_for(40);
    tap_key(i);
    alt_k.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Opt + Space (thumb, either hand) -> Opt+Space.
TEST_F(TsahHrm, opt_space_is_hold) {
    TestDriver driver;
    InSequence s;
    auto alt_d = KeymapKey(0, 2, 1, LALT_T(KC_D));
    auto spc   = KeymapKey(0, 2, 3, KC_SPC);
    set_keymap({alt_d, spc});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    alt_d.press();
    idle_for(40);
    spc.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LALT));
    EXPECT_REPORT(driver, (KC_LALT, KC_SPC));
    EXPECT_REPORT(driver, (KC_LALT));
    EXPECT_EMPTY_REPORT(driver);
    spc.release();
    run_one_scan_loop();
    alt_d.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// All right hand: Cmd/J + Opt/K + L nested -> Cmd+Opt+L (window snapping).
TEST_F(TsahHrm, same_hand_cmd_opt_l_chord) {
    TestDriver driver;
    InSequence s;
    auto gui_j = KeymapKey(0, 6, 1, RGUI_T(KC_J));
    auto alt_k = KeymapKey(0, 7, 1, RALT_T(KC_K));
    auto l     = KeymapKey(0, 8, 1, KC_L);
    set_keymap({gui_j, alt_k, l});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    gui_j.press();
    idle_for(20);
    alt_k.press();
    idle_for(20);
    l.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_RGUI));
    EXPECT_REPORT(driver, (KC_RGUI, KC_RALT));
    EXPECT_REPORT(driver, (KC_RGUI, KC_RALT, KC_L));
    EXPECT_REPORT(driver, (KC_RGUI, KC_RALT));
    l.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_RALT));
    EXPECT_EMPTY_REPORT(driver);
    gui_j.release();
    run_one_scan_loop();
    alt_k.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

// Left Cmd/F + left Opt/D + right-hand L -> Cmd+Opt+L (opposite hand).
TEST_F(TsahHrm, left_cmd_opt_right_l_chord) {
    TestDriver driver;
    InSequence s;
    auto gui_f = KeymapKey(0, 3, 1, LGUI_T(KC_F));
    auto alt_d = KeymapKey(0, 2, 1, LALT_T(KC_D));
    auto l     = KeymapKey(0, 8, 1, KC_L);
    set_keymap({gui_f, alt_d, l});

    idle_for(500);
    EXPECT_NO_REPORT(driver);
    gui_f.press();
    idle_for(20);
    alt_d.press();
    idle_for(20);
    l.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LGUI));
    EXPECT_REPORT(driver, (KC_LGUI, KC_LALT));
    EXPECT_REPORT(driver, (KC_LGUI, KC_LALT, KC_L));
    EXPECT_REPORT(driver, (KC_LGUI, KC_LALT));
    l.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_LALT));
    EXPECT_EMPTY_REPORT(driver);
    gui_f.release();
    run_one_scan_loop();
    alt_d.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}
