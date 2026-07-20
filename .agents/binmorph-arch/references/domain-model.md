# Domain Model

## Canonical entities and values

- `SchemaDocument`: authored JSON plus source identity and dialect version.
- `NormalizedSchema`: immutable canonical model after complete validation and layout analysis.
- `TypeDefinition`: primitive, enum, bitfield, struct, array, union, pointer/offset reference, string, opaque, or computed type.
- `RegionDefinition`: named or anonymous bounded byte region with layout and presence rules.
- `LayoutExpression`: restricted expression for offset, length, alignment, condition, discriminator, or version selection.
- `BinaryView`: bounded random-access or streaming view with explicit permissions.
- `ObjectInstance`: runtime projection of a normalized type/region over a `BinaryView`.
- `Diagnostic`: stable code, severity, message, schema location, binary location, related spans, and remediation.
- `GeneratedArtifact`: immutable artifact manifest and content identity.

## Modeling rules

- Use value objects for offsets, sizes, alignment, byte order, bit ranges, schema IDs, and version IDs.
- Distinguish unknown size, dynamic size, and invalid/unbounded size.
- Represent optionality and condition separately from nullability.
- Distinguish sequential layout from explicit offset layout and overlay/union layout.
- Resolve names and references before layout calculation.
- Preserve source spans in normalized nodes for diagnostics without retaining parser AST ownership.
- Do not store host pointers in normalized IR.
- Do not encode target-language types in normalized IR.

## Suggested interfaces

```text
ISchemaParser.parse(document) -> ParsedSchema
IReferenceResolver.resolve(parsed, repository) -> ResolvedSchema
ISemanticValidator.validate(resolved) -> ValidatedSchema
ILayoutEngine.normalize(validated) -> NormalizedSchema
IBinaryInterpreter.instantiate(schema, view) -> ObjectInstance
ICodeGenerator.generate(schema, options) -> GeneratedSourceSet
IArtifactBuilder.build(sourceSet, toolchain) -> GeneratedArtifact
```

Intermediate types may be internal, but the stage boundaries should remain explicit and testable.
