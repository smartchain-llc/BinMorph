#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
usage: scripts/run-clang-tidy.sh [clang-tidy-arg...]

Runs clang-tidy for project implementation files. Set BUILD_DIR to the CMake
binary directory containing compile_commands.json; defaults to build/projects.
USAGE
}

if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
    usage
    exit 0
fi

ROOT="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
BUILD_DIR="${BUILD_DIR:-${ROOT}/build/projects}"
COMPILE_COMMANDS="${BUILD_DIR}/compile_commands.json"

[[ -f "${COMPILE_COMMANDS}" ]] || {
    echo "Missing ${COMPILE_COMMANDS}; configure with CMAKE_EXPORT_COMPILE_COMMANDS=ON" >&2
    exit 2
}

if command -v run-clang-tidy >/dev/null 2>&1; then
    exec run-clang-tidy -p "${BUILD_DIR}" \
        -header-filter="^(?!.*(?:third_party|external|vendor|build|codex-skills)/).*$" "$@"
fi

command -v clang-tidy >/dev/null 2>&1 || {
    echo "clang-tidy is required" >&2
    exit 127
}

mapfile -d '' files < <(find "${ROOT}" \
    \( -path "${ROOT}/build" \
    -o -path "${ROOT}/artifacts" \
    -o -path "${ROOT}/codex-skills" \
    -o -path "${ROOT}/third_party" \
    -o -path "${ROOT}/external" \
    -o -path "${ROOT}/vendor" \) -prune \
    -o -type f \( -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \) -print0)

clang-tidy -p "${BUILD_DIR}" "${files[@]}" "$@"
