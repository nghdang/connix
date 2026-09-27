# Task 10: Add Script to Build Conan Package

## Objective
Add a dedicated script (`scripts/build_conan_package.sh`) to build and package Connix as a Conan package into the local Conan cache, adhering to the project's scripting patterns and CLI conventions.

## Scope
- `scripts/build_conan_package.sh`
- `.github/workflows/integration.yml`
- `GEMINI.md`
- `CLAUDE.md`

## Important Decisions & Assumptions
- Sourced `scripts/env.sh` to leverage standard helpers (`dump_env`, `dump_and_run_command`, tool version checks, and paths).
- Defaulted host and build profiles to `release/gcc13_x86_64` to match `scripts/build_application_release.sh`.
- Implemented `resolve_profile` helper to seamlessly support profile paths in `${PROJECT_DIR}/conan_profiles/`, custom file paths, or cached Conan profiles.
- Integrated `-f, --force` flag to clear matching `connix*` packages from the local Conan cache before rebuilding.
- Supported standard options: `--host-profile`, `--build-profile`, `-f`/`--force`, `-d`/`--dry-run`, `-h`/`--help`, and pass-through of extra options to `conan create`.
- Added executable permissions (`+x`) to the script.

## Changes Made
- Created `scripts/build_conan_package.sh`.
- Added `Build Conan and Install` step into `.github/workflows/integration.yml` under `release` job.
- Updated `GEMINI.md` and `CLAUDE.md` with usage instructions for `build_conan_package.sh`.

## Validation Performed
- Validated help output (`-h`, `--help`).
- Validated dry-run execution (`-d`, `--dry-run`, with and without `-f`).
- Validated full build of Conan package (`./scripts/build_conan_package.sh`).
- Validated force rebuild of Conan package (`./scripts/build_conan_package.sh --force`).
- Validated formatting with `./scripts/run_clang_format.sh`.
