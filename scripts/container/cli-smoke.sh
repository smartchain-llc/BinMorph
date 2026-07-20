#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
    echo "usage: $0 <binmorph-binary> <output-json>" >&2
    exit 64
fi

BINMORPH="$1"
OUTPUT_JSON="$2"
TMPDIR="$(mktemp -d)"
trap 'rm -rf "${TMPDIR}"' EXIT

SCHEMA="${TMPDIR}/schema.json"
BINARY="${TMPDIR}/sample.bin"
TRUNCATED="${TMPDIR}/truncated.bin"

cat >"${SCHEMA}" <<'JSON'
{
  "dialect": "binmorph.schema.v1",
  "name": "docker-smoke",
  "endianness": "little",
  "root": {
    "type": "struct",
    "fields": [
      { "name": "magic", "type": "bytes", "offset": 0, "size": 4 },
      { "name": "version", "type": "u16", "offset": 4 },
      { "name": "signed_delta", "type": "i16", "offset": 6, "endian": "big" },
      { "name": "label", "type": "ascii", "offset": 8, "size": 3 }
    ]
  }
}
JSON

python3 - "${BINARY}" <<'PY'
import pathlib
import sys

pathlib.Path(sys.argv[1]).write_bytes(bytes([
    0x42, 0x4d, 0x52, 0x46,
    0x34, 0x12,
    0xff, 0xfe,
    0x41, 0x42, 0x43,
]))
PY

"${BINMORPH}" inspect --schema "${SCHEMA}" --input "${BINARY}" --output "${OUTPUT_JSON}"
grep -q '"apiVersion": "binmorph.inspect.v1"' "${OUTPUT_JSON}"
grep -q '"name": "version"' "${OUTPUT_JSON}"
grep -q '"value": 4660' "${OUTPUT_JSON}"
grep -q '"value": "ABC"' "${OUTPUT_JSON}"

python3 - "${TRUNCATED}" <<'PY'
import pathlib
import sys

pathlib.Path(sys.argv[1]).write_bytes(b"BMRF")
PY

if "${BINMORPH}" inspect --schema "${SCHEMA}" --input "${TRUNCATED}" --output "${TMPDIR}/truncated.json"; then
    echo "expected truncated input to fail" >&2
    exit 1
fi
grep -q '"code": "BM_BINARY_RANGE_UNAVAILABLE"' "${TMPDIR}/truncated.json"
