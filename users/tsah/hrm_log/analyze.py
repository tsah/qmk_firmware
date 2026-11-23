#!/usr/bin/env python3
"""Analyse tsah home row mod logs recorded by hrm_log.c + logger.py.

Usage: analyze.py [--flow-term 100] [--tapping-term 200] ~/hrm-logs/*.log

Finds home row mod (HRM) decisions that were probably wrong, using what you
did next:
  missed mod  = HRM settled as a letter while another key was pressed inside it,
                then Backspace/Delete/Cmd+Z soon after, then the same HRM used
                as a mod ("fail, delete, retry")
  stray mod   = HRM settled as a mod, then Backspace/Cmd+Z soon after
and attributes each missed mod to a cause, plus "what if" counts for the
candidate fixes.
"""
import argparse
import collections
import statistics

CORRECTION_MS = 2000  # a correction must start this soon after the HRM release
RETRY_MS = 6000  # the retry (same HRM as a mod) must come this soon after
HID_LETTERS = {0x04 + i: chr(ord('A') + i) for i in range(26)}
MOD_BITS = {'ctl': 0x01, 'sft': 0x02, 'alt': 0x04, 'gui': 0x08}


class Ev:
    __slots__ = ('t', 'pressed', 'id', 'kind', 'hand', 'mod', 'decision', 'token', 'mods', 'release')

    def __init__(self, f):
        # f: HRM t P|R id kind hand ...
        self.t = int(f[1])
        self.pressed = f[2] == 'P'
        self.id = int(f[3])
        self.kind = f[4]
        self.hand = f[5]
        self.mod = self.decision = self.token = None
        if self.kind == 'mt':
            self.mod, self.decision, self.mods = f[6], f[7], int(f[9], 16)
        else:
            self.token, self.mods = f[6], int(f[7], 16)
        self.release = None

    @property
    def name(self):
        return f'{self.hand}-{self.mod}'

    def is_correction(self):
        if self.kind != 'k' or not self.pressed:
            return False
        if self.token in ('bspc', 'del'):
            return True
        return self.mods & 0x09 and self.token == '001D'  # Cmd/Ctrl+Z

    def shortcut_letter(self):
        try:
            return HID_LETTERS.get(int(self.token, 16) & 0xFF)
        except (TypeError, ValueError):
            return None


def read_sessions(paths):
    """Split into sessions wherever the keyboard's timer restarts."""
    sessions, cur, last_t = [], [], None
    for path in paths:
        with open(path) as fh:
            for line in fh:
                parts = line.split()
                if len(parts) < 9 or parts[1] != 'HRM':
                    continue
                try:
                    ev = Ev(parts[1:])
                except (ValueError, IndexError):
                    continue
                if last_t is not None and ev.t < last_t - 1000:
                    sessions.append(cur)
                    cur = []
                cur.append(ev)
                last_t = ev.t
    if cur:
        sessions.append(cur)
    return sessions


def pair_releases(events):
    open_presses = {}
    for ev in events:
        if ev.pressed:
            open_presses[ev.id] = ev
        elif ev.id in open_presses:
            open_presses.pop(ev.id).release = ev.t


def analyse(sessions, flow_term, tapping_term):
    stats = collections.Counter()
    hold_ms = collections.defaultdict(list)
    missed = []  # (cause, detail dict)
    stray = []
    roll_taps = []  # every tap where an opposite-hand key overlapped but the HRM was released first

    for events in sessions:
        events.sort(key=lambda e: e.t)
        pair_releases(events)
        for i, ev in enumerate(events):
            if ev.kind != 'mt' or not ev.pressed or ev.release is None:
                continue
            t0, t1 = ev.t, ev.release
            stats[f'{ev.name} {ev.decision}'] += 1
            hold_ms[f'{ev.name} {ev.decision}'].append(t1 - t0)
            prev_gap = t0 - events[i - 1].t if i > 0 else 10**6
            inside = [e for e in events[i + 1 :] if e.pressed and t0 < e.t < t1]
            after = [e for e in events[i + 1 :] if e.t >= t1 and e.t - t1 <= RETRY_MS]
            corr = next((e for e in after if e.is_correction() and e.t - t1 <= CORRECTION_MS), None)

            if ev.decision == 'hold':
                if corr is not None:
                    stray.append({'hrm': ev.name, 'prev_gap': prev_gap, 'held': t1 - t0})
                continue
            if not inside:
                continue
            other = inside[0]
            nested = other.release is not None and other.release < t1
            same_hand = other.hand == ev.hand
            if not same_hand and not nested:
                roll_taps.append({'hrm': ev.name, 'corrected': corr is not None, 'early': (other.release or t1) - t1, 'overlap': t1 - other.t})
            if corr is None:
                continue
            retry = next((e for e in after if e.t > corr.t and e.kind == 'mt' and e.pressed and e.name == ev.name and e.decision == 'hold'), None)
            if retry is None:
                continue
            letter = next((e.shortcut_letter() for e in after if e.t > retry.t and e.kind == 'k' and e.pressed and e.shortcut_letter()), None)
            if prev_gap < flow_term:
                cause = 'flow tap (pressed too soon after typing)'
            elif same_hand:
                cause = 'same-hand chord'
            elif not nested:
                cause = 'mod released before the other key (roll)'
            else:
                cause = 'other'
            missed.append((cause, {'hrm': ev.name, 'prev_gap': prev_gap, 'held': t1 - t0, 'letter': letter, 'early': (other.release or t1) - t1}))
    return stats, hold_ms, missed, stray, roll_taps


def pct(a, b):
    return f'{100 * a / b:.0f}%' if b else '-'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--flow-term', type=int, default=100)
    ap.add_argument('--tapping-term', type=int, default=200)
    ap.add_argument('logs', nargs='+')
    a = ap.parse_args()

    sessions = read_sessions(a.logs)
    n_events = sum(len(s) for s in sessions)
    stats, hold_ms, missed, stray, roll_taps = analyse(sessions, a.flow_term, a.tapping_term)

    print(f'{n_events} key events in {len(sessions)} session(s)\n')
    print('Home row key decisions (count, median held ms):')
    for key in sorted(stats):
        print(f'  {key:<12} {stats[key]:>6}  {statistics.median(hold_ms[key]):>5.0f} ms')

    print(f'\nLikely MISSED mods (letter, corrected, then retried as a mod): {len(missed)}')
    by_cause = collections.Counter(c for c, _ in missed)
    for cause, n in by_cause.most_common():
        print(f'  {n:>4}  {cause}')
        rows = [d for c, d in missed if c == cause]
        by_hrm = collections.Counter(d['hrm'] for d in rows)
        print('        by key: ' + ', '.join(f'{k} {v}' for k, v in by_hrm.most_common()))
        letters = collections.Counter(d['letter'] for d in rows if d['letter'])
        if letters:
            print('        retried shortcut: ' + ', '.join(f'{k} {v}' for k, v in letters.most_common()))
        if cause.startswith('flow'):
            print(f"        gap after previous key: median {statistics.median(d['prev_gap'] for d in rows):.0f} ms")
        if cause.startswith('mod released'):
            print(f"        other key released after the mod by: median {statistics.median(d['early'] for d in rows):.0f} ms")

    print(f'\nPossible STRAY mods (mod, then corrected within {CORRECTION_MS} ms): {len(stray)}')
    for k, v in collections.Counter(d['hrm'] for d in stray).most_common():
        print(f'  {k} {v}')

    corrected_rolls = sum(r['corrected'] for r in roll_taps)
    print('\nWhat if HOLD_ON_OTHER_KEY_PRESS (opposite-hand press = mod at once)?')
    print(f'  {len(roll_taps)} taps had an opposite-hand key pressed inside them with the HRM released first.')
    print(f'  {corrected_rolls} of those were corrected (would be FIXED), {len(roll_taps) - corrected_rolls} were not (would become NEW stray mods, unless Flow Tap catches them).')
    gui_rolls = [r for r in roll_taps if r['hrm'].endswith('gui')]
    print(f"  Cmd-only variant: {sum(r['corrected'] for r in gui_rolls)} fixed / {sum(not r['corrected'] for r in gui_rolls)} new strays.")


if __name__ == '__main__':
    main()
