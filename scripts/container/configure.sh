#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "usage: $0 <configure-preset>" >&2
    exit 64
fi

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"
PRESET="$1"

echo "Configuring BinMorph with CMake preset: ${PRESET}"
cmake -S "${REPO_ROOT}" --preset "${PRESET}"
