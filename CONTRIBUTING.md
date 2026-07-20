# Contributing

Follow [Getting started](docs/getting-started.md) before making changes. The
project expects build, test, formatting, documentation, and repository hygiene
to be reproducible from the repository root.

## Change Workflow

1. Make a focused change.
2. Add or update tests and documentation for behavior or API changes.
3. Format source with the repository formatter.
4. Run the strongest relevant validation subset.
5. Confirm generated files remain under ignored build or artifact directories.

## Required Checks

For ordinary C++ changes:

```bash
cmake --preset projects
cmake --build --preset projects
ctest --preset projects
scripts/check-format.sh
scripts/check-repository-hygiene.sh
```

When Doxygen and clang-tidy are installed, also run:

```bash
scripts/check-doxygen.sh
BUILD_DIR=build/projects scripts/run-clang-tidy.sh
```

For container changes:

```bash
docker compose -f docker/compose.test.yml run --rm --build gcc-debug
```

## Source Standards

- C++ source uses the root `.clang-format`.
- Public and source-level API contracts are documented with Doxygen comments.
- Headers must be self-contained and avoid accidental transitive includes.
- Binary offsets, sizes, and input limits must stay checked at trust
  boundaries.
- Generated, downloaded, or runtime artifacts belong under ignored build or
  artifact directories unless a document explicitly explains why they are
  tracked.

## Scripts

Repository scripts live under `scripts/`, use Bash with `set -euo pipefail`,
resolve paths relative to the repository, and provide `--help` when they are
developer-facing.

## Pull Requests

Keep pull requests scoped to one coherent change. Include the exact validation
commands run and mention any unavailable optional tools, such as Doxygen,
clang-tidy, Docker, or sanitizer lanes.
