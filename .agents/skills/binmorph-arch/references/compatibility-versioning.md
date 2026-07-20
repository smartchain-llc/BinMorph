# Compatibility and Versioning

Version independently:

- schema dialect;
- individual schema semantic version;
- normalized IR format;
- generator;
- runtime API and ABI;
- generated artifact;
- daemon API;
- plugin contract.

Compatibility is an explicit predicate over manifest fields, never a filename convention.

## Identity hierarchy

- Authored document hash identifies exact source content.
- Normalized schema hash identifies canonical semantics.
- Generation key combines normalized hash, generator version, target, and options.
- Artifact hash identifies exact package bytes.

Semantically equivalent source documents may share a normalized hash.

## Change classification

- Additive optional schema capability: potentially backward compatible.
- Changed offsets, widths, endian, discriminator, or constraints: binary semantic change.
- Normalized IR serialization change: IR version change unless canonically identical and backward-readable.
- Generated public symbol change: generated API compatibility review.
- Runtime ABI surface or layout change: ABI version review.
- Daemon DTO change: API version and contract review.
