# Sanitizers

## ASan + UBSan

Commonly compatible and appropriate for parser, layout, runtime view, and generated-library tests. Build with frame pointers and debug information. Disable leak detection only when the platform demonstrably cannot support it, and document the exception.

Suggested environment:

```bash
ASAN_OPTIONS=abort_on_error=1:detect_leaks=1:strict_string_checks=1
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1
```

## TSan

Run separately. Reduce test concurrency if memory pressure causes false infrastructure failures. Never suppress project races merely to make the lane green.

## MSan

Requires an instrumented dependency ecosystem. Only claim MSan coverage when linked dependencies and C++ runtime are instrumented sufficiently.

## Exit behavior

Sanitizer findings must fail the test process. Preserve logs under `artifacts/logs/sanitizers/`.
