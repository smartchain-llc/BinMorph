# Decision Guide and Anti-Patterns

## Pattern selection

| Situation | Preferred seam |
|---|---|
| One existing collaborator used repeatedly | Constructor injection |
| Dependency only applies to one operation | Parameter injection |
| One transient product with simple creation | Injected callable or Factory Method |
| Related transient products selected together at runtime | Abstract Factory |
| Product family fixed at compile time | Static factory/policy |
| Runtime plugin boundary | Abstract Factory/interface adapter |
| Performance-sensitive internal algorithm | Static policy, after measurement |
| Unable to edit production source temporarily | Link seam/wrapper as transition only |

## Runtime Abstract Factory strengths

- Runtime family selection.
- Stable consumer type and ABI-friendly interface boundary.
- Concrete constructors hidden from consumers.
- Straightforward mocking and fault injection.
- Product ownership can be explicit with smart pointers.

## Runtime costs

- Interface and factory boilerplate.
- Virtual calls.
- Often heap allocation.
- Invalid family combinations may only surface at runtime unless construction is tightly controlled.

## Compile-time factory strengths

- Static contract checking.
- No required virtual dispatch or allocation.
- Products can be returned by value.
- Compiler optimization across abstraction boundaries.

## Compile-time costs

- Template implementation exposure.
- Longer rebuilds and potentially larger binaries.
- Harder runtime selection and ABI stability.
- Coverage reports can contain multiple instantiations.

## Anti-patterns

### Service locator disguised as a factory

```cpp
Services::get<IFileSystem>();
```

Dependencies remain hidden and tests mutate global state.

### Factory for every value

Do not create factories for strings, vectors, DTOs, iterators, or deterministic helper values merely to increase abstraction count.

### God factory

An `IEverythingFactory` coupling databases, UI, networking, clocks, logging, and parsing is a service locator with creation methods. Split by cohesive subsystem.

### Mocking implementation details

Prefer behavioral assertions. Verify call order only when order is part of the contract.

### Mixed ownership

Do not return raw owning pointers. Use `unique_ptr`, values, or an explicit project ownership type.

### Default constructor that secretly owns dependencies

Avoid classes with one constructor that borrows dependencies and another that silently allocates them internally through complicated nullable member state. Prefer an external composition root or named production builder.
