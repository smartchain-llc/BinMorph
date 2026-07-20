# Docker Test Change Review

## Reproducibility
- [ ] Base image and major tool versions are pinned.
- [ ] Clean-cache build succeeds.
- [ ] Build commands are repository-owned and shared with CI.
- [ ] Host environment is not required for correctness.

## Build and tests
- [ ] Configure, build, and test presets are explicit.
- [ ] Test failures propagate nonzero exit status.
- [ ] CTest emits useful failure output and JUnit.
- [ ] Fixtures and working directories are deterministic.
- [ ] Generated API consumers compile and run where applicable.

## Safety
- [ ] Tests execute as non-root.
- [ ] No privileged mode, Docker socket, or broad host mount.
- [ ] Secrets are not present in image layers or logs.
- [ ] Resource limits exist for adversarial/fuzz tests.

## Artifacts
- [ ] Logs and reports survive container deletion.
- [ ] Coverage raw data is retained.
- [ ] Metadata records toolchain and image identity.
- [ ] Artifact ownership is usable by the host caller.

## Maintainability
- [ ] Docker logic does not duplicate CMake semantics.
- [ ] Matrix lanes each cover a stated risk.
- [ ] Dockerfile cache boundaries match dependency boundaries.
- [ ] Documentation includes exact local and CI commands.
