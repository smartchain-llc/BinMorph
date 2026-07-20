# BinMorph Docker Test Images

This directory owns the container workflow for building and testing BinMorph
binaries. The Dockerfile calls repository scripts under `scripts/container/`,
and those scripts call CMake presets from `CMakePresets.json`.

## Baseline Build And Test

```bash
DOCKER_BUILDKIT=1 docker build \
  -f docker/Dockerfile \
  --target test \
  --build-arg CONFIGURE_PRESET=docker-gcc-debug \
  --build-arg BUILD_PRESET=docker-gcc-debug \
  --build-arg TEST_PRESET=docker-gcc-debug \
  -t binmorph/test:gcc-debug \
  .
```

The default lane configures, builds, runs CTest, runs a CLI smoke test, and
writes artifacts under `/workspace/artifacts` in the image. Use the `artifacts`
target to produce a smaller image containing the tested CLI binary and captured
artifacts.

```bash
DOCKER_BUILDKIT=1 docker build \
  -f docker/Dockerfile \
  --target artifacts \
  --build-arg BUILD_PRESET=docker-gcc-debug \
  -t binmorph/artifacts:gcc-debug \
  .
```

## Compose Matrix

```bash
BINMORPH_DOCKER_UID="$(id -u)" BINMORPH_DOCKER_GID="$(id -g)" \
  docker compose -f docker/compose.test.yml run --rm --build gcc-debug
BINMORPH_DOCKER_UID="$(id -u)" BINMORPH_DOCKER_GID="$(id -g)" \
  docker compose -f docker/compose.test.yml --profile clang run --rm --build clang-debug
BINMORPH_DOCKER_UID="$(id -u)" BINMORPH_DOCKER_GID="$(id -g)" \
  docker compose -f docker/compose.test.yml --profile release run --rm --build gcc-release
BINMORPH_DOCKER_UID="$(id -u)" BINMORPH_DOCKER_GID="$(id -g)" \
  docker compose -f docker/compose.test.yml --profile sanitizer run --rm --build asan-ubsan
```

Compose writes host-visible outputs under `artifacts/docker/<lane>/`.

## Integration Shape

`docker/compose.integration.yml` currently exercises the CLI contract on an
internal network. When a `binmorphd` target exists, add a daemon service on the
same network and have test clients communicate with it by service DNS over an
ephemeral port or Unix socket volume. Do not use host networking, fixed host
ports, privileged mode, or Docker socket mounts for these tests.

## Artifact Layout

Container workflows write:

```text
artifacts/
├── junit/
├── coverage/
├── logs/
├── generated/
├── packages/
├── fuzz/
└── metadata/
```
