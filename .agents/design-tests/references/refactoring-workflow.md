# Incremental Refactoring Workflow

## 1. Characterize existing behavior

Before changing construction, add characterization tests around the current public entry point where possible. Record outputs, error codes, logging behavior, and side effects.

## 2. Inventory local construction

For every local object, record:

- Is it deterministic?
- Does it touch external state?
- Is construction a policy decision?
- Is it reusable or per-call?
- Does the consumer require the full concrete API?
- Who should own it?

Keep pure values local. Extract only meaningful collaborators.

## 3. Extract a testable core

Legacy free function:

```cpp
bool transmit(const Message& message) {
    TcpSocket socket;
    JsonSerializer serializer;
    return transmit_impl(message, socket, serializer);
}

bool transmit_impl(const Message& message,
                   ISocket& socket,
                   ISerializer& serializer) {
    return socket.send(serializer.serialize(message));
}
```

Test `transmit_impl` first. This creates a seam without changing existing callers.

## 4. Select lifecycle mechanism

- Existing reusable object: constructor injection.
- Per-call caller-provided context: parameter injection.
- Per-call resource with runtime implementation selection: runtime factory.
- Fixed implementation with static substitution: compile-time factory.

## 5. Introduce a production wrapper

```cpp
bool process_request(const Request& request) {
    ProductionRuntimeFactory factory(std::clog, "server", 443);
    return process_request(request, factory);
}

bool process_request(const Request& request,
                     const IRuntimeFactory& factory) {
    return RequestProcessor(factory).process(request);
}
```

The wrapper may construct dependencies but should contain no business branching.

## 6. Move creation to a composition root

As callers migrate, create and wire long-lived factories at application startup or subsystem initialization. Pass references downward.

## 7. Remove transitional seams carefully

Only remove the compatibility wrapper when all callers use the new composition path and production integration tests exist.

## Review checklist

- No service locator introduced.
- No mutable global function pointers introduced as final architecture.
- Interfaces are consumer-shaped, not concrete-class mirrors.
- Factory products form a cohesive family.
- Ownership is visible in types.
- Destructors are virtual for polymorphic deletion.
- Tests cover failure ordering and skipped downstream calls.
- Production factory configuration is tested.
- UML and architecture documentation match the implementation.
