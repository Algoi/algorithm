#!/usr/bin/env bash

set -euo pipefail

have_cmd() {
  command -v "$1" >/dev/null 2>&1
}

echo "Checking GitHub workflow prerequisites..."

missing=0

if have_cmd git; then
  echo "git: OK ($(git --version))"
else
  echo "git: MISSING"
  missing=1
fi

if have_cmd gh; then
  echo "gh: OK ($(gh --version | head -n 1))"
else
  echo "gh: MISSING"
  missing=1
fi

if [ "$missing" -ne 0 ]; then
  echo
  echo "Install the missing tools before running GitHub repository workflows."
  exit 1
fi

echo
if gh auth status >/dev/null 2>&1; then
  echo "gh auth: OK"
else
  echo "gh auth: NOT LOGGED IN"
  echo "Run 'gh auth login' before using remote GitHub actions."
  exit 2
fi
