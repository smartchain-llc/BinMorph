---
name: binmorph-docker-test
summary: Create and use reproducible Docker environments to configure, build, test, sanitize, fuzz, measure coverage, and package the BinMorph C/C++ project with CMake and CTest.
description: Use when a task involves Dockerfiles, BuildKit, docker compose, containerized CMake builds, CTest, GoogleTest/Catch2, compiler matrices, sanitizers, coverage, fuzzing, dependency isolation, CI parity, test fixtures, runtime daemon integration tests, or extracting build and test artifacts for the BinMorph project.
---

# BinMorph Docker Test Skill

Use this skill to create or operate hermetic Docker-based build and test environments for BinMorph. The goal is to make local execution, Codex validation, and CI behavior equivalent and repeatable.

## Activate this skill when

- A task asks to build or test BinMorph in Docker.
- A Dockerfile, Compose file, container entrypoint, or test runner is added or changed.
- Host-specific failures need isolation or reproduction.
- Multiple compilers, build types, architectures, or dependency versions must be tested.
- Coverage, ASan, UBSan, TSan, MSan, Valgrind, or fuzzing is requested.
- The BinMorph daemon, CLI, generated library, or sample schemas require integration tests.
- Test reports, coverage files, binaries, generated sources, or packages must be copied from a container.
- CI should invoke the same container workflow developers use locally.

Do not activate for production container deployment unless the task also includes build or test validation. Production image guidance belongs in a deployment-oriented skill.

## Operating principles

1. **One canonical build interface.** Docker, local development, and CI invoke the same CMake presets and scripts.
2. **Hermetic by default.** Do not depend on undeclared host compilers, libraries, environment variables, or mounted package caches for correctness.
3. **BuildKit-first.** Use syntax-supported cache mounts and multi-stage builds without making caches semantically required.
4. **Non-root runtime.** Build as root only where package installation requires it; execute project tests as an unprivileged user.
5. **Immutable source input.** Mount source read-only for interactive test runs when practical. Write build outputs to named volumes or container-owned directories.
6. **No hidden test success.** Preserve process exit codes. Never append `|| true` to required configure, build, test, coverage, sanitizer, or packaging commands.
7. **CTest is authoritative.** Register tests with CMake and execute them through CTest unless a narrowly scoped diagnostic requires direct invocation.
8. **Artifacts survive containers.** Emit JUnit, coverage, logs, generated code, and packages into a predictable artifact directory.
9. **Security boundaries remain intact.** Do not use `--privileged`, host networking, Docker socket mounts, or broad host filesystem mounts unless the test explicitly requires them and the risk is documented.
10. **Generated/runtime parity.** Integration tests should exercise both generic binary interpretation and generated APIs against identical fixtures where applicable.

## First actions

1. Inspect the repository before inventing build commands:
   - top-level `CMakeLists.txt`;
   - `CMakePresets.json` and `CMakeUserPresets.json`;
   - dependency manifests and lockfiles;
   - existing `Dockerfile*`, `compose*.yml`, CI files, test directories, and coverage tooling.
2. Detect the supported compiler and language standard from project files.
3. Reuse existing presets. Add presets only when the required test mode is missing.
4. Select the smallest workflow from **Task routing**.
5. Run the resulting container workflow and report exact commands and artifacts.

## Task routing

### Create the baseline build-and-test image

Read:
- `references/dockerfile-design.md`
- `references/cmake-ctest-contract.md`
- `references/security.md`
- `templates/docker/Dockerfile`

Required stages:
- `toolchain`: OS packages, compiler, CMake, Ninja, and dependency prerequisites;
- `configure`: CMake configure with a named preset;
- `build`: compile requested targets;
- `test`: execute CTest and emit JUnit;
- `artifacts`: expose selected outputs without carrying the full toolchain.

### Add a compiler or build-type matrix

Read:
- `references/test-matrix.md`
- `references/cmake-ctest-contract.md`
- `templates/compose/compose.test.yml`

Prefer one parametrized Dockerfile using build arguments and CMake presets over duplicated Dockerfiles. Matrix dimensions should be purposeful, not Cartesian by default.

### Run sanitizers

Read:
- `references/sanitizers.md`
- `references/test-matrix.md`

Use dedicated presets and separate build trees. Do not combine incompatible sanitizers. Preserve symbolization and fail on sanitizer findings.

### Generate coverage

Read:
- `references/coverage.md`
- `scripts/run-coverage.sh`

Coverage requires an instrumented build from a clean build directory. Exclude external dependencies, generated third-party code, tests, and toolchain headers by explicit path rules—not broad patterns that hide project code.

### Test the daemon or CLI

Read:
- `references/integration-testing.md`
- `templates/compose/compose.integration.yml`

Use health checks and explicit readiness, not arbitrary sleeps. Put test clients and the daemon on an isolated Compose network. Use temporary named volumes for fixtures and outputs.

### Test generated libraries

Read:
- `references/generated-api-testing.md`
- `references/integration-testing.md`

The workflow must:
1. compile a representative schema;
2. generate source/library artifacts;
3. compile a consumer against the generated API;
4. decode valid fixtures;
5. reject truncated, malformed, overflow-inducing, and incompatible fixtures;
6. compare results with the generic runtime interpreter.

### Add fuzzing

Read:
- `references/fuzzing.md`

Build fuzz targets separately with Clang/libFuzzer where supported. Seed corpora from valid schemas and binary fixtures. Keep crashing inputs as regression fixtures.

### Reproduce a CI or host-only failure

Read:
- `references/debugging.md`

Capture:
- base image digest;
- compiler, linker, CMake, and dependency versions;
- configure command/cache;
- complete failing test output;
- relevant environment variables;
- architecture and kernel constraints.

Minimize the reproduction before changing project code.

## Required repository interface

Prefer these repository-owned commands:

```text
./scripts/container/configure.sh <preset>
./scripts/container/build.sh <preset> [targets...]
./scripts/container/test.sh <test-preset>
./scripts/container/coverage.sh <preset>
./scripts/container/package.sh <preset>
```

When those scripts do not exist, introduce them only if they remove duplicated Docker/CI logic. Each script must use `set -euo pipefail`, print the invoked high-level operation, avoid implicit `cd` assumptions, and propagate the underlying exit code.

## Expected CMake preset model

Use repository presets with distinct binary directories:

- `docker-gcc-debug`
- `docker-clang-debug`
- `docker-gcc-release`
- `docker-asan-ubsan`
- `docker-tsan`
- `docker-coverage`
- `docker-fuzz`

Associated build and test presets should derive from configure presets. Do not rely on a user’s `CMakeUserPresets.json` inside CI or canonical Docker tests.

## Test execution requirements

At minimum, a standard Docker validation must perform:

1. configure from a clean build tree;
2. build all production and test targets;
3. run `ctest --output-on-failure`;
4. emit JUnit XML;
5. run a representative CLI smoke test;
6. run schema-to-generated-library integration tests when generation is enabled;
7. place artifacts beneath `/workspace/artifacts`;
8. exit nonzero on any failure.

For parallel CTest, use `--parallel` or `CTEST_PARALLEL_LEVEL`; do not simultaneously oversubscribe Docker CPU limits and nested test runners.

## Dockerfile requirements

- Pin the base image by version; use a digest in release or CI-sensitive contexts.
- Set `DEBIAN_FRONTEND=noninteractive` only for package installation scope.
- Combine `apt-get update` and install in one layer and remove package lists.
- Use `COPY --link` where supported, but do not require it for correctness.
- Copy dependency metadata before source code to maximize cache reuse.
- Use `--mount=type=cache` for package and compiler caches only as an optimization.
- Never bake credentials, tokens, SSH keys, proprietary fixtures, or user home directories into layers.
- Set a deterministic `WORKDIR`.
- Create and switch to an unprivileged user before executing tests.
- Use exec-form `ENTRYPOINT` or `CMD`.
- Add an init process only when tests spawn child processes that need reaping.

## Compose requirements

- Compose is for multi-container integration tests or convenient matrix orchestration, not a mandatory wrapper around every single-container build.
- Use service profiles for optional coverage, sanitizer, fuzz, or daemon suites.
- Use `depends_on` with health conditions only where supported; test runners must still implement bounded readiness checks.
- Keep services on an internal test network unless external connectivity is genuinely required.
- Avoid fixed container names and host ports; use service DNS and ephemeral ports.
- Mark source mounts read-only.
- Use `tmpfs` for transient state when practical.
- Set resource limits for tests that can runaway.

## Artifact contract

Write outputs to `/workspace/artifacts` using this layout:

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

`metadata/` should include compiler versions, CMake version, selected presets, base image identity, Git revision when available, and the exact test command.

## Validation checklist

Before declaring success:

- [ ] The image builds from a clean Docker cache.
- [ ] The workflow also succeeds with cache enabled.
- [ ] Tests run as non-root.
- [ ] Required test failures produce a nonzero container exit code.
- [ ] JUnit and logs are copied to the artifact directory.
- [ ] Source changes invalidate build layers correctly.
- [ ] Dependency-only layers remain reusable when source files change.
- [ ] No secret is present in image history or build arguments.
- [ ] No privileged mode, Docker socket, or host network is used without justification.
- [ ] The documented local command matches the CI command.
- [ ] Generated and generic decoding paths are compared where applicable.
- [ ] Truncated and malformed binary fixtures are tested.

Use `checklists/review.md` for detailed review.

## Codex completion report

Every completed task should report:

1. Container workflow created or used.
2. Base image and compiler/toolchain.
3. CMake configure, build, and test presets.
4. Tests executed and test count.
5. Sanitizer, coverage, fuzz, or integration modes exercised.
6. Artifact paths produced.
7. Docker security exceptions, if any.
8. Exact reproduction command.
9. Remaining untested matrix dimensions or platform limitations.

Do not claim the Docker workflow works unless it was executed successfully in an environment with Docker access. When Docker is unavailable, validate syntax and repository integration as far as possible and explicitly state that runtime execution remains unverified.
