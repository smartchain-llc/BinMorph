#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
usage: scripts/check-format.sh [file...]

Fails when clang-format would change any selected C or C++ file.
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

status=0
while IFS= read -r -d '' file; do
    if ! diff -u "$file" <("${CLANG_FORMAT}" --style=file "$file"); then
        status=1
    fi
done < <(collect_cpp_sources "${ROOT}" "$@")

exit "${status}"
