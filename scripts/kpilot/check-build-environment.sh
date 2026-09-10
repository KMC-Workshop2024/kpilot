#!/usr/bin/env bash
set -eu

check() {
    if command -v "$1" >/dev/null 2>&1; then
        printf 'OK   %-12s %s\n' "$1" "$(command -v "$1")"
    else
        printf 'MISS %-12s\n' "$1"
    fi
}

check cmake
check git
check ninja
check qmake6
check qtpaths6
check ctest

printf 'OS   %s\n' "$(uname -a)"
printf 'CXX  %s\n' "${CXX:-unset}"
