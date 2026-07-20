# Test Matrix

Use risk-based dimensions.

## Required core matrix

| Dimension | Minimum |
|---|---|
| Compiler | Supported GCC and Clang major versions |
| Build type | Debug and Release |
| Tests | Unit, component, integration |
| Binary modes | Generic interpreter and generated API |
| Input classes | Valid, truncated, malformed, overflow-inducing |

## Specialized lanes

- ASan + UBSan: every pull request where runtime permits.
- TSan: scheduled or for concurrency-sensitive changes.
- Coverage: one compiler/build lane; do not multiply coverage lanes without purpose.
- Fuzz: scheduled and before releases; short smoke fuzzing may run on pull requests.
- Oldest supported dependency set: scheduled compatibility lane.
- Architecture lanes: native amd64 plus arm64 when officially supported.

Do not combine every dimension. Each lane needs a named risk it covers.
