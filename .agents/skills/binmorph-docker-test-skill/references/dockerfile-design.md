# Dockerfile Design

Use a multi-stage Dockerfile so dependency installation, configuration, compilation, testing, and artifact extraction remain separable.

## Recommended stage graph

```text
toolchain
   ├── configure
   │      └── build
   │             ├── test
   │             ├── coverage
   │             ├── package
   │             └── generated-api-test
   └── interactive-dev

scratch/alpine artifact stage <- selected outputs only
```

The exact artifact base depends on whether binaries must execute in that stage. For pure export, use BuildKit `--output` or a minimal stage containing files only.

## Context discipline

Maintain a `.dockerignore` that excludes build trees, VCS internals where not needed, IDE state, downloaded corpora, credentials, and existing artifacts. Do not exclude schema fixtures or test data required by the build.

## Dependency caching

Copy dependency descriptors and CMake modules before general source code. Cache package downloads and compiler caches, but test correctness with empty caches periodically.

## Source mounting versus copying

CI should build copied source to guarantee the context is complete. Interactive development may mount source read-only and place the binary directory on a named volume.
