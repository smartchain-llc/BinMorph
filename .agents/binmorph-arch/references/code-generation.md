# Code Generation

## Generator contract

An `ICodeGenerator` consumes only `NormalizedSchema`, generator options, and a stable target runtime contract. It returns a deterministic source set plus a manifest.

The manifest records:

- source schema identity and content hash;
- normalized IR version and hash;
- generator name and version;
- generator options hash;
- runtime API/ABI requirement;
- target language and language version;
- target platform and architecture where applicable;
- declared schema capabilities;
- source and artifact hashes.

## Generated API design

Generated types should provide ergonomic names and compile-time types while delegating:

- range checking;
- endian conversion;
- primitive decoding/encoding;
- string validation;
- array bounds;
- reference traversal;
- diagnostics;
- mutation primitives

to the runtime library.

## Tests

Every generator requires:

1. Golden source tests.
2. Repeat-generation byte comparison.
3. Generated source compilation.
4. Runtime fixture tests.
5. Generic/generated parity tests.
6. Unsupported-capability diagnostics.
7. Compatibility manifest tests.
