#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
usage: scripts/format.sh [file...]

Formats C and C++ source with the repository .clang-format. With no files,
formats project source and tests while skipping build, artifact, vendored, and
root codex-skills directories.
USAGE
}

if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
    usage
    exit 0
fi

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "${SCRIPT_DIR}/common.sh"

ROOT="$(repo_root)"
CLANG_FORMAT="$(find_clang_format)" || {
    echo "clang-format is required; install it or the VS Code C/C++ extension" >&2
    exit 127
}

collect_cpp_sources "${ROOT}" "$@" | xargs -0 -r "${CLANG_FORMAT}" -i --style=file
