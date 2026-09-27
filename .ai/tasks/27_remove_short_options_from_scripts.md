# Task 27: Remove Short Options from Scripts

## Objective
Remove all short command-line options (`-h`, `-d`, `-f`, `-c`, `-m`, `-s`) across all utility and CI scripts in `scripts/`, standardizing exclusively on explicit long options (`--help`, `--dry-run`, `--force`, `--clean`, `--clean-only`, `--min`, `--fix`, `--show`).

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
- `GEMINI.md`
- `CLAUDE.md`
- `.ai/tasks/27_remove_short_options_from_scripts.md`

## Important Decisions & Assumptions
- Standardized command-line argument parsing across all 10 scripts to recognize only explicit long options.
- Updated the `print_usage` display text in each script to omit short flags and clearly present the supported long options.
- Updated documentation in `GEMINI.md` and `CLAUDE.md` to remove references to deprecated short options (`-f`, `-c`, `-d`).

## Changes Made
- Updated `scripts/build_application_native.sh`: Removed `-f`, `-d`, `-h`.
- Updated `scripts/build_application_release.sh`: Removed `-f`, `-d`, `-h`.
- Updated `scripts/build_conan_package.sh`: Removed `-f`, `-d`, `-h`.
- Updated `scripts/build_docstring.sh`: Removed `-c`, `-d`, `-h`.
- Updated `scripts/check_coverage.sh`: Removed `-m`, `-d`, `-h`.
- Updated `scripts/check_spelling.sh`: Removed `-d`, `-h`.
- Updated `scripts/run_clang_format.sh`: Removed `-f`, `-s`, `-d`, `-h`.
- Updated `scripts/run_clang_tidy.sh`: Removed `-d`, `-h`.
- Updated `scripts/run_integration_tests.sh`: Removed `-d`, `-h`.
- Updated `scripts/run_unit_tests.sh`: Removed `-d`, `-h`.
- Updated `GEMINI.md` and `CLAUDE.md` to reference only long options.
- Created task tracking log `.ai/tasks/27_remove_short_options_from_scripts.md`.

## Validation Performed
- Ran `--help` and `--dry-run` on all 10 scripts, confirming all returned exit code 0.
- Ran tests verifying short options are no longer accepted or are treated as unknown/passthrough.
- Ran `./scripts/check_spelling.sh` across all files (clean, 213 files checked, 0 errors).
- Ran `./scripts/run_clang_format.sh` (clean).
