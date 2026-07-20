#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "usage: $0 <test-preset>" >&2
    exit 64
fi

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ARTIFACT_DIR="${BINMORPH_ARTIFACT_DIR:-/workspace/artifacts}"

mkdir -p \
    "${ARTIFACT_DIR}/coverage" \
    "${ARTIFACT_DIR}/fuzz" \
    "${ARTIFACT_DIR}/generated" \
    "${ARTIFACT_DIR}/junit" \
    "${ARTIFACT_DIR}/logs" \
    "${ARTIFACT_DIR}/metadata" \
    "${ARTIFACT_DIR}/packages"

if [[ "$(id -u)" -eq 0 ]]; then
    chown -R builder:builder "${ARTIFACT_DIR}"
    exec runuser --user builder -- env \
        BINMORPH_ARTIFACT_DIR="${ARTIFACT_DIR}" \
        "${SCRIPT_DIR}/test.sh" "$@"
fi

exec "${SCRIPT_DIR}/test.sh" "$@"
