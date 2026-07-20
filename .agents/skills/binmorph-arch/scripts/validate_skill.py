#!/usr/bin/env python3
from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SKILL = ROOT / "SKILL.md"


def main() -> int:
    errors: list[str] = []
    text = SKILL.read_text(encoding="utf-8")

    if not text.startswith("---\n"):
        errors.append("SKILL.md must start with YAML front matter")
    if "name: binmorph-architecture" not in text:
        errors.append("missing expected skill name")
    if "description:" not in text:
        errors.append("missing skill description")

    refs = sorted(set(re.findall(r"`((?:references|templates|checklists|examples)/[^`]+)`", text)))
    for relative in refs:
        if not (ROOT / relative).is_file():
            errors.append(f"missing referenced file: {relative}")

    plantuml = list((ROOT / "references" / "uml").glob("*.puml"))
    if len(plantuml) < 12:
        errors.append(f"expected at least 12 UML sources, found {len(plantuml)}")

    manifest = ROOT / "manifest.txt"
    listed = [line.strip() for line in manifest.read_text(encoding="utf-8").splitlines() if line.strip()]
    actual = sorted(str(path.relative_to(ROOT)) for path in ROOT.rglob("*") if path.is_file() and path != manifest)
    expected = sorted(item for item in listed if item != "manifest.txt")
    if actual != expected:
        missing = sorted(set(actual) - set(expected))
        stale = sorted(set(expected) - set(actual))
        if missing:
            errors.append("manifest missing: " + ", ".join(missing))
        if stale:
            errors.append("manifest stale: " + ", ".join(stale))

    if errors:
        print("Skill validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print(f"Skill validation passed: {len(actual) + 1} files, {len(plantuml)} UML diagrams")
    return 0


if __name__ == "__main__":
    sys.exit(main())
