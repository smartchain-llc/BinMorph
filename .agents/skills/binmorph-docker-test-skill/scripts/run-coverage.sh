#!/usr/bin/env bash
set -euo pipefail

preset="${1:-docker-coverage}"
artifact_root="${BINMORPH_ARTIFACT_ROOT:-/workspace/artifacts}"
build_dir="${BINMORPH_BUILD_DIR:-/workspace/build/$preset}"

mkdir -p "$artifact_root/coverage" "$artifact_root/junit"
cmake --preset "$preset"
cmake --build --preset "$preset" --parallel
ctest --preset "$preset" --output-on-failure \
  --output-junit "$artifact_root/junit/coverage-results.xml"

lcov --capture --directory "$build_dir" \
  --output-file "$artifact_root/coverage/raw.info"
lcov --remove "$artifact_root/coverage/raw.info" \
  '/usr/*' '*/_deps/*' '*/tests/*' \
  --output-file "$artifact_root/coverage/project.info"
genhtml "$artifact_root/coverage/project.info" \
  --output-directory "$artifact_root/coverage/html"
