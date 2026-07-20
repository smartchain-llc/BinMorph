# Coverage

Use either GCC/gcov/lcov or Clang source-based coverage consistently within one lane.

## GCC flow

1. Configure a clean instrumented build.
2. Build and run tests.
3. Capture coverage from the same object tree.
4. Filter external and generated-unowned code by rooted paths.
5. Produce LCOV and HTML output.

## Clang flow

Set `LLVM_PROFILE_FILE` with a pattern that prevents parallel tests from overwriting profiles. Merge profiles using `llvm-profdata`, then export with `llvm-cov`.

## Integrity rules

- Do not reuse non-instrumented objects.
- Do not filter missing project coverage by wildcard accident.
- Preserve raw coverage files for diagnosis.
- Treat branch coverage separately from line coverage.
- Coverage thresholds should distinguish regression prevention from aspirational targets.
