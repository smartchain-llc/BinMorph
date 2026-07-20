# Codex Architecture Guidance

## Mission

Use this architecture when modifying or extending the declarative binary schema compiler, code generator, runtime library, CLI, or daemon.

## Mandatory constraints

1. Preserve inward dependency direction.
2. Do not place file-system, HTTP, database, compiler, process, or dynamic-loader logic in domain classes.
3. Keep `NormalizedSchema` immutable.
4. Add new schema syntax through parser and semantic-validation stages; do not bypass normalization.
5. Add new generated language targets through `ICodeGenerator` plugins.
6. Reuse the runtime bounds-checking API from generated code.
7. Never generate unchecked pointer arithmetic or unbounded loops from schema content.
8. Add structured diagnostics with stable diagnostic codes.
9. Maintain deterministic generation: identical normalized schema + generator version + options must produce byte-identical source output where feasible.
10. Update UML and conformance tests when component responsibilities, public interfaces, or workflows change.

## Task routing rules

### Schema feature task
Modify:
- Schema grammar/parser
- Semantic validator
- Normalized IR
- Layout engine where applicable
- Runtime metadata reader
- All affected generators
- Schema conformance tests

### New generator task
Implement:
- `ICodeGenerator`
- Target capability declaration
- Runtime compatibility manifest
- Golden-file tests
- Generated compile tests
- Generated runtime conformance tests

### Runtime decoding task
Modify:
- BinaryView abstractions
- Decoder strategy or primitive codec
- Bounds/overflow tests
- Dynamic and generated API parity tests

### Daemon API task
Modify:
- Transport adapter
- Application service DTO mapping
- API version contract
- Authentication/authorization policy where applicable
- Integration tests

Do not expose domain entities directly as network DTOs.

## Required tests

- Parser unit tests.
- Semantic validation unit tests.
- Layout calculation property tests.
- BinaryView boundary and overflow tests.
- Golden normalized-IR tests.
- Generator golden-file tests.
- Generated source compile tests.
- Generated API/runtime equivalence tests.
- CLI integration tests.
- Daemon contract tests.
- Artifact compatibility tests.
- Fuzz tests for schema input and binary input.

## Review checklist

- Does the change preserve deterministic behavior?
- Are all binary accesses bounded?
- Are offsets and sizes overflow-checked?
- Is schema-supplied behavior restricted to declarative expressions?
- Does the generic dynamic API remain functional?
- Does generated code remain compatible with the runtime ABI?
- Are diagnostics actionable and stable?
- Are new extension points represented as interfaces rather than conditionals in core services?
- Are UML diagrams and architecture text still accurate?

