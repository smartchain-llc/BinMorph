# Debugging Containerized Failures

## Reproduce exactly

Record image digest, target architecture, build arguments, preset names, source revision, and cache state. Re-run the failing test with verbose CTest output:

```bash
ctest --test-dir <build-dir> -R '<regex>' -VV --repeat until-fail:10
```

## Interactive diagnosis

Override the entrypoint and enter the same image/stage. Do not switch to a different development image and assume equivalence.

## Common causes

- test working-directory assumptions;
- missing runtime shared libraries;
- generated files written outside the build tree;
- UID/GID ownership mismatch on bind mounts;
- architecture-specific alignment or endian assumptions;
- parallel tests sharing ports, filenames, or registry state;
- stale CMake caches copied between incompatible presets;
- coverage tools mismatched with compiler-produced data.
