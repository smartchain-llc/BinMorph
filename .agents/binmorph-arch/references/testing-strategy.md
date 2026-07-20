# Testing Strategy

## Test matrix

| Area | Required tests |
|---|---|
| Parser | valid syntax, malformed JSON, unknown keywords, source spans, dialect versions |
| Resolution | local/external refs, missing refs, cycles, duplicate symbols, import policy |
| Semantics | invalid types, expression typing, union discriminators, endian inheritance, constraints |
| Layout | exact offsets/sizes, alignment, overlays, dynamic lengths, checked arithmetic, property tests |
| Normalized IR | golden snapshots, canonical ordering, deterministic hash, serialization compatibility |
| BinaryView | zero/final bounds, truncation, overflow, permissions, stream behavior |
| Runtime | primitive/aggregate decoding, lazy access, diagnostics, limits, cycles, checksums |
| Generator | golden files, deterministic regeneration, capability rejection, compile tests |
| Parity | generic API vs every generated target on shared fixtures |
| Mutation | writable authorization, validation, dependent recomputation, rollback/atomicity |
| Registry | identity, version resolution, transitions, concurrency, cache correctness |
| Artifact | manifest validation, ABI mismatch, platform mismatch, tampering, loader isolation |
| CLI/API | command/DTO contracts, exit/status mapping, pagination, cancellation, auth policy |
| Security | fuzz schemas, fuzz binaries, resource exhaustion, compiler sandbox escape assumptions |

## Fixture policy

Maintain small canonical fixtures for each schema construct and composed fixtures for cross-feature behavior. Include valid, boundary-valid, truncated, malformed, and adversarial binaries.

## Coverage principle

Line coverage is secondary to semantic state-space coverage. Each new layout feature must test boundary math and generic/generated equivalence.
