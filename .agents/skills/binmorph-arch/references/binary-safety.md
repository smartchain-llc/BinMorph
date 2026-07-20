# Binary Safety

## Mandatory checks

Before every access, prove:

```text
offset >= 0
size >= 0
offset + size does not overflow
offset + size <= view.length
```

For multiplication, prove `count * elementSize` does not overflow before addition. Use checked arithmetic types or dedicated checked operations; do not rely on post-overflow comparisons.

## Dynamic constructs

- Validate dynamic counts and lengths against schema constraints and daemon policy limits.
- Bound recursion, nesting depth, indirection depth, and total materialized object count.
- Detect cyclic offset/reference traversal.
- Treat unterminated strings as errors unless the schema defines a bounded fallback.
- Validate discriminators before selecting union alternatives.
- Make alignment calculations overflow safe.
- Define behavior for overlapping writable regions.

## Streaming inputs

A streaming view must distinguish unavailable-yet, end-of-input, and invalid-range. It may buffer only within configured limits. Random-access-required schemas must be rejected or spooled through an explicit infrastructure policy.

## Mutation

- Views are read-only by default.
- Writable views require explicit construction and authorization.
- Mutation validates width and encoding before writing.
- Changes affecting lengths, offsets, checksums, or dependent expressions trigger controlled recomputation.
- Partial writes must not leave an object in an unspecified state; use transactional staging where multiple regions change.
