# Getting Started

This guide documents the clean-checkout path for building, testing, running,
debugging, formatting, and validating BinMorph.

## Supported Environment

The repository is maintained on Linux with:

- CMake 3.20 or newer
- GCC or Clang with C++17 support
- Python 3 for CLI fixture generation
- GoogleTest and GoogleMock, found by CMake when installed or fetched through
  `FetchContent`
- Optional Doxygen for API documentation
- Optional clang-tidy for static analysis
- Optional Docker 26 or compatible for containerized validation

## Configure

```bash
cmake --preset projects
```

This configures a Debug build in `build/projects` and emits
`build/projects/compile_commands.json` for clang-tidy and editor tooling.

## Build

```bash
cmake --build --preset projects
```

Primary targets:

```text
binmorph_core          Static library containing parser, compiler, interpreter, renderer, and runner logic
binmorph               CLI executable at build/projects/binmorph
binmorph_unit_tests    GoogleTest executable at build/projects/binmorph_unit_tests
```

## Test

```bash
ctest --preset projects
```

The suite includes discovered GoogleTest cases plus the `cli_contract` shell
test. Fixtures live under `tests/resources`.

## Run BinMorph

Show usage:

```bash
./build/projects/binmorph --help
```

Inspect the checked-in sample data:

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

The command writes `binmorph.inspect.v1` JSON to
`build/projects/sample-output.json`.

## Format And Validate

```bash
scripts/format.sh
scripts/check-format.sh
scripts/check-repository-hygiene.sh
```

When optional tools are installed:

```bash
scripts/check-doxygen.sh
BUILD_DIR=build/projects scripts/run-clang-tidy.sh
```

Generated Doxygen HTML is written to `build/docs/html`.

## Docker

Run the default containerized GCC lane:

```bash
docker compose -f docker/compose.test.yml run --rm --build gcc-debug
```

The Compose lane writes host-visible reports under
`artifacts/docker/gcc-debug`. See [../docker/README.md](../docker/README.md)
for additional profiles and the integration-test shape reserved for a future
daemon.

## VS Code

Open the repository root and select the `projects` CMake preset. The shared
workspace settings use the Microsoft C/C++ extension with the repository
`.clang-format`, disable fallback formatting, and recommend CMake Tools.

Useful tasks:

- `cmake: build projects`
- `ctest: projects`
- `debug setup: sample inspect`

Launch configurations are provided for the CLI sample, truncated input, and
unit tests.

## Cleanup

Generated files are ignored by Git. To remove local build and validation output:

```bash
rm -rf build artifacts CMakeFiles
```

Do not remove source directories such as `include`, `src`, `tests`, `docs`,
`docker`, or `scripts`.

## Troubleshooting

- If CMake cannot find GoogleTest, it uses `FetchContent` to download
  GoogleTest v1.14.0. Network-restricted environments should install
  GoogleTest system-wide or use the Docker workflow.
- If `scripts/check-format.sh` cannot find `clang-format`, install
  `clang-format` or the VS Code C/C++ extension. The script can use the
  extension-bundled formatter when available.
- If Docker Compose artifacts are owned by root, rerun the documented Compose
  command. The test runner fixes the artifact mount ownership before dropping
  to the unprivileged `builder` user.
