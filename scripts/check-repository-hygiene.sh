#!/usr/bin/env bash
set -euo pipefail

usage() {
    cat <<'USAGE'
usage: scripts/check-repository-hygiene.sh

Checks repository-facing hygiene: required onboarding files, generated-file
policy, tracked debris, shell script syntax, and obvious local path leaks.
The root codex-skills directory is intentionally ignored.
USAGE
}

if [[ "${1:-}" == "--help" || "${1:-}" == "-h" ]]; then
    usage
    exit 0
fi

ROOT="$(git rev-parse --show-toplevel 2>/dev/null || pwd)"
cd "${ROOT}"

errors=0
warnings=0

warn() {
    printf 'warning: %s\n' "$*" >&2
    warnings=$((warnings + 1))
}

fail() {
    printf 'error: %s\n' "$*" >&2
    errors=$((errors + 1))
}

[[ -f README.md ]] || fail "README.md is missing"
[[ -f CONTRIBUTING.md ]] || fail "CONTRIBUTING.md is missing"
[[ -f docs/getting-started.md ]] || fail "docs/getting-started.md is missing"
[[ -f .clang-format ]] || fail ".clang-format is missing"
[[ -f .editorconfig ]] || fail ".editorconfig is missing"
[[ -f Doxyfile ]] || fail "Doxyfile is missing"
[[ -d docker ]] || warn "docker/ directory is missing"

if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    for path in build CMakeFiles _build out coverage html latex Testing .cache artifacts; do
        if git ls-files --error-unmatch "${path}" >/dev/null 2>&1; then
            fail "generated or transient path is tracked: ${path}"
        fi
    done

    tracked_debris="$(git ls-files ':!codex-skills/**' | grep -E '(^|/)(\.DS_Store|Thumbs\.db|.*\.swp|.*\.tmp|core(\.[0-9]+)?|compile_commands\.json|coverage\.info)$' || true)"
    if [[ -n "${tracked_debris}" ]]; then
        fail "tracked transient files detected:
${tracked_debris}"
    fi

    absolute_paths="$(git grep -nE '(/home/[^ /]+|/Users/[^ /]+|[A-Za-z]:\\Users\\[^ \\]+)' -- ':!codex-skills/**' ':!.agents/**' ':!scripts/check-repository-hygiene.sh' | grep -v '/home/builder/' || true)"
    if [[ -n "${absolute_paths}" ]]; then
        warn "possible user-specific absolute paths detected:
${absolute_paths}"
    fi

    while IFS= read -r script; do
        [[ -z "${script}" ]] && continue
        bash -n "${script}" || fail "shell syntax check failed: ${script}"
    done < <(git ls-files ':!codex-skills/**' '*.sh')
else
    warn "not a Git working tree; skipped tracked-file checks"
fi

if ((errors > 0)); then
    printf 'Repository hygiene check failed with %d error(s) and %d warning(s).\n' \
        "${errors}" "${warnings}" >&2
    exit 1
fi

printf 'Repository hygiene baseline passed with %d warning(s).\n' "${warnings}"
