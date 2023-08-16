#!/bin/bash
set -Exeuo pipefail

print_status() {
  local level="$1"
  local body="${2//%/%25}"
  body="${body//$'\r'/}"
  body="${body//$'\n'/%0A}"

  echo "::$level::$body"
}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")"; pwd -P)"
FIRMWARE_DIR="$1"

LASTVER="$(git -C "$SCRIPT_DIR" tag | grep firmware-v | sort -V | tail -n1 | sed -e 's/^firmware-v//')"
print_status notice "last built against firmware version: $LASTVER"

VER="$(git -C "$FIRMWARE_DIR" tag | sed -E -e '/^[0-9]+\.[0-9]+\.[0-9]+$/!d' | sort -V | sed -e "1,/$LASTVER/d" | tail -n1)"
# VER="$(curl https://api.github.com/repos/flipperdevices/flipperzero-firmware/tags | jq -r --arg current "$LASTVER" 'def ver($v): $v | ltrimstr("v") | split(".") | map(tonumber); map(.name) | map(select(. | test("^\\d+\\.\\d+\\.\\d+$";"s"))) | map(ver(.)) | map(select(. > ver($current))) | sort | last | if . == null then "" else join(".") end')"
if [ -z "$VER" ]; then
  print_status notice "no new firmware version"
  exit 0
fi
print_status notice "new firmware version: $VER"
echo "version=$VER" >> $GITHUB_OUTPUT
