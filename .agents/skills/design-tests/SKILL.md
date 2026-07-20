---
name: design-tests
summary: Implementing and refactoring existing source to obtain a more testable runtime or compile-time seams.
description: Use for repository C++23 source code where software components violate a clean, testable, or orthogonal design for example, if the source has construct concrete temporary dependencies, making unit tests, mocks, fault injection, or code coverage difficult. Applies Abstract Factory, Factory Method, dependency injection, compile-time factory policies, SFINAE contract checks, composition roots, and branch-coverage tests. Do not trigger for ordinary value temporaries, pure algorithms, or simple data objects without external effects.
---

# C++ Testable Construction Refactoring

Use this skill when code constructs ill designed architecture. For example, collaborators inside a function body and those collaborators perform I/O, syscalls, parsing, logging, networking, persistence, time, randomness, threads, or other externally observable behavior.

## Required workflow

1. Inspect the target function and classify each local object:
   - Keep deterministic value objects local.
   - Extract externally interacting or policy-bearing collaborators.
   - Distinguish reusable dependencies from per-call resources.
2. Identify the narrowest behavioral abstraction required by the consumer. Do not mirror an entire concrete API unless the consumer genuinely needs it.
3. Choose a seam:
   - Parameter injection for a dependency used only by one call.
   - Constructor injection for a required, reusable dependency.
   - Runtime Abstract Factory for related products, runtime selection, ABI boundaries, or plugin families.
   - Compile-time factory/policy for fixed product families, value semantics, no virtual dispatch, or embedded/performance-sensitive code.
   - A hybrid adapter when runtime selection and compile-time implementations are both required.
4. Preserve existing callers with a thin production wrapper when incremental migration is needed.
5. Move concrete construction toward a composition root. Do not replace hard-coded construction with a service locator or mutable global registry.
6. Add tests before removing the legacy seam:
   - Success path.
   - Each failure and short-circuit path.
   - Factory creation parameters and creation counts when relevant.
   - Product lifetime and cleanup when relevant.
   - Null/exception policy if the runtime factory contract permits failure.
   - Positive and negative compile-time contract tests for static factories.
7. Run the repository's formatter, compiler, unit tests, and coverage workflow. Report line, function, and branch coverage separately when available.

## Design rules

### Testing Requirements

- The Google Test framework should be used for tests and mocks.
- Determine if the gtest libraries are installed and if not then fetch the content.
- Tests should reside in a 'tests' directory
- Tests should use mocks as much as possible, where determined to be appropriate.

### Implementations

- Static polymorphism should be used for potential critical sections where execution time matters.
- If the static polymorphism implementations are too messy or hard to comphrehend for new developers then utilitze runtime polymorphism.
- Business logic uses dependencies; composition code creates dependencies.
- Creating a temporary is acceptable. Choosing a concrete external dependency inside business logic is the problem.
- Inject the highest-level capability the consumer actually needs.
- Use `std::unique_ptr<Base>` when a runtime factory transfers ownership of a polymorphic product.
- Prefer non-null-by-contract factories. If null is possible, document and test it explicitly.
- Avoid factories for `std::string`, containers, DTOs, and deterministic algorithm helpers.
- Keep factories cohesive. Split unrelated product families rather than creating an `IEverythingFactory`.
- In C++11, use explicit `std::unique_ptr<T>(new T(...))` or a local `make_unique` helper.
- For compile-time factories, validate both creation functions and returned product operations with C++11 detection traits and `static_assert`.
- Compile-time tests do not contribute executed gcov lines; pair them with runtime behavioral tests.

## Select references progressively

Read only the references needed for the current task:

- Runtime Abstract Factory design and implementation: `references/runtime-abstract-factory.md`
- Compile-time factory policies and SFINAE contracts: `references/compile-time-factory.md`
- UML diagrams and architectural relationships: `references/uml.md`
- Unit, integration, compile-failure, and coverage testing: `references/testing-and-coverage.md`
- Incremental legacy migration procedure: `references/refactoring-workflow.md`
- Pattern selection and anti-patterns: `references/decision-guide.md`

## Expected output for implementation tasks

Provide or implement:

1. A brief diagnosis of the current construction coupling.
2. The selected seam and why it fits lifecycle and selection requirements.
3. Product interfaces or compile-time contracts.
4. Production factory/policy implementation.
5. Consumer refactor with ownership and lifetime explicit.
6. Test doubles and tests covering all meaningful branches.
7. Build-system changes for tests and coverage.
8. Updated UML when architecture changes.
9. Migration notes for unchanged legacy callers.

Do not claim complete coverage merely from line coverage. Inspect branch data, especially short-circuit Boolean expressions, exceptions, factory failures, and multiple template instantiations.
