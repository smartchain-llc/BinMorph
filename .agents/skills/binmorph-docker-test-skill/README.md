# BinMorph Docker Test Codex Skill

A companion skill for building and testing BinMorph through reproducible Docker and Docker Compose workflows.

## Install

Copy this directory to a Codex skills directory, for example:

```bash
mkdir -p .agents/skills
cp -R binmorph-docker-test-skill .agents/skills/binmorph-docker-test-skill
```

Personal installation may use `$HOME/.agents/skills/binmorph-docker-test-skill`.
The directory name may differ, but `SKILL.md` must remain at the skill root.

## Validate package

```bash
python3 scripts/validate_skill.py
```

## Scope

This skill covers development and CI test containers. It intentionally distinguishes those images from production daemon deployment images.
