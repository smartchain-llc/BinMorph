# Optional AGENTS.md integration snippet

Add this brief instruction to the repository-level `AGENTS.md` when you want Codex to favor the skill for relevant refactors:

```markdown
## Legacy C++ testability

When free functions or class methods construct concrete I/O, syscall, parser, logger, network, persistence, clock, randomness, or thread collaborators inside their bodies, use the `$cpp11-testable-construction` skill. Preserve pure value temporaries. Require explicit ownership, a composition boundary, unit tests for success and failure paths, and branch-coverage review.
```

The skill description is the main implicit activation mechanism. This `AGENTS.md` entry reinforces repository-specific expectations without duplicating the full skill.
