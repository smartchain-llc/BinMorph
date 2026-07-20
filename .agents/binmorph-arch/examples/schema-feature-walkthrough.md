# Example: Add a Counted Array Feature

Suppose a field declares an array whose count is read from an earlier integer field.

1. Add syntax for `countFrom` and retain its source span.
2. Resolve the referenced field symbol.
3. Validate that the reference is visible, integral, nonnegative, and not cyclic.
4. Normalize it to a typed `LayoutExpression`, not a raw string.
5. Let the layout engine classify the array as dynamically sized and calculate checked bounds.
6. Add runtime metadata and evaluate the count through the bounded expression evaluator.
7. Have generators expose a typed array view that calls runtime element access.
8. Add limits for maximum element count and total decoded bytes.
9. Add diagnostics for missing reference, wrong type, negative count, overflow, truncation, and policy limit.
10. Test parser, semantics, normalized golden output, boundary layout math, runtime decoding, generator compilation, and generic/generated parity.
11. Update domain, compilation-sequence, interpretation-sequence, and extension diagrams when interfaces changed.
