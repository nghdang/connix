# Task 36: Add Line Length Check Script

## Objective
Implement an automated line length verification script
(`scripts/check_line_length.sh`) adhering to the workspace line length mandate
(maximum 80 characters per line, ignoring long URLs), integrated with the
established `scripts/` standards (`env.sh`, `--help`, `--dry-run`, long options
only).

## Scope
- `scripts/check_line_length.sh`
- `GEMINI.md`
- `CLAUDE.md`
- `.ai/tasks/36_add_line_length_check_script.md`

## Important Decisions & Assumptions
- **Script Interface & Conventions**: Standardized exclusively on long options
  (`--max-length`, `--dry-run`, `--help`) with no deprecated short options,
  matching Task 27 guidelines.
- **URL Handling**: Automatically excludes lines containing URLs
  (`http://` or `https://`) from violation counts, directly reflecting the
  project mandate: *"Always limit line length to 80 characters. Except too
  long URLs."*
- **Path Resolution**: Supports positional arguments for target files or
  directories. For directories, utilizes `git ls-files` to respect `.gitignore`
  rules. Defaults to `connix` and `docs` when no positional paths are supplied.
- **Binary File Skipping**: Skips binary files using `file -b --mime`
  `charset=binary` detection to prevent false positives on binary artifacts.
- **No `/dev/null`**: Strictly avoided `/dev/null` redirection per engineering
  mandates.

## Changes Made
- Created `scripts/check_line_length.sh` with executable permissions.
- Documented `./scripts/check_line_length.sh` in `GEMINI.md` and `CLAUDE.md`.
- Created task summary `.ai/tasks/36_add_line_length_check_script.md`.

## Validation Performed
- Verified `./scripts/check_line_length.sh --help` displays correct usage text.
- Verified `./scripts/check_line_length.sh --dry-run` performs parameter and
  environment dumping without execution.
- Verified line length of `scripts/check_line_length.sh` itself (0 violations).
- Verified line length of `.ai/tasks/36_add_line_length_check_script.md`
  (0 violations).
- Ran `./scripts/check_spelling.sh scripts/check_line_length.sh` (0 issues).
