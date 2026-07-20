# BinMorph Architecture Codex Skill

Install this directory as a Codex skill, preserving the directory name and all relative paths.

Repository-local installation:

```bash
mkdir -p .agents/skills
cp -R binmorph-arch .agents/skills/binmorph-arch
```

Personal installation may use `$HOME/.agents/skills/binmorph-arch` with the same directory layout.

Validate the package before installation:

```bash
python3 scripts/validate_skill.py
```

The skill activates for changes involving declarative binary schemas, normalized layout IR, safe binary interpretation, generated APIs, generators, registries, artifacts, CLI/daemon services, compatibility, testing, or UML architecture.
