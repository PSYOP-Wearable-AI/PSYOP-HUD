#!/bin/bash
set -euo pipefail

OS=$(uname -s)

echo "🚀 Setting up LVGL HUD dependencies..."

if [[ "$OS" == "Darwin" ]]; then
  if ! command -v brew >/dev/null 2>&1; then
    echo "❌ Homebrew not found. Install from https://brew.sh"; exit 1
  fi
  echo "📦 Installing SDL2, cmake, pkg-config..."
  brew install sdl2 sdl2_image sdl2_ttf cmake pkg-config
else
  echo "📦 Installing SDL2, cmake, pkg-config..."
  sudo apt update
  sudo apt install -y build-essential cmake pkg-config libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
fi

echo "✅ Dependencies installed"
