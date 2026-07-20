# Example Activation Prompts

- Create a Dockerfile that builds BinMorph with the GCC debug preset and runs all CTest tests.
- Add an ASan/UBSan Docker test lane and preserve sanitizer logs as CI artifacts.
- Reproduce the generated C++ library integration test in a clean Debian container.
- Create a Compose test topology for `binmorphd` and a black-box CLI client.
- Add Clang coverage generation using Docker and export LCOV plus HTML reports.
- Run the schema parser fuzz target for 60 seconds and retain crash inputs.
- Diagnose why CTest passes locally but fails in the GitHub Actions container.
