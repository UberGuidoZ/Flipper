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
FIRMWARE_VER="$1"

pushd "$(git -C "$SCRIPT_DIR" rev-parse --show-toplevel)"
print_status notice "updating to firmware $FIRMWARE_VER"

# setup git
git config --local user.name $GIT_USER_NAME
git config --local user.email $GIT_USER_EMAIL

# construct a new version number for the qrcode app
VER="$(git tag | sed -E -e '/^v[0-9]+\.[0-9]+\.[0-9]+$/!d' | sort -V | tail -n1)"
if [[ "$VER" =~ ^(v[0-9]+.[0-9]+).([0-9]+)$ ]]; then
    VER="${BASH_REMATCH[1]}.$(( ${BASH_REMATCH[2]} + 1 ))"
else
    print_status warning "couldn't construct new version number from $VER"
    exit 1
fi
print_status notice "new qrcode version: $VER"

# update firmware version in automation
sed -i -e "/firmware_version:/s/'.*'/'$FIRMWARE_VER'/" .github/workflows/release.yml

# commit and tag
git add .github/workflows/release.yml
git commit -m "update to firmware $FIRMWARE_VER"
git tag -a -m "$VER" "$VER"
git tag "firmware-v$FIRMWARE_VER"
git push --atomic origin main "$VER" "firmware-v$FIRMWARE_VER"

popd
