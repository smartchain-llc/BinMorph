# Service Boundaries

## Interface adapters

CLI, HTTP, gRPC, and SDK adapters own protocol concerns. They convert requests into application commands and convert results into versioned DTOs.

## Application services

- `SchemaService`: register, validate, normalize, inspect, and version schemas.
- `GenerationService`: generate, build, test, publish, and retrieve artifacts.
- `InterpretationService`: select schemas, acquire views, interpret/query/validate/mutate inputs.
- `RegistryService`: resolve schema and artifact identities and compatibility.
- `JobCoordinator`: schedule bounded asynchronous daemon jobs and expose status.

Application services define transaction and authorization boundaries but do not implement storage or transport.

## Ports

Typical inward-defined ports:

- schema repository;
- source/import resolver;
- artifact repository/cache;
- toolchain/compiler runner;
- process sandbox;
- dynamic artifact loader;
- binary source provider;
- clock and identity provider;
- metrics/tracing/audit sink.

## DTO rule

Do not expose domain entities directly. Public DTOs have independent versioning and map explicitly to domain commands/results.
