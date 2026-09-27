# Task 21: Add Script to Build Docstrings and Integrate to CI

## Objective
Add a dedicated script (`scripts/build_docstring.sh`) to build Doxygen HTML API documentation from source docstrings into `docs/doxygen/_build/html`, establish Doxygen configuration and Makefile under `docs/doxygen/`, and integrate docstring generation into the CI documentation workflow (`.github/workflows/integration.yml`).

## Scope
- `scripts/build_docstring.sh`
- `docs/doxygen/Doxyfile`
- `docs/doxygen/Makefile`
- `.github/workflows/integration.yml`
- `GEMINI.md`
- `CLAUDE.md`
- `.ai/tasks/21_add_script_to_build_docstrings.md`

## Important Decisions & Assumptions
- **Directory Layout**: Placed Doxygen configuration and build files under `docs/doxygen/`, mirroring Sphinx documentation folders (`docs/arch/` and `docs/reqs/`).
- **Output Directory**: Configured Doxygen output to `_build/` inside `docs/doxygen/` (`docs/doxygen/_build/html`), matching the `.gitignore` pattern `_build` so generated artifacts are never tracked.
- **Exclusion Rules**: Configured Doxygen to scan source code under `connix/` while excluding unit/integration test suites (`*/tst/*`), `.git`, and build folders.
- **CI Workflow Integration**: Integrated `./scripts/build_docstring.sh --clean` into the `documentation` job of `.github/workflows/integration.yml`.
- **Script Conventions**: Built `scripts/build_docstring.sh` following repository standards (`env.sh`, `dump_env`, `dump_command`, `-c/--clean`, `--clean-only`, `-d/--dry-run`, `-h/--help`, line lengths $\le 80$ characters).

## Changes Made
- Created `docs/doxygen/Doxyfile` configuring Doxygen 1.9.8 for C++17 source code documentation.
- Created `docs/doxygen/Makefile` with `html` and `clean` targets.
- Created `scripts/build_docstring.sh` with executable permissions, supporting `-c/--clean` and `--clean-only`.
- Integrated `./scripts/build_docstring.sh --clean` into `.github/workflows/integration.yml` under `documentation`.
- Documented `./scripts/build_docstring.sh` in `GEMINI.md` and `CLAUDE.md`.
- Created task tracking log `.ai/tasks/21_add_script_to_build_docstrings.md`.

## Validation Performed
- Ran `./scripts/build_docstring.sh --help` to verify CLI options.
- Ran `./scripts/build_docstring.sh --dry-run` to verify environment and command dumping.
- Ran `./scripts/build_docstring.sh --clean` and confirmed successful HTML generation in `docs/doxygen/_build/html`.
- Tested `./scripts/build_docstring.sh --clean-only` to verify direct cleanup without rebuilding.
- Tested `make clean && make html` inside `docs/doxygen/` (passed).
- Validated YAML syntax for `.github/workflows/integration.yml` (valid).
- Ran `./scripts/check_spelling.sh` across all files and verified documentation and script files (0 errors).
- Ran `./scripts/run_clang_format.sh` to ensure formatting compliance (clean).
