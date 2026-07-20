# BinMorph Architecture Codex Skill

Install this directory as a Codex skill, preserving the directory name and all relative paths.

Typical personal installation:

```bash
mkdir -p ~/.codex/skills
cp -R binmorph-architecture-skill ~/.codex/skills/binmorph-architecture
```

Repository-local installation may use the Codex skill directory convention adopted by that repository.

Validate the package before installation:

```bash
python3 scripts/validate_skill.py
```

The skill activates for changes involving declarative binary schemas, normalized layout IR, safe binary interpretation, generated APIs, generators, registries, artifacts, CLI/daemon services, compatibility, testing, or UML architecture.
