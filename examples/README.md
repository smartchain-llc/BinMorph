# BinMorph Examples

Build and run the step-through example:

```bash
cmake -S . -B /tmp/binmorph-build -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
cmake --build /tmp/binmorph-build
/tmp/binmorph-build/examples/binmorph_step_through
```

The example walks through the library the same way a host project would:

- load a JSON binary layout
- parse and validate the schema
- compile an execution plan
- parse an arbitrary byte buffer
- inspect mapped field values and diagnostics
- export a JSON projection
