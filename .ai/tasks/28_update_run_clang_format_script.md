# Task 28: Update run_clang_format.sh Script Options

## Objective
Update `scripts/run_clang_format.sh` to:
1. Replace `--show` with `--check`.
2. Default to check-only mode when no arguments are provided or when `--check` is explicitly specified.
3. Apply in-place formatting fixes when `--fix` is specified.
4. Align CI integration (`.github/workflows/integration.yml`) and documentation (`CLAUDE.md`).

## Scope
- `scripts/run_clang_format.sh`
- `.github/workflows/integration.yml`
- `CLAUDE.md`
- `.ai/tasks/28_update_run_clang_format_script.md`

## Important Decisions & Assumptions
- Replaced `--show` option with `--check` in both usage documentation and the command-line argument parser.
- Retained `DEFAULT_SHOULD_FIX="$NO"` so running `./scripts/run_clang_format.sh` with no arguments performs a non-modifying formatting check (`--dry-run` passed to `clang-format`).
- Running `./scripts/run_clang_format.sh --check` explicitly sets `SHOULD_FIX="$NO"`.
- Running `./scripts/run_clang_format.sh --fix` sets `SHOULD_FIX="$YES"`, applying `-i` in-place formatting.
- Updated `.github/workflows/integration.yml` to run `./scripts/run_clang_format.sh --check` in the CI pipeline.

## Changes Made
- Updated `scripts/run_clang_format.sh`: Replaced `--show` with `--check` in `print_usage` and option parsing.
- Updated `.github/workflows/integration.yml`: Changed `./scripts/run_clang_format.sh --dry-run` to `./scripts/run_clang_format.sh --check`.
- Updated `CLAUDE.md`: Updated code formatting command examples.
- Created task tracking log `.ai/tasks/28_update_run_clang_format_script.md`.

## Validation Performed
- Ran `./scripts/run_clang_format.sh --help` to verify usage output.
- Ran `./scripts/run_clang_format.sh` with no arguments (verified check-only execution, exit code 0).
- Ran `./scripts/run_clang_format.sh --check` (verified check-only execution, exit code 0).
- Ran `./scripts/run_clang_format.sh --fix` (verified in-place formatting, exit code 0).
- Ran `./scripts/run_clang_format.sh --dry-run` (verified command dumping without execution, exit code 0).
- Validated YAML syntax for `.github/workflows/integration.yml` (valid).
- Ran `./scripts/check_spelling.sh` across all targets (clean, 213 files checked, 0 errors).
