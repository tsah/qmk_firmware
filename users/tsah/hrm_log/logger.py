#!/usr/bin/env python3
"""Background recorder for the tsah home row mod log (run by launchd).

Runs `qmk console`, keeps only the keyboard's "HRM ..." lines and appends them,
prefixed with the wall-clock time, to <log-dir>/hrm-YYYY-MM-DD.log.

Two deadlines make it self-limiting:
  --record-until  stop recording (kill qmk console) at this Unix time
  --purge-at      delete the logs, the launchd job and this script at this
                  Unix time, unless they were removed earlier by hand
"""
import argparse
import os
import re
import shutil
import signal
import subprocess
import threading
import time

ANSI = re.compile(r'\x1b\[[0-9;]*m')


def purge(args):
    shutil.rmtree(args.log_dir, ignore_errors=True)
    shutil.rmtree(args.install_dir, ignore_errors=True)
    if os.path.exists(args.plist):
        os.remove(args.plist)
    # Unload ourselves last; this kills this process.
    subprocess.run(['launchctl', 'bootout', f'gui/{os.getuid()}/{args.label}'])


def record(args):
    os.makedirs(args.log_dir, exist_ok=True)
    proc = subprocess.Popen([args.qmk, 'console'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, bufsize=1)

    def stop_at_deadline():
        time.sleep(max(0, args.record_until - time.time()))
        proc.send_signal(signal.SIGINT)
        time.sleep(5)
        proc.kill()

    threading.Thread(target=stop_at_deadline, daemon=True).start()

    for line in proc.stdout:
        line = ANSI.sub('', line).rstrip('\n')
        idx = line.find('HRM ')
        if idx < 0:
            continue
        now = time.time()
        path = os.path.join(args.log_dir, time.strftime('hrm-%Y-%m-%d.log', time.localtime(now)))
        with open(path, 'a') as f:
            f.write(f'{now:.3f} {line[idx:]}\n')
    proc.wait()


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--qmk', required=True)
    p.add_argument('--log-dir', required=True)
    p.add_argument('--install-dir', required=True)
    p.add_argument('--plist', required=True)
    p.add_argument('--label', required=True)
    p.add_argument('--record-until', type=float, required=True)
    p.add_argument('--purge-at', type=float, required=True)
    args = p.parse_args()

    while time.time() < args.record_until:
        record(args)
        time.sleep(5)  # qmk console exited (e.g. error); restart it

    # Recording is over: idle until the purge deadline (launchd restarts us at
    # login, so this survives reboots).
    while time.time() < args.purge_at:
        time.sleep(min(600, max(1, args.purge_at - time.time())))
    purge(args)


if __name__ == '__main__':
    main()
