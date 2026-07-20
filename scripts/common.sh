#!/usr/bin/env bash

repo_root() {
    git rev-parse --show-toplevel 2>/dev/null || pwd
}

find_clang_format() {
    if command -v clang-format >/dev/null 2>&1; then
        command -v clang-format
        return 0
    fi

    local candidate
    for candidate in \
        "$HOME"/.vscode-server/extensions/ms-vscode.cpptools-*/LLVM/bin/clang-format \
        "$HOME"/.vscode/extensions/ms-vscode.cpptools-*/LLVM/bin/clang-format; do
        if [[ -x "$candidate" ]]; then
            printf '%s\n' "$candidate"
            return 0
        fi
    done

    return 1
}

collect_cpp_sources() {
    local root="$1"
    shift || true

    if [[ $# -gt 0 ]]; then
        local path
        for path in "$@"; do
            if [[ -f "$path" ]]; then
                printf '%s\0' "$path"
            elif [[ -f "$root/$path" ]]; then
                printf '%s\0' "$root/$path"
            fi
        done
        return 0
    fi

    find "$root" \
        \( -path "$root/.git" \
        -o -path "$root/.agents" \
        -o -path "$root/build" \
        -o -path "$root/artifacts" \
        -o -path "$root/codex-skills" \
        -o -path "$root/CMakeFiles" \
        -o -path "$root/third_party" \
        -o -path "$root/external" \
        -o -path "$root/vendor" \) -prune \
        -o -type f \( \
        -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \
        -o -name '*.h' -o -name '*.hh' -o -name '*.hpp' -o -name '*.hxx' \
        -o -name '*.ipp' -o -name '*.tpp' -o -name '*.ixx' -o -name '*.cppm' \
        \) -print0
}
