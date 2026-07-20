# BinMorph Declarative Binary Data Architecture — UML Documentation Set

This package documents an executable/daemon that:

1. Accepts JSON schema files describing byte-oriented binary layouts.
2. Validates and compiles those schemas into an internal representation.
3. Generates a language-specific library exposing typed APIs for schema-defined regions.
4. Loads the generated library to interpret arbitrary binary inputs.
5. Supports both one-shot CLI execution and long-running daemon operation.

## Barebones CLI implementation

This repository now includes a first vertical slice of the one-shot CLI:

```bash
cmake --preset projects
cmake --build --preset projects
./build/projects/binmorph inspect --schema schema.json --input data.bin --output architecture.json
```

Use `--input -` or `--output -` for stdin/stdout.

Supported schema dialect:

```json
{
  "dialect": "binmorph.schema.v1",
  "name": "example",
  "endianness": "little",
  "root": {
    "type": "struct",
    "fields": [
      { "name": "magic", "type": "bytes", "offset": 0, "size": 4 },
      { "name": "version", "type": "u16", "offset": 4 },
      { "name": "label", "type": "ascii", "offset": 6, "size": 3 }
    ]
  }
}
```

Supported field types are `u8/u16/u32/u64`, `i8/i16/i32/i64`, `bytes`, and `ascii`.
Multi-byte integer endianness must be explicit at schema level through `endianness`
or at field level through `endian`.

The output DTO is `binmorph.inspect.v1` JSON containing schema metadata, binary
source/size, decoded root fields, raw bytes as hex, and structured diagnostics.

Run tests with:

```bash
ctest --preset projects
```

## Intended audiences

### Users
Users need to understand the supported workflows, schema lifecycle, input/output behavior, error handling, generated artifacts, and daemon interaction model.

Recommended diagrams:
- `01-system-context.puml`
- `02-user-use-cases.puml`
- `08-cli-daemon-activity.puml`
- `11-deployment.puml`

### Developers
Developers need component boundaries, domain types, lifecycle rules, extension points, sequencing, ownership, failure behavior, and deployment assumptions.

Recommended diagrams:
- `03-container-architecture.puml`
- `04-component-architecture.puml`
- `05-domain-model.puml`
- `06-schema-compilation-sequence.puml`
- `07-binary-interpretation-sequence.puml`
- `09-schema-state-machine.puml`
- `10-code-generation.puml`
- `12-extension-points.puml`

### Codex agents
Codex needs explicit architectural constraints, dependency direction, invariants, canonical workflows, modification boundaries, and traceability from requirements to components.

Recommended inputs:
- All UML files
- `ARCHITECTURE.md`
- `CODEX_GUIDANCE.md`

## Rendering

All diagrams use PlantUML syntax.

```bash
plantuml uml/*.puml
```

For SVG output:

```bash
plantuml -tsvg uml/*.puml
```
