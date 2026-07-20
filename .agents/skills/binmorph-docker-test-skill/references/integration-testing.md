# Integration Testing

## Daemon suite

Start the daemon with a temporary registry, isolated artifact directory, deterministic configuration, and an ephemeral endpoint. Readiness must verify a real health or protocol operation.

Test:
- schema registration and validation;
- binary interpretation request;
- generated artifact request when supported;
- malformed schema and malformed binary diagnostics;
- clean shutdown and outstanding job handling;
- persistence/restart behavior only when persistence is in scope.

## CLI suite

Run the installed CLI, not only the build-tree executable. Verify exit codes, stdout/stderr separation, stable diagnostic identifiers, and machine-readable output.

## Fixture ownership

Keep small fixtures in source control. Generate large deterministic fixtures during the test. Never download mutable fixtures without checksum pinning.
