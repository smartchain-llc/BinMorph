#!/usr/bin/env bash
set -euo pipefail

BINMORPH="$1"
TMPDIR="$(mktemp -d)"
trap 'rm -rf "$TMPDIR"' EXIT

SCHEMA="$TMPDIR/schema.json"
BINARY="$TMPDIR/sample.bin"
OUTPUT="$TMPDIR/out.json"

cat > "$SCHEMA" <<'JSON'
{
  "dialect": "binmorph.schema.v1",
  "name": "sample",
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

python3 - "$BINARY" <<'PY'
import pathlib
import sys
pathlib.Path(sys.argv[1]).write_bytes(bytes([
    0x42, 0x4d, 0x52, 0x46,
    0x34, 0x12,
    0xff, 0xfe,
    0x41, 0x42, 0x43,
]))
PY

"$BINMORPH" inspect --schema "$SCHEMA" --input "$BINARY" --output "$OUTPUT"

grep -q '"apiVersion": "binmorph.inspect.v1"' "$OUTPUT"
grep -q '"name": "magic"' "$OUTPUT"
grep -q '"rawHex": "424d5246"' "$OUTPUT"
grep -q '"name": "version"' "$OUTPUT"
grep -q '"value": 4660' "$OUTPUT"
grep -q '"name": "signed_delta"' "$OUTPUT"
grep -q '"value": -2' "$OUTPUT"
grep -q '"value": "ABC"' "$OUTPUT"
grep -q '"diagnostics": \[' "$OUTPUT"

STDIN_OUTPUT="$TMPDIR/stdin-out.json"
"$BINMORPH" inspect --schema "$SCHEMA" --input - --output "$STDIN_OUTPUT" < "$BINARY"
grep -q '"source": "stdin"' "$STDIN_OUTPUT"
grep -q '"value": 4660' "$STDIN_OUTPUT"

TRUNCATED="$TMPDIR/truncated.bin"
python3 - "$TRUNCATED" <<'PY'
import pathlib
import sys
pathlib.Path(sys.argv[1]).write_bytes(b"BMRF")
PY

if "$BINMORPH" inspect --schema "$SCHEMA" --input "$TRUNCATED" --output "$TMPDIR/truncated.json"; then
  echo "expected truncated input to fail" >&2
  exit 1
fi
grep -q '"code": "BM_BINARY_RANGE_UNAVAILABLE"' "$TMPDIR/truncated.json"
grep -q '"binaryOffset": 4' "$TMPDIR/truncated.json"

BAD_SCHEMA="$TMPDIR/bad-schema.json"
cat > "$BAD_SCHEMA" <<'JSON'
{
  "dialect": "binmorph.schema.v1",
  "root": {
    "type": "struct",
    "fields": [
      { "name": "version", "type": "u16", "offset": 0 }
    ]
  }
}
JSON

if "$BINMORPH" inspect --schema "$BAD_SCHEMA" --input "$BINARY" --output "$TMPDIR/bad-schema-out.json"; then
  echo "expected schema without explicit endianness to fail" >&2
  exit 1
fi
grep -q '"code": "BM_SCHEMA_ENDIAN_REQUIRED"' "$TMPDIR/bad-schema-out.json"
