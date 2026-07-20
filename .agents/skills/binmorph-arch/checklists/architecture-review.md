# Architecture Review Checklist

- [ ] Request is classified into a task route.
- [ ] Domain/application/infrastructure/adapter ownership is explicit.
- [ ] No outward dependency entered the domain.
- [ ] Normalized IR remains canonical and immutable.
- [ ] Generic and generated paths share semantics.
- [ ] Binary arithmetic and accesses are checked.
- [ ] Resource limits are specified for dynamic behavior.
- [ ] Diagnostics have stable codes and useful locations.
- [ ] Public API, ABI, DTO, IR, and schema compatibility are assessed separately.
- [ ] Generated outputs are deterministic.
- [ ] Required tests cover negative and boundary behavior.
- [ ] Relevant UML and architecture text are updated.
