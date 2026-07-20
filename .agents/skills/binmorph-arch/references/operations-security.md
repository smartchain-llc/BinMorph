# Operations and Security

## Daemon limits

Enforce configurable limits for schema bytes, binary bytes, imports, recursion, object count, expression steps, generated source size, compiler duration, job duration, memory, CPU, open files, and artifact size.

## Isolation

Treat schemas and binary inputs as untrusted. Run external compilers and generated tests in a sandbox with restricted filesystem, network, privileges, environment, and resource limits. Never load untrusted native artifacts into the main daemon process without an explicit isolation design.

## Observability

Record correlation ID, schema identity, normalized hash, generator/runtime versions, job state, input size, elapsed time, limit failures, diagnostic codes, and artifact identity. Avoid logging binary contents or secrets by default.

## Lifecycle

Suggested job states:

```text
Accepted -> Validating -> Normalized -> Generating -> Building -> Testing
         -> Published -> Completed
```

Any active state may transition to `Failed`, `Cancelled`, or `Expired`, with structured reason and retained diagnostics.
