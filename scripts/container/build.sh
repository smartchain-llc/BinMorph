#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "usage: $0 <build-preset> [targets...]" >&2
    exit 64
fi

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"
PRESET="$1"
shift

echo "Building BinMorph with CMake preset: ${PRESET}"
if [[ $# -eq 0 ]]; then
    cmake --build --preset "${PRESET}" --parallel
else
    cmake --build --preset "${PRESET}" --parallel --target "$@"
fi
