#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "usage: $0 <test-preset>" >&2
    exit 64
fi

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"
PRESET="$1"
ARTIFACT_DIR="${BINMORPH_ARTIFACT_DIR:-${REPO_ROOT}/artifacts/${PRESET}}"
BINARY="${BINMORPH_BINARY:-${REPO_ROOT}/build/${PRESET}/binmorph}"

mkdir -p \
    "${ARTIFACT_DIR}/coverage" \
    "${ARTIFACT_DIR}/fuzz" \
    "${ARTIFACT_DIR}/generated" \
    "${ARTIFACT_DIR}/junit" \
    "${ARTIFACT_DIR}/logs" \
    "${ARTIFACT_DIR}/metadata" \
    "${ARTIFACT_DIR}/packages"

{
    echo "preset=${PRESET}"
    echo "artifact_dir=${ARTIFACT_DIR}"
    echo "binary=${BINARY}"
    echo "test_command=ctest --preset ${PRESET} --output-on-failure --output-junit ${ARTIFACT_DIR}/junit/${PRESET}.xml"
    cmake --version | head -n 1
    if command -v g++ >/dev/null 2>&1; then
        g++ --version | head -n 1
    fi
    if command -v clang++ >/dev/null 2>&1; then
        clang++ --version | head -n 1
    fi
    git -C "${REPO_ROOT}" rev-parse --short HEAD 2>/dev/null || echo "git_revision=unavailable"
    uname -a
} >"${ARTIFACT_DIR}/metadata/${PRESET}.txt"

echo "Testing BinMorph with CTest preset: ${PRESET}"
(
    cd "${REPO_ROOT}"
    ctest --preset "${PRESET}" --output-on-failure --output-junit "${ARTIFACT_DIR}/junit/${PRESET}.xml"
) 2>&1 | tee "${ARTIFACT_DIR}/logs/${PRESET}-ctest.log"

echo "Running BinMorph CLI smoke test: ${BINARY}"
"${SCRIPT_DIR}/cli-smoke.sh" "${BINARY}" "${ARTIFACT_DIR}/logs/${PRESET}-cli-smoke.json" \
    2>&1 | tee "${ARTIFACT_DIR}/logs/${PRESET}-cli-smoke.log"
