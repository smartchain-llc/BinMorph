# BinMorph

BinMorph is a C++ command-line prototype for describing byte-oriented binary
layouts with JSON schemas and decoding binary inputs into a structured JSON
inspection report. The implemented surface is intentionally small: a
`binmorph inspect` CLI, a testable `binmorph_core` library, and schema fixtures
for the current `binmorph.schema.v1` dialect.

The broader architecture documents future generated-library and daemon
capabilities, but those surfaces are not implemented yet.

## Quick Start

### Prerequisites

- CMake 3.20 or newer
- A C++17 compiler such as GCC or Clang
- Python 3 for CLI contract fixtures
- Optional: Docker 26 or compatible for containerized validation
- Optional: Doxygen and clang-tidy for documentation and static-analysis checks

### Configure, Build, And Test

```bash
cmake --preset projects
cmake --build --preset projects
ctest --preset projects
```

The primary executable is written to:

```text
build/projects/binmorph
```

### Run The CLI

Create a small binary from the checked-in hex fixture, then inspect it with the
sample schema:

```bash
python3 - <<'PY'
import pathlib
pathlib.Path("build/projects/sample.bin").write_bytes(
    bytes.fromhex(pathlib.Path("tests/resources/sample-binary.hex").read_text())
)
PY

./build/projects/binmorph inspect \
  --schema tests/resources/sample-schema.json \
  --input build/projects/sample.bin \
  --output build/projects/sample-output.json
```

Expected behavior: the command exits with status `0` and writes
`binmorph.inspect.v1` JSON containing decoded fields such as `version` with the
numeric value `4660`.

Use `--input -` for stdin and `--output -` for stdout.

## Repository Layout

```text
include/   Public C++ API for the current CLI/core slice
src/       Library implementation and the binmorph executable entry point
tests/     GoogleTest unit tests, CLI contract test, and fixtures
docs/      Architecture notes, getting-started guide, and UML diagrams
docker/    Containerized build/test images and Compose test lanes
scripts/   Formatting, documentation, static-analysis, and container helpers
.vscode/   Shared VS Code settings, tasks, launch configs, and extensions
```

The root `codex-skills/` directory is local agent tooling and is not part of
the project build or validation surface.

## Schema Slice

Supported field types are:

- Unsigned integers: `u8`, `u16`, `u32`, `u64`
- Signed integers: `i8`, `i16`, `i32`, `i64`
- Byte strings: `bytes`
- Printable text: `ascii`

Multi-byte integer endianness must be explicit at schema level through
`endianness` or at field level through `endian`.

## Quality Checks

```bash
scripts/check-repository-hygiene.sh
scripts/check-format.sh
scripts/check-doxygen.sh
BUILD_DIR=build/projects scripts/run-clang-tidy.sh
```

The CMake project also exposes `format`, `format-check`, `docs-check`, and
`hygiene-check` custom targets.

## Docker Validation

```bash
docker compose -f docker/compose.test.yml run --rm --build gcc-debug
```

See [docker/README.md](docker/README.md) for the compiler matrix, sanitizer
lane, artifact layout, and daemon-ready integration shape.

## Documentation

- [Getting started](docs/getting-started.md)
- [Architecture](docs/ARCHITECTURE.md)
- [Docker test images](docker/README.md)
- [Contributing](CONTRIBUTING.md)

Generate API documentation with:

```bash
scripts/check-doxygen.sh
```

Generated documentation is written under `build/docs/html/`.
