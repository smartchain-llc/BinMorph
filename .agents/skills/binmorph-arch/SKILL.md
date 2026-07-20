---
name: binmorph-arch
description: Design, implement, review, test, and document BinMorph-style declarative binary schema systems that compile JSON format definitions into normalized layouts, generated typed libraries, and safe runtime binary interpreters. Use for schema language features, layout engines, binary views, code generators, CLI or daemon APIs, registries, artifact loading, compatibility, security, testing, or related UML updates.
---

# BinMorph Architecture Skill

Use this skill for work on a declarative binary data structure definition system whose schemas describe byte regions, generate typed APIs, and interpret arbitrary binary inputs.

## Architectural objective

Preserve one canonical semantic model across three execution surfaces:

1. Schema compilation into an immutable normalized intermediate representation.
2. Generic runtime interpretation through metadata and bounded binary views.
3. Generated, strongly typed APIs that delegate decoding and safety checks to the runtime.

Do not create separate semantics for generated and dynamic paths.

## Start here

1. Classify the request using **Task routing** below.
2. Read only the referenced files needed for that task.
3. Identify affected contracts and invariants before editing code.
4. Implement the smallest coherent vertical slice.
5. Add all tests required by the applicable test matrix.
6. Update UML whenever responsibilities, interfaces, dependencies, workflows, lifecycle, or deployment change.

## Non-negotiable invariants

- Domain code has no dependency on CLI, HTTP, database, filesystem, process, compiler, or dynamic-loader implementations.
- `NormalizedSchema` is immutable and is the sole input to layout, generation, registry publication, and runtime metadata construction.
- Every binary access is bounds checked.
- Offset and size arithmetic is overflow checked before use.
- Endianness is explicit and never inferred from host architecture.
- Schema expressions are declarative, deterministic, bounded, and sandboxed.
- Generated code performs no unchecked pointer arithmetic and delegates primitive access to the stable runtime API.
- Generic interpretation remains available without generated code.
- Artifacts are loaded only after schema hash, IR version, generator version, runtime ABI, target, and platform compatibility are validated.
- Equivalent normalized schema, generator version, and options produce deterministic output.
- Diagnostics use stable codes and include source/schema path or binary offset where applicable.

## Task routing

### Add or change schema syntax

Read:
- `references/task-routing.md` → Schema language feature
- `references/domain-model.md`
- `references/testing-strategy.md`
- `references/uml/06-schema-compilation-sequence.puml`

Expected impact:
parser → reference resolution → semantic validation → normalized IR → layout engine → runtime metadata → affected generators → conformance tests.

Never let a parser-specific node escape into downstream services.

### Add or change a binary type, region, or layout rule

Read:
- `references/domain-model.md`
- `references/binary-safety.md`
- `references/testing-strategy.md`
- `references/uml/05-domain-model.puml`

Model the construct in normalized IR first. Specify size, alignment, endianness, presence, repetition, offset, and failure behavior explicitly.

### Add a code generator or generated API feature

Read:
- `references/code-generation.md`
- `references/compatibility-versioning.md`
- `references/testing-strategy.md`
- `references/uml/10-code-generation.puml`

Implement an `ICodeGenerator` plugin. Generated APIs must be facades over the runtime, not independent decoders.

### Change runtime interpretation or mutation

Read:
- `references/runtime-interpretation.md`
- `references/binary-safety.md`
- `references/testing-strategy.md`
- `references/uml/07-binary-interpretation-sequence.puml`

Preserve dynamic/generated parity. Mutation requires an explicitly writable view and must revalidate impacted constraints.

### Change CLI, daemon, jobs, or transport APIs

Read:
- `references/service-boundaries.md`
- `references/operations-security.md`
- `references/testing-strategy.md`
- `references/uml/08-cli-daemon-activity.puml`
- `references/uml/11-deployment.puml`

Adapters map transport DTOs to application commands. Never serialize domain entities directly as public DTOs.

### Change schema registry, artifact cache, loading, or versioning

Read:
- `references/compatibility-versioning.md`
- `references/service-boundaries.md`
- `references/operations-security.md`
- `references/uml/09-schema-state-machine.puml`

Treat manifests as authoritative. Filenames are not compatibility metadata.

### Review or refactor architecture

Read:
- `references/architecture.md`
- `references/service-boundaries.md`
- `checklists/architecture-review.md`
- relevant UML diagrams

Reject dependency inversions, duplicated semantic models, unbounded schema behavior, and generator-specific logic in core domain services.

## Required implementation workflow

### 1. Establish contracts

State:
- input and output contracts;
- normalized IR changes;
- public API or ABI changes;
- diagnostic codes;
- limits and failure behavior;
- compatibility implications.

### 2. Implement inward-out

Preferred order:

1. Domain value types and invariants.
2. Domain service or port contracts.
3. Application orchestration.
4. Infrastructure adapters.
5. CLI/transport adapters.
6. Generated surface changes.

### 3. Verify parity

For every supported construct, compare the same binary fixture through:

- generic metadata API;
- generated API;
- serialization or mutation path, when applicable.

Results and diagnostics must agree.

### 4. Update architecture artifacts

Update PlantUML when a change affects:
- component ownership;
- dependencies;
- public interfaces;
- important sequences;
- schema or job lifecycle;
- deployment topology;
- extension points.

Use `checklists/uml-change-checklist.md`.

## Formatting Files

### VSCode extension

- Update the vscode extension recommendations for this project if any extension requirements have changed.
- Check if a C/C++ formatter is installed
  - If a formatter is found then format all c++ files with it.
  - If no formatter is found then install the C/C++ formatter from microsft.

## Testing floor

At minimum, select all applicable categories from `references/testing-strategy.md`:

- parser and semantic validation unit tests;
- normalized IR golden tests;
- layout property and boundary tests;
- binary view overflow and truncation tests;
- generator golden and compile tests;
- dynamic/generated equivalence tests;
- artifact compatibility tests;
- CLI or daemon contract tests;
- schema and binary fuzz tests.

A happy-path unit test alone is never sufficient for a new binary construct.

## Output expectations for Codex tasks

When proposing or completing work, report:

1. Task classification.
2. Architectural contracts affected.
3. Files/components changed.
4. Safety and compatibility implications.
5. Tests added and their coverage purpose.
6. UML or documentation updated.
7. Remaining risks or unsupported cases.

Use `templates/change-plan.md` before substantial changes and `templates/change-summary.md` after implementation.
