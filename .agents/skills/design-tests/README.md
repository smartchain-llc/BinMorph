# C++11 Testable Construction Codex Skill

This skill guides Codex through refactoring hard-coded temporary dependencies into runtime Abstract Factories, compile-time factory policies, or simpler injection seams. It includes Mermaid UML and testing/coverage references.

## Repository installation

Copy the skill folder to:

```text
<repository>/.agents/skills/cpp11-testable-construction/
```

Codex scans `.agents/skills` from the current working directory toward the repository root.

## User installation

Copy it to:

```text
$HOME/.agents/skills/cpp11-testable-construction/
```

## Invocation

Explicit:

```text
$cpp11-testable-construction refactor this function so its local socket, parser, and logger are testable
```

Implicit examples:

- “Refactor these concrete local dependencies so I can unit test failure paths.”
- “Introduce an abstract factory for this C++11 subsystem.”
- “Add a compile-time factory contract with SFINAE and coverage tests.”
- “The function constructs `ifstream` and a concrete parser internally; create a test seam.”

## Validate package

```bash
python3 scripts/validate_skill.py .
```
