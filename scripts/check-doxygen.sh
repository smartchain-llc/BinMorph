#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
usage: scripts/check-doxygen.sh [Doxyfile]

Generates API documentation with warnings treated as errors. Output is written
under build/docs by the repository Doxyfile.
USAGE
}

if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
    usage
    exit 0
fi

ROOT="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
DOXYFILE="${1:-${ROOT}/Doxyfile}"

command -v doxygen >/dev/null 2>&1 || {
    echo "doxygen is required" >&2
    exit 127
}
[[ -f "${DOXYFILE}" ]] || {
    echo "Doxyfile not found: ${DOXYFILE}" >&2
    exit 2
}

cd "${ROOT}"
doxygen "${DOXYFILE}"
