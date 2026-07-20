#!/usr/bin/env python3
"""Minimal offline validation for this Codex skill package."""

from __future__ import annotations

import re
import sys
from pathlib import Path


def fail(message: str) -> None:
    print(f"ERROR: {message}", file=sys.stderr)
    raise SystemExit(1)


def main() -> None:
    root = Path(sys.argv[1] if len(sys.argv) > 1 else ".").resolve()
    skill = root / "SKILL.md"
    if not skill.is_file():
        fail(f"missing {skill}")

    text = skill.read_text(encoding="utf-8")
    match = re.match(r"\A---\n(.*?)\n---\n", text, re.DOTALL)
    if not match:
        fail("SKILL.md must begin with YAML front matter")

    frontmatter = match.group(1)
    for field in ("name", "description"):
        if not re.search(rf"(?m)^{field}:\s*\S", frontmatter):
            fail(f"front matter missing non-empty '{field}'")

    references = re.findall(r"`(references/[^`]+\.md)`", text)
    missing = [path for path in references if not (root / path).is_file()]
    if missing:
        fail("missing referenced files: " + ", ".join(missing))

    print(f"OK: {skill}")
    print(f"OK: {len(references)} referenced documentation files")


if __name__ == "__main__":
    main()
