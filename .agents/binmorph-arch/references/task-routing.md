# Task Routing

## Schema language feature

Touch the complete semantic path:

1. JSON syntax/schema definition.
2. Parser AST local to the parser boundary.
3. Import and reference resolution.
4. Semantic type validation.
5. Canonical normalized IR.
6. Layout constraint calculation.
7. Runtime metadata representation.
8. Each generator that declares support.
9. Diagnostics and compatibility metadata.
10. Conformance, negative, parity, and fuzz tests.

A generator may explicitly reject an unsupported capability, but it must do so deterministically with a stable diagnostic.

## Generator feature

Keep target concerns outside normalized IR. Add target capability negotiation rather than target conditionals in the schema compiler. Verify source determinism, source compilation, runtime behavior, and runtime ABI compatibility.

## Runtime decoding feature

Implement primitive behavior once in the runtime. Generated code should call that implementation. Test exact boundaries: zero length, final valid byte, one-byte truncation, integer overflow, malicious length, cyclic references, maximum recursion, and unexpected discriminator.

## Service/API feature

Transport adapters own authentication, request decoding, DTO validation, status mapping, and protocol versioning. Application services own use-case orchestration. Domain code owns semantic rules. Infrastructure owns persistence, compilation, process execution, and dynamic loading.

## Registry/artifact feature

Maintain separate identities for source schema, normalized schema, generated sources, compiled artifact, and deployment registration. Every transition must be observable and recoverable.
