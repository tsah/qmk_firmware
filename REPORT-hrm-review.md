# Home row mods + keymap review: tsah keymaps (3w6 Pro, Corne)

Branch `hrm-timeless-review`, based on `ea02f45a90`. Nothing has been pushed.

## TL;DR

- **No upstream rebase is needed.** This tree (upstream merged up to `b1aea25560`, 2026-09-26) already has
  Chordal Hold, Flow Tap (`FLOW_TAP_TERM`, `is_flow_tap_key()`, `get_flow_tap_term()`), Speculative Hold
  and all the per-key callbacks (`quantum/action_tapping.{c,h}`, `docs/tap_hold.md`).
- **Timeless HRMs are now on for both boards**, from one shared place: `users/tsah/`. QMK includes it
  automatically for any keymap named `tsah`. The settings are tapping term 280, Flow Tap 150, Permissive
  Hold and Chordal Hold, plus a Shift exemption after Space.
- **The behaviour is unit-tested** against QMK's tap-hold test harness (`make test:tsah_hrm`, 7 scenarios,
  all pass).
- **`flash.sh` works on macOS and Linux.** It is one shared script; the per-keymap `flash.sh` files are
  now thin wrappers.
- The keymap review is in the second half, prioritised. Nothing in it is applied except the HRM tuning.

## Commits

| Commit | What |
|---|---|
| `a0cb9795b1` | Move the duplicated tap-hold and mouse key config into `users/tsah/config.h`. No behaviour change. |
| `3ee9d288c1` | Timeless HRMs: `TAPPING_TERM 280`, `FLOW_TAP_TERM 150`, Flow Tap callbacks in `users/tsah/tsah.c`. |
| `54689e4b89` | `tests/tsah_hrm/`: unit tests that build the real `users/tsah` config and code. |
| `a7e752cc7f` | Portable `users/tsah/flash.sh`; the keymap `flash.sh` files become wrappers. |

Both `keebart/3w6_pro:tsah` and `crkbd/rev4_1/standard:tsah` were compiled after every commit and build
cleanly.

---

## 1. Timeless home row mods

### What "timeless" is (urob's ZMK config)

urob's goal is HRMs that almost never misfire and that don't depend on precise timing. Four ingredients:

1. **`flavor = "balanced"`.** A hold is decided by key *order*, not time. If another key is pressed *and
   released* while the HRM is down (a nested press), the HRM is a hold. If the HRM is released first (a
   roll), it is a tap.
2. **Positional hold-tap with `hold-trigger-on-release`.** Only keys on the *opposite* hand (and thumbs)
   can turn an HRM into a hold. Checking that on release lets several same-hand mods be stacked.
3. **`require-prior-idle-ms = 150`.** If the HRM is pressed less than 150 ms after the previous key, it is
   a plain tap, full stop. This kills misfires during fast typing, which is where nearly all of them
   happen.
4. **A long `tapping-term-ms = 280`.** Because 1–3 decide the real cases by key order, the term only
   matters when a key is held *alone*. A long term means a slow or lingering tap still types its letter.
   Also `quick-tap-ms = 175`, so a tap followed by a hold auto-repeats.

### How that maps onto this QMK tree

| ZMK (urob) | QMK here | Status |
|---|---|---|
| balanced flavor | `PERMISSIVE_HOLD` | already on, kept |
| positional hold-tap, thumbs as triggers | `CHORDAL_HOLD` + `chordal_hold_layout` (thumbs `'*'`) | already on, kept (layouts checked against both matrices) |
| hold-trigger-on-release (stack same-hand mods) | Built into Chordal Hold: several same-hand mod-taps may chord | works, unit-tested |
| require-prior-idle 150 ms | `FLOW_TAP_TERM 150` + callbacks | **new** |
| tapping term 280 | `TAPPING_TERM 280` (was 200) | **new** |
| quick-tap 175 | `QUICK_TAP_TERM 175` | unchanged |

**Why `PERMISSIVE_HOLD` and not `HOLD_ON_OTHER_KEY_PRESS`.** With Hold On Other Key Press, an HRM becomes
a hold as soon as *any* opposite-hand key is pressed. That includes ordinary cross-hand rolls such as
`f`→`j` or `a`→`n`. Chordal Hold only filters same-hand rolls, so cross-hand rolls would misfire. Permissive
Hold needs the other key to be released first (nested), which a roll doesn't do. Permissive Hold is the
balanced flavor. Keep it.

**Why the tapping term goes up, not down.** The intuition is "shorter term = snappier mods". With
Permissive + Chordal Hold, intentional opposite-hand shortcuts already fire on the nested release, well
inside the term. A shorter term only makes lingering taps (pinkies especially) turn into holds, which is
exactly the misfire being fixed. The costs of 280 are listed under "Trade-offs" below.

**Flow Tap details (`users/tsah/tsah.c`):**

- `is_flow_tap_key()` is QMK's default list (A–Z, `,` `.` `;` `/`, Space) **plus `'`**, because `'` is a
  letter-area key on Sturdy. As in the default, Flow Tap switches off while Ctrl, Gui or left Alt are held,
  so a second mod can be added to a hotkey quickly.
- `get_flow_tap_term()` makes one exception: **the Shift HRMs are exempt when the previous key was
  Space.** Without it, a quick capital at the start of a word (`space`, then hold Shift+letter within
  150 ms) would type a lowercase letter. That is the most common complaint about prior-idle setups. Inside
  words, where the lingering-pinky Shift misfires happen, Flow Tap still applies. After Space, behaviour is
  the same as before this change, so nothing regresses.
- A small difference from ZMK: QMK's Flow Tap also counts key *releases* as activity, so it is a bit
  stricter (it suppresses slightly more holds) than ZMK's prior-idle.

### Verified behaviour (`make test:tsah_hrm`)

| Scenario | Result |
|---|---|
| `n`, then 50 ms later Shift/A with a right-hand key nested inside it | tap: `a`, `n` (was Shift+N) |
| Space, then 50 ms later Shift/A + nested right-hand key | **Shift+N** (capitals still work) |
| Space, then 50 ms later Gui/F + nested key | tap (the exemption is Shift-only) |
| Idle, then Gui/F + nested right-hand key at 40 ms | Gui+N (Permissive Hold) |
| Shift/A held alone for 250 ms, released | types `a` (was a dead Shift at 200 ms) |
| Shift/A with a same-hand key nested | tap (Chordal Hold) |
| Shift/A + Ctrl/S + right-hand N | Ctrl+Shift+N (same-hand multi-mod) |

### Trade-offs to be aware of

- **Same-hand shortcuts are slower.** Chordal Hold makes Gui/F + `W` (same hand) a tap unless F has been
  held for the whole tapping term, now 280 ms instead of 200 ms. Habit fix: use the opposite hand's mod
  (right-hand Cmd/J + W/Q/T/C/V on the left). This is how timeless HRMs are meant to be used.
- **Mod + real-mouse click is laggier.** With no other key pressed, the mod only registers after 280 ms. If
  Shift/Cmd+click feels late, enable `SPECULATIVE_HOLD` (see the tuning plan).
- **Same-hand capitals** (for example Shift on `;` + `I`) need the opposite-hand Shift, or a full-term
  hold. This was already true with Chordal Hold.

## 2. Tuning plan

Change one thing at a time, in `users/tsah/config.h` / `tsah.c` (both boards follow), and give each change
a day or two. Re-run `make test:tsah_hrm` after editing. Most tests are written relative to the 150/280
values, so a failing test shows you which behaviour moved.

| Symptom | Adjust | Direction |
|---|---|---|
| Unwanted mods **while typing fast** (rolls) | `FLOW_TAP_TERM` | **up**, 150 → 175 → 200 |
| Unwanted mods when typing **slowly** / a key lingers | `TAPPING_TERM` | **up**, 280 → 300 |
| Intended mod comes out as a letter **right after typing** (e.g. Cmd+S just after a word) | `FLOW_TAP_TERM` | **down**, 150 → 125 → 100; or pause briefly before shortcuts |
| Same-hand shortcuts (Cmd+W with Cmd on F) feel sluggish | `TAPPING_TERM` | **down** to 250 (QMK's recommended start); better, use the opposite hand |
| Capital letters lost at the start of words | Check the Space exemption; consider also exempting `KC_ENT` / `KC_TAB` as previous keys in `get_flow_tap_term()` | — |
| Lowercase-instead-of-capital *mid-sentence* after punctuation (e.g. `."`) | Add those keycodes as previous keys to the Shift exemption | — |
| Shift misfires persist on the pinkies specifically | Per-key: give the Shift mod-taps a higher Flow Tap term (`get_flow_tap_term`, e.g. +25 ms), or enable `TAPPING_TERM_PER_KEY` and give the pinkies +30 ms | up, pinkies only |
| Gui (index) misfires: index fingers roll the most | Per-key Flow Tap term +25 ms on `LGUI_T`/`RGUI_T` | up, index only |
| Shift/Cmd + mouse click lags | `#define SPECULATIVE_HOLD` (Shift/Ctrl by default; extend in `get_speculative_hold()`). Add `SPECULATIVE_HOLD_ONE_KEY` if apps react to lone Shift taps (e.g. IntelliJ double-Shift) | — |
| Quick tap-then-hold gives auto-repeat when you wanted a mod | `QUICK_TAP_TERM` | **down**, 175 → 120 |

To see what is actually happening, add `CONSOLE_ENABLE = yes` to `users/tsah/rules.mk`, call
`debug_enable = true` in `keyboard_post_init_user`, and run `qmk console`. Flow Tap logs
`within flow tap term ... considered a tap`, and Chordal Hold logs its decisions.

---

## 3. Keymap review (prioritised)

The current design is clean: a dedicated thumb key per layer, no tri-layer, and both boards nearly
identical. Most of the items below are about the few places where HRMs and layers interact, and about
reducing duplication.

### P1: Put plain modifiers on the NAV layer's left home row (Callum-style)

**Now:** on NAV (right thumb), the left home row is `-` `+` `_` `=`. To select text with Shift+arrows, or
to jump by word or line with Alt/Cmd+arrows, you must hold the HRM *first* and then NAV, in that order.
That is order-dependent, goes through tap-hold resolution, and is exactly the kind of chord that misfires.

**Proposal:** NAV left home row = `KC_LSFT KC_LCTL KC_LALT KC_LGUI` (same order as the HRMs). Move
`- + _ =` up one row to NAV's left top row, which is entirely empty today, so they stay in the same
columns. These are plain modifiers, not tap-holds, so there is no timing at all: hold NAV, hold
Shift/Alt/Cmd, press arrows.

**Trade-off:** `- + _ =` move up one row (a few days of retraining). In return, the most common editing
chords stop depending on tap-hold, so they can't misfire.

### P1: Make the base-layer switch persistent

`TO(_STURDY)` on FUN is lost on every replug, and the board comes back up in QWERTY. Use
`PDF(_QWERTY)` / `PDF(_STURDY)` (persistent default layer, supported in this tree) to store the choice in
EEPROM. The layer order stays the same.
**Trade-off:** none in practice. It's an EEPROM write only when you switch.

### P2: Shifted number-row symbols cost a three-key chord

`!@#$%^&*` are NUM (left thumb) + Shift on the NAV thumb + a number key. For `@ # $ & *` (code, email,
shell) that's awkward. Options, cheapest first:

- Put the most-used ones directly on NAV's free bottom row (all 10 NAV bottom slots except PScr are
  empty).
- Or add a SYM layer that mirrors the number pad positions with shifted symbols, reached from the NAV
  thumb + NUM thumb (this re-introduces a tri-layer, which the current design avoids on purpose).

**Trade-off:** NAV gets denser. The tri-layer option breaks the "one thumb per layer" rule.

### P2: Add `QK_BOOT` to the FUN layer

There is no bootloader key on either board, so every flash needs unplugging or bootmagic. Put `QK_BOOT` on
FUN's left bottom-left pinky slot (empty on both boards).
**Caveat:** `QK_BOOT` reboots only the half that runs the keymap (the USB-connected one), so the other half
still needs bootmagic or the reset button. It's still half the physical steps. It sits behind a held
thumb, so accidental triggers are unlikely.

### P3: Share the layers themselves, not just the config

`users/tsah/` now holds the shared tap-hold config, the Flow Tap code and the flash script. The six layers
are still written out twice: the 3w6 Pro as readable 3×5 grids, the Corne as one-line
`LAYOUT_split_3x6_3_ex2(...)` calls. They have already drifted a little: the Corne's NUM bottom-left has
no Vol−/Vol+ and its NAV has no PScr, because those live on the Corne's extra keys.

**Proposal:** `users/tsah/tsah.h` defines the 36-key layers once, as row macros (`TSAH_QWERTY_L1`, …), with
the `enum layers` and `LA_*` aliases. Each keymap supplies a `LAYOUT_tsah(...)` wrapper:

- 3w6 Pro: `LAYOUT_split_3x5_3` directly.
- Corne: `LAYOUT_split_3x5_3_ex2` (the outer columns are `KC_NO` today anyway, so nothing is lost), plus
  the 4 extra keys (Vol−/Vol+/Mute/PScr) per layer.

**Trade-offs:** macro-based keymaps are harder to read and diff than the explicit 3w6 grids. The
`tsah_layouts.html` visualizer is hand-maintained data, so it still has to be updated by hand either way.
The benefit is that a layer change is made once, and the Corne also gains the layer copies of Vol/PScr.
Worth doing before the next layout change, not urgent. The Ferris shown in the visualizer would also
benefit if it gets a `tsah` keymap in this tree.

### P3: Smaller things

- **Comment mismatch:** the thumb comment says "Hold NUM, *tap* right mid = Shift", but it is a held
  `KC_LSFT`. Change it to "hold".
- **Esc lives on NUM** (layer + middle finger). If you use vim or modal editors heavily, consider an Esc
  combo on two non-HRM keys (e.g. `W`+`E`). That adds combo timing, so only if Esc reach bothers you.
- **Caps Word** (`CAPS_WORD_ENABLE`) would help typing CONSTANTS without holding a pinky Shift. Put it on
  FUN or NUM rather than `BOTH_SHIFTS_TURNS_ON_CAPS_WORD`, which would need two HRM holds.
- **Mouse layer:** wheel-right sits on the pinky home key and wheel-left on the bottom index key. An odd
  asymmetry, but harmless if you rarely scroll horizontally.
- **HRM order (Shift on pinky):** pinkies linger the most, which is why Shift misfires are the classic
  symptom. The Flow Tap Shift tuning targets this. If they persist after tuning, moving Shift to the index
  finger (current order pinky→index is Shift-Ctrl-Alt-Gui; flip to Gui-Alt-Ctrl-Shift) is the structural fix, but it costs relearning.
- **Thumb tap-holds:** don't add `LT()` on the thumbs to get Backspace/Tab without a layer. It would
  bring tap-hold misfires to the thumbs, the opposite of this branch's goal.

---

## 4. Flashing on macOS

`users/tsah/flash.sh <keyboard> [left|right|both] [--no-compile]`. Each keymap's `flash.sh` calls it with
its board.

- **macOS:** waits for `/Volumes/RPI-RP2` (checking for `INFO_UF2.TXT`), copies with `cp -X` (some macOS
  versions fail to write extended attributes to the RP2040 boot drive), then waits for the drive to eject
  itself. If `cp` reports an error but the drive has already ejected, it treats the flash as done, because
  the RP2040 reboots on the last block.
- **Linux:** the previous `lsblk`/`udisksctl` logic, unchanged.
- Boot-mode hints are per board. The 3w6 Pro's bootmagic keys are matrix `[0,0]` (Q) and `[4,4]` (P); the
  Corne's are `[0,1]` / `[4,1]`, which are also Q/P.
- The alternative is `qmk flash -kb <kb> -km tsah`, once per half.

## 5. Flash log

(The 3w6 Pro flash with the new firmware is recorded below once done.)
