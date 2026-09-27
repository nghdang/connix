# Task 26: Verify CI Scripts Help and Dry-Run

## Objective
Audit and verify that the `--help` (and `-h`) and `--dry-run` (and `-d`) command-line options of all utility and CI scripts in `scripts/` are fully functional, return standard zero exit codes, and execute properly without runtime errors.

## Scope
- `scripts/build_application_native.sh`
- `scripts/build_application_release.sh`
- `scripts/build_conan_package.sh`
- `scripts/build_docstring.sh`
- `scripts/check_coverage.sh`
- `scripts/check_spelling.sh`
- `scripts/run_clang_format.sh`
- `scripts/run_clang_tidy.sh`
- `scripts/run_integration_tests.sh`
- `scripts/run_unit_tests.sh`
- `.cspell/custom-dictionary.txt`
- `.ai/tasks/26_verify_ci_scripts_help_and_dry_run.md`

## Important Decisions & Assumptions
- **Identification of Root Cause**: During the audit, 4 scripts (`build_application_release.sh`, `check_coverage.sh`, `run_clang_format.sh`, `run_unit_tests.sh`) failed with exit code 127 (`print_usage: command not found`). In all four scripts, the usage function had been defined as `function usage()` while the option parser invoked `print_usage`.
- **Standardized Usage Function Name**: Renamed `function usage()` to `function print_usage()` across all four scripts to align with project conventions and the calling code.
- **Dictionary Update**: Added `DCONAN` to `.cspell/custom-dictionary.txt` under `Build tools & CMake` for the `-DCONAN_*` arguments present in the build scripts.

## Changes Made
- Updated `scripts/build_application_release.sh`: Renamed `usage()` to `print_usage()`.
- Updated `scripts/check_coverage.sh`: Renamed `usage()` to `print_usage()`.
- Updated `scripts/run_clang_format.sh`: Renamed `usage()` to `print_usage()`.
- Updated `scripts/run_unit_tests.sh`: Renamed `usage()` to `print_usage()`.
- Added `DCONAN` to `.cspell/custom-dictionary.txt`.
- Created task tracking log `.ai/tasks/26_verify_ci_scripts_help_and_dry_run.md`.

## Validation Performed
- Ran a systematic test across all 10 scripts testing `--help`, `-h`, `--dry-run`, and `-d`:
  - `build_application_native.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `build_application_release.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `build_conan_package.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `build_docstring.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `check_coverage.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `check_spelling.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `run_clang_format.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `run_clang_tidy.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `run_integration_tests.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
  - `run_unit_tests.sh`: `--help` (0), `-h` (0), `--dry-run` (0), `-d` (0).
- Ran spell checking across `scripts/` and default targets: `./scripts/check_spelling.sh` (213 files checked, 0 errors).
- Ran clang-format check: `./scripts/run_clang_format.sh` (clean).
- Verified git status (all changes unstaged).
