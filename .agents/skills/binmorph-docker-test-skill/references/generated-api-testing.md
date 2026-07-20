# Generated API Testing

A generator test is incomplete if it only compares text output.

Required layers:

1. Golden output for stable representative fragments.
2. Compile generated code with warnings enabled.
3. Install/export runtime dependencies as a consumer would receive them.
4. Compile an external consumer in a fresh build tree.
5. Decode fixtures through both generated and generic APIs.
6. Compare values and diagnostics.
7. Exercise unsupported schema constructs and compatibility rejection.
8. Run generated consumers under applicable sanitizers.

Generated code must never bypass runtime bounds checks or duplicate primitive decoding semantics.
