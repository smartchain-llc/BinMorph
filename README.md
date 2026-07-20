# BinMorph Declarative Binary Data Architecture — UML Documentation Set

This package documents an executable/daemon that:

1. Accepts JSON schema files describing byte-oriented binary layouts.
2. Validates and compiles those schemas into an internal representation.
3. Generates a language-specific library exposing typed APIs for schema-defined regions.
4. Loads the generated library to interpret arbitrary binary inputs.
5. Supports both one-shot CLI execution and long-running daemon operation.

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

