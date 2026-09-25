# Task 03: Fix clang-tidy Script Error Handling and Suppress Noise

## Objective
Fix `scripts/run_clang_tidy.sh` so that it exits with a non-zero exit code upon encountering any warnings or errors. Hide suppressed warnings and header analysis noise to only display actual actionable warnings, avoiding confusion. Resolve the active `performance-avoid-endl` warnings in the CLI and GUI bootstrap entrypoints.

## Scope
- `scripts/run_clang_tidy.sh`
- `connix/connix-cli/src/Bootstrap/Connix_CLI_Main.cpp`
- `connix/connix-gui/src/Bootstrap/Connix_GUI_Main.cpp`

## Important Decisions & Assumptions
- Added `-quiet` and `-warnings-as-errors=*` flags to `run-clang-tidy` invocation.
- Filtered stdout/stderr through `grep -v -E "(^[0-9]+ (warning|error)s? generated\.|^clang-tidy|Suppressed [0-9]+ warnings?|Use -header-filter=)"` so only actual code diagnostics and relevant file locations are output.
- Captured `STATUS_CODE=${PIPESTATUS[0]}` before executing `popd` to prevent `popd` from overwriting the return status with `0`.
- Replaced `std::endl` with `'\n'` in `Connix_CLI_Main.cpp` and `Connix_GUI_Main.cpp` to resolve `performance-avoid-endl` diagnostics cleanly.

## Changes Made
- `scripts/run_clang_tidy.sh`:
  - Fixed typo `usage` -> `print_usage`.
  - Added `-quiet` and `-warnings-as-errors=*` to argument list.
  - Filtered output to strip `clang-tidy` banners and suppressed warning summaries.
  - Captured exit code from `${PIPESTATUS[0]}` and exited with that status after `popd`.
- `connix/connix-cli/src/Bootstrap/Connix_CLI_Main.cpp`:
  - Replaced `std::endl` with `'\n'`.
- `connix/connix-gui/src/Bootstrap/Connix_GUI_Main.cpp`:
  - Replaced `std::endl` with `'\n'`.

## Validation Performed
- Verified failure behavior: Artificially introduced a `std::endl` diagnostic and confirmed `scripts/run_clang_tidy.sh` printed only the diagnostic and exited with `1`.
- Verified success behavior: Restored clean code, executed `./scripts/run_clang_tidy.sh` and confirmed 0 noise output and exit code `0`.
- `./scripts/run_clang_format.sh`: 0 formatting violations.
- `./scripts/build_application_release.sh`: Succeeded and updated deploy artifacts.
- `./scripts/run_unit_tests.sh`: All 4 test suites passed with 100% coverage.
