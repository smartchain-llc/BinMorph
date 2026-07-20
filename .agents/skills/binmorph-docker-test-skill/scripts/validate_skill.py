#!/usr/bin/env python3
from __future__ import annotations

import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
required = [
    ROOT / "SKILL.md",
    ROOT / "README.md",
    ROOT / "references/dockerfile-design.md",
    ROOT / "references/cmake-ctest-contract.md",
    ROOT / "templates/docker/Dockerfile",
    ROOT / "templates/compose/compose.test.yml",
    ROOT / "scripts/run-tests.sh",
    ROOT / "checklists/review.md",
]
missing = [str(p.relative_to(ROOT)) for p in required if not p.is_file()]
if missing:
    print("Missing required files:", *missing, sep="\n- ", file=sys.stderr)
    raise SystemExit(1)

skill = (ROOT / "SKILL.md").read_text(encoding="utf-8")
if not skill.startswith("---\n") or "name: binmorph-docker-test" not in skill:
    print("SKILL.md front matter is invalid", file=sys.stderr)
    raise SystemExit(1)

scripts = list((ROOT / "scripts").glob("*.sh"))
for script in scripts:
    text = script.read_text(encoding="utf-8")
    if "set -euo pipefail" not in text:
        print(f"{script.name} lacks strict shell mode", file=sys.stderr)
        raise SystemExit(1)

files = [p for p in ROOT.rglob("*") if p.is_file()]
print(f"Skill validation passed: {len(files)} files")
