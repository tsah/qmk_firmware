# Home row mods + keymap review: tsah keymaps (3w6 Pro, Corne)

Branch `hrm-timeless-review`, based on `ea02f45a90`, pushed to the fork (not merged to master).

## TL;DR

- **No upstream rebase is needed.** This tree (upstream merged up to `b1aea25560`, 2026-09-26) already has
  Chordal Hold, Flow Tap (`FLOW_TAP_TERM`, `is_flow_tap_key()`, `get_flow_tap_term()`), Speculative Hold
  and all the per-key callbacks (`quantum/action_tapping.{c,h}`, `docs/tap_hold.md`).
- **The misfire to fix is a *missed* mod** (an intended mod comes out as a letter), not stray mods while
  typing. urob's full timeless recipe targets the opposite problem, so it was tried and then retuned (see
  §1). Final settings, shared by both boards from `users/tsah/`, which QMK includes automatically for any
  keymap named `tsah`:
  - tapping term **200** (unchanged)
  - Flow Tap **100 ms** (new)
  - Permissive Hold + Chordal Hold (kept)
  - **a same-hand Cmd shortcut allow-list** (new)
  - a Shift exemption after Space (new)
- **The behaviour is unit-tested** against QMK's tap-hold test harness (`make test:tsah_hrm`, 11 scenarios,
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
| `5fd5b58fac` | **Retune for missed mods**: term back to 200, Flow Tap 100, Cmd same-hand allow-list (`get_chordal_hold`). |
| `b83dc0b0aa` | Tests for the allow-list and the 100 ms Flow Tap. |

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

### How that maps onto this QMK tree (first attempt, commit `3ee9d288c1`)

> Superseded for the term values: see **Retune** below. The final values are term 200 and Flow Tap 100.

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
the stray-mod misfire. That turned out *not* to be the misfire tsah gets, hence the retune.

**Flow Tap details (`users/tsah/tsah.c`):**

- `is_flow_tap_key()` is QMK's default list (A–Z, `,` `.` `;` `/`, Space) **plus `'`**, because `'` is a
  letter-area key on Sturdy. As in the default, Flow Tap switches off while Ctrl, Gui or left Alt are held,
  so a second mod can be added to a hotkey quickly.
- `get_flow_tap_term()` makes one exception: **the Shift HRMs are exempt when the previous key was
  Space.** Without it, a quick capital at the start of a word (`space`, then hold Shift+letter within the
  Flow Tap term) would type a lowercase letter. That is the most common complaint about prior-idle setups. Inside
  words, where the lingering-pinky Shift misfires happen, Flow Tap still applies. After Space, behaviour is
  the same as before this change, so nothing regresses.
- A small difference from ZMK: QMK's Flow Tap also counts key *releases* as activity, so it is a bit
  stricter (it suppresses slightly more holds) than ZMK's prior-idle.

### Retune: the reported misfire is the opposite one

After the timeless commit, tsah clarified that the misfires are **intended mods coming out as letters**.
They're on macOS, and unsure whether the failing shortcuts are same-hand or opposite-hand. With
Permissive + Chordal Hold, an intended mod becomes a letter in three ways:

1. **The mod and the letter are on the same hand** (Cmd on F + W). Chordal Hold settles as tap the
   moment W is *pressed*, unless F has been held for the full term. On a Mac most shortcut letters are
   on the left hand, so this is the most likely cause.
2. **The mod is released a hair before the letter** (Cmd on J, then C, J let go first). Permissive Hold
   needs the letter pressed *and released* inside the mod, so this reads as the roll "jc".
3. **A shortcut fired immediately after typing**, which Flow Tap turns into a letter.

The timeless commit made 1 slower (280 ms) and created 3 (150 ms), so it was retuned:

| Setting | Timeless commit | Now | Why |
|---|---|---|---|
| `TAPPING_TERM` | 280 | **200** | A long term only helps against stray mods. It makes same-hand shortcuts wait longer. |
| `FLOW_TAP_TERM` | 150 | **100** | Still forces letters during genuinely fast typing. A deliberate shortcut (normally after a short pause) almost never lands within 100 ms. |
| Same-hand Cmd chords | always a letter | **Cmd+letter if nested** | `get_chordal_hold()` allows Cmd (either Gui mod-tap) + A C F L N O P Q R S T V W X Z when the letter is pressed and released inside the Cmd hold. A roll (Cmd released first) still types both letters. Other same-hand letters (e.g. F+E in "few") keep the opposite-hand rule. The list is keycode-based, so it works on both QWERTY and Sturdy. |
| Permissive + Chordal Hold | on | on | Unchanged. |

Cause 2 (releasing the mod early) is not addressed yet, on purpose. The fix is `HOLD_ON_OTHER_KEY_PRESS`
(an opposite-hand *press* triggers the mod immediately), but it brings back stray mods on cross-hand rolls
whenever Flow Tap isn't active. It is step 2 of the tuning plan, to try only if missed mods continue.

### Later changes from the recorder and the Opt keybinds (2026-11-23)

- **Shift is exempt from Flow Tap entirely** (commit `23e2a17593`). On the first day the recorder caught
  Flow Tap eating intended Shifts: "mac?" came out as "maca/". Shift was pressed 97 ms after a letter, with
  "/" nested inside a 212 ms hold. Another Shift held 560 ms was also typed as a letter. Flow Tap decides at
  press time, so no amount of holding rescues it. tsah uses Shift straight after letters all the time
  (punctuation, camelCase).
- **Opt same-hand allow-list** (commit `253a2b982a`). The Mac global keybinds moved from Ctrl+Opt to
  Option alone (Hammerspoon, set up by the mac-manager session; the list is in `users/tsah/opt-keybinds.txt`):
  H J K L, X O, 1–5, P W U. A nested press of one of these inside the Opt home row key counts as Opt even
  on the same hand, the same way as the Cmd list.
- **Recorder** (`users/tsah/hrm_log/`, see its README). The 3w6 Pro runs the `TSAH_HRM_LOG=yes` build until
  the analysis (results below). The logs have been deleted and the recorder uninstalled.
- **Corne:** it gets all of the above through `users/tsah`, and both its home row keycodes and its
  handedness layout match the 3w6. Flash it from home with `keyboards/crkbd/keymaps/tsah/flash.sh`
  (normal build, no recorder). Boot key: Q (left) or P (right), the top-row key second from the outside.

### Recorder results (2026-11-23 to 11-24): no further change

The 3w6 Pro recorded about 46,000 key events over 1.5 working days (6 sessions). The logs were deleted
afterwards. Only the aggregate numbers below were kept. The counts after the Shift exemption (sessions 4–5,
about 44,000 events, roughly 4,700 home row key presses) are what matter:

- **Missed mods** (letter, then corrected, then retried as a mod): **10**, about 1 per 470 home row presses.
  - 8 are left Shift. In every one, Shift was released **before** the shifted letter: a roll, not a nested
    press. Shift went down, the letter went down about 99 ms later (median), and the two keys overlapped
    for only about 14 ms (median).
  - 2 are right Ctrl.
  - The analyser files most of them under "Flow Tap" only because the gap from the previous key was under
    100 ms. Shift is exempt from Flow Tap in that build, so the real cause is the roll.
- **Those 8 rolls look exactly like normal typing.** 130 opposite-hand Shift rolls in the same period were
  correctly letters. Their timing was the same: the letter came about 82 ms after Shift, with about 24 ms of
  overlap.
  - Any rule that turns the 8 into Shift (`HOLD_ON_OTHER_KEY_PRESS` for Shift, or a shorter tapping term)
    turns most of the 130 into stray capitals.
  - Across all mods, `HOLD_ON_OTHER_KEY_PRESS` would have changed 551 taps: 190 were followed by some
    correction and 361 were not. For Cmd only, the split was 30 / 44.
  - It is not enabled.
- **Cmd and Opt allow-lists:** no missed same-hand shortcuts were recorded, so the lists stay as they are.
- **Stray mods:** the analyser flagged 114 "mod, then Backspace within 2 s". Most are Shift (65), which is
  ordinary capital-letter-then-typo editing rather than wrong decisions. No key stood out, so there is no
  change there either.
- **Decision:** keep the settings as they are (tapping term 200, Flow Tap 100 with Shift exempt, Permissive +
  Chordal Hold, the Cmd and Opt allow-lists).
  - The remaining Shift misses are a habit issue: keep Shift down until the letter is up.
  - Caps Word (NAV + G) covers runs of capitals.
  - If they keep bothering you, the next step is a layout change, not timing: Shift on a thumb key, or a
    one-shot Shift.

### Verified behaviour (`make test:tsah_hrm`)

| Scenario | Result |
|---|---|
| `n`, then 50 ms later Shift/A with a right-hand key nested inside it | tap: `a`, `n` (Flow Tap) |
| Space, then 50 ms later Shift/A + nested right-hand key | **Shift+N** (capitals still work) |
| Space, then 50 ms later Gui/F + nested key | tap (the exemption is Shift-only) |
| Idle, then Gui/F + nested right-hand key at 40 ms | Cmd+N (Permissive Hold) |
| `e`, then 120 ms later Gui/F + nested N | **Cmd+N** (outside the 100 ms Flow Tap) |
| Gui/F + W nested (same hand) | **Cmd+W** (allow-list) |
| Gui/F + Ctrl/S nested (same hand, S is itself a home row mod) | **Cmd+S** |
| Gui/F down, W down, F up, W up (roll) | `fw` |
| Gui/F + E nested (same hand, not a shortcut letter) | `fe` |
| Shift/A with a same-hand key nested | tap (Chordal Hold) |
| Shift/A + Ctrl/S + right-hand N | Ctrl+Shift+N (same-hand multi-mod) |

### Trade-offs to be aware of

- **The allow-list re-opens a small stray-mod risk:** a slow word start like "fr…"/"fa…" where F stays down
  through the whole R/A press becomes Cmd+R/Cmd+A. Flow Tap covers it during normal-speed typing. If it
  happens, remove that letter from `is_cmd_shortcut_letter()` in `users/tsah/tsah.c`.
- **Same-hand shortcuts with Ctrl/Alt/Shift** (not Cmd) still need the full 200 ms hold, or the opposite
  hand's mod.
- **Mod + real-mouse click** waits up to 200 ms (as before). `SPECULATIVE_HOLD` would remove that.

## 2. Tuning plan

Change one thing at a time, in `users/tsah/config.h` / `tsah.c` (both boards follow), and give each change
a day or two. Re-run `make test:tsah_hrm` after editing. A failing test shows you which behaviour moved.

**If intended mods still come out as letters (the current problem):**

| Situation | Change |
|---|---|
| A same-hand Cmd shortcut whose letter isn't on the list | Add the letter to `is_cmd_shortcut_letter()` |
| Same-hand shortcuts with Ctrl/Alt/Shift | Extend `get_chordal_hold()` the same way for those mods; or lower `TAPPING_TERM` to 175 |
| Opposite-hand shortcuts fail, especially fast ones (cause 2: the mod is released first) | **Step 2:** `#define HOLD_ON_OTHER_KEY_PRESS`. With Chordal Hold it applies to opposite-hand presses only. Possibly only for Cmd: `HOLD_ON_OTHER_KEY_PRESS_PER_KEY` + `get_hold_on_other_key_press()` returning true for Gui mod-taps |
| Shortcuts right after typing turn into letters | `FLOW_TAP_TERM` **down**, 100 → 75, or remove it |

**If stray mods appear while typing (the opposite problem, e.g. after step 2):**

| Situation | Change |
|---|---|
| During fast typing (rolls) | `FLOW_TAP_TERM` **up**, 100 → 125 → 150 |
| When typing slowly, or a key lingers | `TAPPING_TERM` **up**, 200 → 250 |
| One specific same-hand Cmd+letter fires while typing a word | Remove that letter from the allow-list |
| Mostly on the pinky Shift keys | Per-key: a higher Flow Tap term for Shift in `get_flow_tap_term()` |
| Shift/Cmd + mouse click lags | `#define SPECULATIVE_HOLD` (+ `SPECULATIVE_HOLD_ONE_KEY` if an app reacts to lone Shift taps) |

To see what is actually happening, add `CONSOLE_ENABLE = yes` to `users/tsah/rules.mk`, call
`debug_enable = true` in `keyboard_post_init_user`, and run `qmk console`. Flow Tap logs
`within flow tap term ... considered a tap`, and Chordal Hold logs its decisions.

---

## 3. Keymap review (prioritised)

**Decisions (2026-11-23, with tsah):**

| Item | Decision |
|---|---|
| Plain mods on NAV | **Dropped.** tsah's habit is "Shift, then arrow" (home row mod first, then NAV). That now works reliably: Shift is out of Flow Tap, and the thumbs count as either hand. |
| Persistent base layer (`PDF`) | **Dropped.** Sturdy is abandoned for now; the layer stays for a later retry. |
| Shifted symbols via two thumbs | **Kept as is.** Two thumbs, then the number pad, is a comfortable chord. |
| `QK_BOOT` | **Done:** FUN + Z position (commit "add Caps Word on NAV and a bootloader key on FUN"). |
| Shared layers | **Done:** `users/tsah/tsah.h`. Each keymap has a `LAYOUT_tsah()` wrapper. The compiled 3w6 keymap is byte-identical; the Corne differs only in its former drift. |
| Caps Word | **Done:** NAV + G position. |
| Esc/Tab placement | **Kept** on NUM's home row. |
| Opt-only Mac keybinds (instead of Ctrl+Opt "hyper") | **Done:** same-hand Opt allow-list (H J K L O P U W X, 1–5). |

The original recommendations follow for reference.

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

- 2026-11-26: Corne (`crkbd/rev4_1/standard`), normal build from `a3134d6eac`, left then right half.
