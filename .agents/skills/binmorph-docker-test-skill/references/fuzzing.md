# Fuzzing

Prioritize these targets:

- JSON/schema parser;
- schema normalization and expression evaluation;
- binary interpreter with normalized metadata;
- generated API facade inputs;
- artifact manifest parser and compatibility validator.

Use bounded input sizes and execution limits. Seed with minimal valid schemas, boundary layouts, recursive-looking but legal constructs, endian variants, and truncated binary fixtures.

Persist every unique crash or timeout input. Convert fixed findings into deterministic regression tests before removing them from the active crash corpus.
