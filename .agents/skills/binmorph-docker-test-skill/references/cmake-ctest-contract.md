# CMake and CTest Contract

Docker is an execution environment; CMake presets remain the project build API.

## Configure

Use:

```bash
cmake --preset "$CONFIGURE_PRESET"
```

Avoid restating cache variables in Docker when they belong in presets. Build arguments may select a preset, but must not silently mutate its semantics.

## Build

Use:

```bash
cmake --build --preset "$BUILD_PRESET" --parallel
```

For multi-config generators, ensure the configuration is represented by the build/test preset.

## Test

Use:

```bash
ctest --preset "$TEST_PRESET" --output-on-failure \
  --output-junit /workspace/artifacts/junit/results.xml
```

Tests requiring fixtures should receive paths through CMake-generated environment properties or test command arguments, not the caller's current working directory.

## Installation and packaging

Exercise `cmake --install` into a staging prefix and compile at least one external consumer when the project exports a library package. Use CPack only after installation rules are verified.
