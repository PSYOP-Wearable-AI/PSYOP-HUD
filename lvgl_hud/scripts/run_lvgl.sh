#!/bin/bash
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"
BIN="$BUILD_DIR/visor_lvgl_hud"

if [[ ! -x "$BIN" ]]; then
  echo "❌ Binary not found. Run scripts/build_lvgl.sh first"; exit 1
fi

echo "🚀 Launching VISOR LVGL HUD..."
"$BIN"
