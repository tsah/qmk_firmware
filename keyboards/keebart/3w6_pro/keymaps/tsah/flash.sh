#!/usr/bin/env bash
# Compile and flash both halves (macOS or Linux). Extra args are passed on:
#   ./flash.sh [left|right|both] [--no-compile]
exec "$(git -C "$(dirname "$0")" rev-parse --show-toplevel)/users/tsah/flash.sh" keebart/3w6_pro "$@"
