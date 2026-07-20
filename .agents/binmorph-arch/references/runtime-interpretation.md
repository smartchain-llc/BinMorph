# Runtime Interpretation

## Interpretation model

The runtime combines immutable type metadata with a bounded `BinaryView`. Object instances should be lightweight projections and may decode lazily.

Recommended operations:

- inspect type and region metadata;
- obtain field/element by name or index;
- decode primitive or aggregate values;
- query offsets, encoded sizes, and presence;
- validate constraints and checksums;
- emit structured diagnostics;
- serialize to a neutral tree representation;
- mutate through a writable view when supported.

## Error model

Do not collapse failures into a single parse error. Distinguish:

- schema incompatibility;
- unavailable/truncated bytes;
- range or arithmetic overflow;
- invalid discriminator;
- failed constraint;
- checksum mismatch;
- encoding failure;
- policy limit exceeded;
- unsupported random access;
- mutation conflict.

## Lazy evaluation

Cache only immutable derived values or cache with explicit invalidation on mutation. Do not allow lazy access to bypass global limits. Track traversal context for cycles and diagnostic paths.
