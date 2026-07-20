#!/usr/bin/env bash
set -euo pipefail

image="${BINMORPH_TEST_IMAGE:-binmorph-test}"
target="${BINMORPH_DOCKER_TARGET:-test}"
configure_preset="${BINMORPH_CONFIGURE_PRESET:-docker-gcc-debug}"
build_preset="${BINMORPH_BUILD_PRESET:-docker-gcc-debug}"
test_preset="${BINMORPH_TEST_PRESET:-docker-gcc-debug}"
artifact_dir="${BINMORPH_ARTIFACT_DIR:-$PWD/artifacts/docker}"

mkdir -p "$artifact_dir"

docker build \
  --target "$target" \
  --build-arg "CONFIGURE_PRESET=$configure_preset" \
  --build-arg "BUILD_PRESET=$build_preset" \
  --build-arg "TEST_PRESET=$test_preset" \
  --tag "$image" \
  .

docker run --rm \
  --read-only \
  --tmpfs /tmp:rw,nosuid,nodev \
  --volume "$artifact_dir:/workspace/artifacts" \
  "$image"
