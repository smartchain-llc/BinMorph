# Architecture Specification

## 1. System purpose

The system provides a declarative mechanism for describing binary formats with JSON schemas and then using those schemas to generate typed libraries and interpret arbitrary binary data.

The architecture separates four concerns:

1. **Schema authoring and validation** — syntax and semantic correctness of format definitions.
2. **Schema compilation** — conversion of JSON documents into a normalized intermediate representation.
3. **Library generation and loading** — generation, compilation, packaging, versioning, and loading of schema-specific APIs.
4. **Binary interpretation** — safe reading, validation, traversal, mutation, and serialization of binary objects.

## 2. Primary architectural style

The recommended design combines:

- Hexagonal architecture for isolating infrastructure.
- Compiler pipeline architecture for schema processing.
- Plugin architecture for code generators, schema dialects, storage backends, and protocol adapters.
- Immutable domain models for normalized schemas.
- Runtime type metadata for dynamic interpretation.
- Generated strongly typed facades for compile-time integration.

## 3. System boundary

### In scope

- JSON schema ingestion.
- Schema validation and normalization.
- Byte-region layout modeling.
- Primitive, aggregate, array, union, conditional, offset, alignment, and checksum constructs.
- Generated library production.
- Dynamic library loading or static package consumption.
- Binary input interpretation.
- CLI and daemon interfaces.
- Schema registry and artifact cache.
- Diagnostics, observability, and sandboxing.

### Out of scope

- General-purpose source-code compilation beyond generated bindings.
- Arbitrary executable code embedded in schemas.
- Unbounded runtime reflection into unrelated languages.
- Storage-specific binary acquisition beyond adapters.

## 4. Core domain concepts

### SchemaDocument
The original user-authored JSON document and metadata.

### NormalizedSchema
An immutable, canonical representation after validation, reference resolution, type checking, and layout calculation.

### RegionDefinition
A contiguous or logically addressable binary region. A region may be fixed-size, dynamically sized, repeated, conditional, overlaid, or referenced indirectly.

### TypeDefinition
A primitive, enum, bitfield, struct, array, union, pointer, string, opaque block, or computed type.

### LayoutExpression
A restricted expression used for lengths, offsets, conditions, discriminators, alignment, and versioning.

### GeneratedArtifact
A versioned library or source package produced from a normalized schema.

### BinaryView
A bounded view over binary memory or a stream. It enforces offsets, lengths, endianness, and access permissions.

### ObjectInstance
A runtime object representing a schema-defined region over a BinaryView.

### Diagnostic
A structured warning or error containing a code, severity, source location, schema path, binary offset, and remediation hint.

## 5. Major components

### Interface layer

- CLI Adapter
- HTTP/gRPC Daemon Adapter
- Optional language SDKs

### Application layer

- Schema Service
- Generation Service
- Interpretation Service
- Registry Service
- Job Coordinator

### Domain layer

- Schema compiler pipeline
- Layout engine
- Type system
- Binary object model
- Validation rules
- Diagnostic model

### Infrastructure layer

- File system repository
- Schema registry database
- Artifact cache
- Compiler toolchain adapter
- Dynamic loader adapter
- Observability adapter
- Sandbox/process isolation adapter

## 6. Dependency rules

1. Domain code must not depend on CLI, HTTP, file system, compiler, database, or dynamic loader implementations.
2. Application services depend on domain abstractions and ports.
3. Infrastructure implements ports defined inward.
4. Generated libraries depend only on a stable runtime ABI/API package, never on daemon internals.
5. Schema-specific generated code must be reproducible from the normalized schema and generator version.
6. Runtime interpretation must work without generated code through the generic metadata API.
7. Strongly typed generated APIs must delegate bounds checking and primitive decoding to the runtime library.

## 7. Canonical workflows

### Schema registration

1. Receive schema document.
2. Parse JSON.
3. Validate syntax.
4. Resolve imports and references.
5. Validate semantic type rules.
6. Calculate layout constraints.
7. Produce NormalizedSchema.
8. Compute content hash and schema version.
9. Persist schema metadata.
10. Optionally trigger artifact generation.

### Library generation

1. Load NormalizedSchema.
2. Select generator plugin and target language.
3. Build target-neutral generation model.
4. Render source files.
5. Compile or package sources.
6. Run generated conformance tests.
7. Publish artifact and manifest.
8. Record generator/runtime compatibility metadata.

### Binary interpretation

1. Select schema and version.
2. Acquire binary source.
3. Create bounded BinaryView.
4. Instantiate root ObjectInstance.
5. Resolve lazy regions on access.
6. Decode fields with explicit byte order.
7. Validate constraints and checksums.
8. Return a dynamic tree, typed object, query result, or serialized representation.

### Implemented barebones CLI workflow

The initial executable implements the generic interpretation path only:

1. CLI adapter parses `binmorph inspect --schema <json> --input <binary|-> --output <json|->`.
2. Schema input is read under a fixed byte limit and parsed as JSON.
3. The schema compiler validates `binmorph.schema.v1` root struct fields and produces an immutable in-memory `NormalizedSchema`.
4. Binary input is read from a file or stdin under a fixed byte limit.
5. `BinaryView` performs checked range access for each normalized field.
6. The interpreter decodes supported primitive fields and emits the public `binmorph.inspect.v1` DTO.
7. Stable diagnostics are included in the DTO; any error diagnostic returns a non-zero process status.

This slice intentionally does not generate typed libraries, load artifacts, run as a daemon, or support dynamic expressions. Those surfaces should continue to use the same `NormalizedSchema` and runtime access rules rather than introducing separate decoding semantics.

### Implemented test seam

The CLI orchestration is split into:

- `binmorph_core`, containing parser, schema compiler, bounded binary view, runtime interpreter, output rendering, and command orchestration.
- `binmorph`, a thin production composition root that wires `FilesystemApplicationIO` to stdin/stdout/filesystem.
- `IApplicationIO`, a narrow port used by `runInspect` for schema input, binary input, and output writing.

This keeps external construction at the executable boundary. Unit tests inject GoogleMock implementations of `IApplicationIO` to verify success, schema-read failure, malformed schema short-circuiting, binary-read failure, help handling, and usage errors without depending on real files. File-backed CLI behavior remains covered by an end-to-end contract test.

## 8. Safety and correctness invariants

- Every read and write is range-checked.
- Integer overflow is checked during offset and size calculations.
- Recursive type expansion is bounded and cycle-aware.
- Dynamic lengths must be validated against configured limits.
- Schema expressions are declarative and sandboxed.
- Endianness is explicit at schema, type, or field scope.
- Generated code never performs unchecked pointer arithmetic.
- Artifact loading validates schema hash, ABI version, target platform, and generator version.
- Inputs are immutable by default; mutation requires an explicit writable view.
- Daemon jobs have CPU, memory, input-size, recursion-depth, and execution-time limits.

## 9. Versioning

The system should version independently:

- Schema dialect version.
- Individual schema semantic version.
- Normalized IR version.
- Generator version.
- Runtime ABI version.
- Generated artifact version.
- Daemon API version.

Compatibility is determined from a manifest, not inferred from filenames.

## 10. Recommended UML coverage

A complete architecture package should include:

- System context diagram.
- Use-case diagram.
- Container diagram.
- Component diagram.
- Domain class diagram.
- Schema compilation sequence.
- Binary interpretation sequence.
- CLI/daemon activity diagram.
- Schema lifecycle state machine.
- Code-generation pipeline diagram.
- Deployment diagram.
- Extension-point diagram.
