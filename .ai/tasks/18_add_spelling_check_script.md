# Task 18: Add Script to Check Spelling with CSpell

## Objective
Add a dedicated script (`scripts/check_spelling.sh`) to perform automated spell checking across the codebase using `cspell`, integrate it with repository environment conventions in `scripts/env.sh`, configure ignore paths in `cspell.json`, and enrich the custom dictionary with project vocabulary. Per user preference, line length checking was skipped.

## Scope
- `scripts/check_spelling.sh`
- `scripts/env.sh`
- `cspell.json`
- `.cspell/custom-dictionary.txt`
- `.github/workflows/integration.yml`
- `GEMINI.md`
- `CLAUDE.md`
- `.ai/tasks/18_add_spelling_check_script.md`

## Important Decisions & Assumptions
- **Script Scope**: Focused exclusively on `cspell` spell checking after user confirmation to skip checking line length.
- **Default Targets**: Defaults to scanning source code and documentation (`${PROJECT_DIR}/connix` and `${PROJECT_DIR}/docs`), while supporting optional custom positional path arguments.
- **Gitignore Integration**: Invokes `cspell` with `--gitignore` and configured `ignorePaths` (`**/_build/**`, `build/**`, `build-*/**`, `.git/**`, `.cache/**`) to avoid scanning generated build artifacts like Sphinx output.
- **Standard CLI Interface**: Followed established script conventions (`env.sh`, `dump_env`, `dump_command`, `-d/--dry-run`, `-h/--help`).
- **Dictionary Updates**: Added 29 legitimate domain terms, PlantUML syntax tokens (`startuml`, `enduml`, `usecase`), Sphinx directives, Linux utilities, and testing acronyms to `.cspell/custom-dictionary.txt` so that full scans of `connix/` and `docs/` pass with zero false positives.

## Changes Made
- Created `scripts/check_spelling.sh` with executable permissions.
- Updated `scripts/env.sh` to locate and version `CSPELL_EXEC` and `CSPELL_VERSION` with PATH fallback for runner environments.
- Updated `cspell.json` with ignore paths for build directories.
- Updated `.cspell/custom-dictionary.txt` with required technical vocabulary.
- Added `Check Spelling` step to the `static-analysis` job in `.github/workflows/integration.yml`.
- Documented `./scripts/check_spelling.sh` in `GEMINI.md` and `CLAUDE.md`.
- Created task tracking log `.ai/tasks/18_add_spelling_check_script.md`.

## Validation Performed
- Ran `./scripts/check_spelling.sh --help` to verify usage output.
- Ran `./scripts/check_spelling.sh --dry-run` to verify parameter and environment dumping.
- Ran `./scripts/check_spelling.sh` across all 173 files in `connix/` and `docs/` (clean, 0 errors).
- Ran `./scripts/check_spelling.sh` with custom sub-paths (`connix/connix-core/infrastructure/transport/dd`, `docs/arch docs/reqs`) (clean, 0 errors).
- Verified line lengths in `scripts/check_spelling.sh` and `scripts/env.sh` (all <= 80 characters).
- Ran `./scripts/run_clang_format.sh` to ensure no C++ formatting regressions (clean).
