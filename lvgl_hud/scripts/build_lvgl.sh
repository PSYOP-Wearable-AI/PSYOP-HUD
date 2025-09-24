#!/bin/bash
set -euo pipefail

ROOT_DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR="$ROOT_DIR/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "🔨 Configuring..."
cmake -DCMAKE_BUILD_TYPE=Release ..

echo "🏗️  Building..."
cmake --build . --parallel

echo "✅ Build complete"
