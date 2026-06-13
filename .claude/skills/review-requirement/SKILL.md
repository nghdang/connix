---
name: review-requirement
description: >
  Review a Volere requirements .rst file for the Connix project. Use this skill when the user asks
  to review any .rst file under docs/requirements/, or says "review the requirement for X" where X
  is a section name like purpose, stakeholders, users, constraints, assumptions, performance,
  security, usability, operational, maintainability, naming conventions, or functional requirements.
---

# Review Requirement

Reviews a filled Volere-template `.rst` file in the Connix requirements docs and persists findings
to a per-branch markdown report.

## Volere section map

| Folder                         | File                         | Volere section                              |
|--------------------------------|------------------------------|---------------------------------------------|
| 01_project_drivers             | purpose.rst                  | SS1: Purpose -- situation, goal, background |
| 01_project_drivers             | stakeholders.rst             | SS2: Stakeholders -- interest, win/loss     |
| 01_project_drivers             | users.rst                    | SS3: Users -- who uses it, characteristics  |
| 02_project_constraints         | mandated_constraints.rst     | SS4: Mandated constraints                   |
| 02_project_constraints         | naming_conventions.rst       | SS5: Naming conventions and definitions     |
| 02_project_constraints         | assumptions.rst              | SS6: Assumptions                            |
| 03_functional_requirements     | functional_requirements.rst  | SS10: Functional requirements               |
| 04_nonfunctional_requirements  | look_and_feel.rst            | SS11: Look and feel                         |
| 04_nonfunctional_requirements  | usability.rst                | SS12: Usability                             |
| 04_nonfunctional_requirements  | performance.rst              | SS13: Performance                           |
| 04_nonfunctional_requirements  | operational.rst              | SS14: Operational                           |
| 04_nonfunctional_requirements  | maintainability.rst          | SS15: Maintainability and portability       |
| 04_nonfunctional_requirements  | security.rst                 | SS16: Security                              |

## Report persistence

**Location:** `.claude/requirement-reviews/<sanitized-branch>.md`

Branch name sanitization: replace every `/` with `-`.
Example: `req/add-software-requirements` -> `req-add-software-requirements.md`.

**YAML frontmatter** at the top of every report:
```yaml
---
branch: <git branch name>
commit-range: <base>..<HEAD>
---
```

Compute the commit range:
```bash
BASE=$(git log --oneline main..HEAD | tail -1 | cut -d' ' -f1)
HEAD=$(git rev-parse --short HEAD)
# commit-range: "$BASE..$HEAD"
```

**Decision logic before reviewing:**
1. Report does not exist -- create a fresh report.
2. Report exists AND `commit-range` in frontmatter matches current range -- read
   existing findings before reviewing; append NEW findings only under a new
   `### Review Nth round` section (N = count of existing `### Review` sections + 1).
3. Report exists AND `commit-range` differs -- overwrite the entire report (fresh start).

## Finding format

Each finding is rendered as a heading, a two-column table, and a false-positive checkbox:

```markdown
**Finding N** -- <one-line summary>

| Field      | |
|---|---|
| Location   | `<file path>:<line>` |
| Reason     | <why this is a valid finding -- rule or principle violated> |
| Confidence | <0-100> |
| Suggestion | <concrete fix: rewritten text, replacement, or command> |
| Trade-off  | **Pro:** <benefit of fixing>. **Con:** <cost or downside>. |

- [ ] False positive
```

Severity levels group findings under `####` headings: **Must fix**, **Should fix**,
**Suggestion**. Use `---` as a separator between consecutive findings in the same group.

## Strikethrough on subsequent runs

Before appending a new round, update the existing report:

1. For each finding in prior sections, re-check whether the flagged issue still exists
   in the current file content. If the issue is gone, wrap the entire finding block
   (from `**Finding N**` through the false-positive line) in `~~...~~` strikethrough
   on the summary line only: change `**Finding N** -- summary` to
   `~~**Finding N** -- summary~~` and add a `> Fixed.` blockquote below the table.
2. For each finding whose checkbox reads `- [x] False positive`, apply the same
   strikethrough and add `> Marked false positive.` blockquote.

## Review checks

### Automated checks

Run these commands against the target file. Any output is a finding.

```bash
# 1. Non-ASCII characters
grep -Pn '[^\x00-\x7F]' <file>

# 2. UK English spellings
grep -Pin '\b(organisation|behaviour|colour|analyse|centre|programme)\b' <file>

# 3. Lines over 120 characters
awk 'length>120 {print NR": "length" chars: "$0}' <file>

# 4. Leftover todo directive
grep -n '\.\. todo::' <file>

# 5. Sphinx build errors/warnings
cd docs/requirements && make html 2>&1 | grep -E '(ERROR|WARNING)'
```

### Manual content review

For each paragraph or list item, assess:

- **Grammar** -- subject-verb agreement, tense consistency, punctuation.
- **Clarity** -- would a new contributor understand this without additional context?
- **Conciseness** -- can any sentence be shortened without losing meaning?
- **Cohesion** -- do sentences within a section build a single coherent point?
- **Necessity** -- is this sentence or item load-bearing? If removing it loses no
  information a reader needs, flag it as unnecessary (Should fix).
- **Accuracy** -- do stated facts align with `README.md` and other filled pages?

### Fit criterion / Gherkin rule

Applies to: `03_functional_requirements/` and `04_nonfunctional_requirements/` files.
Skip for: `purpose.rst`, `stakeholders.rst`, `users.rst`, `assumptions.rst`,
`naming_conventions.rst`.

Every fit criterion **must** follow Gherkin format:

```
Given <precondition>
When  <action or event>
Then  <expected observable outcome>
```

Flag any fit criterion written as plain prose as **Must fix**. Provide a Gherkin
rewrite in the Suggestion field.

### Cross-file consistency

Compare names and facts in the reviewed file against the canonical definitions:

- Component names (Must fix on mismatch):
  - CLI tool: `connix`
  - Library: `connix-core`
  - GUI: `connix-gui`
- Protocol family labels (Should fix on mismatch):
  - REST APIs / TCP / UDP / Unix Domain Sockets
- User names (Should fix on mismatch -- must match `users.rst`):
  - "CLI user", "library consumer", "GUI user"

## Workflow

1. Run `git rev-parse --abbrev-ref HEAD` to get the branch name.
2. Compute the commit range (base..HEAD) using the commands above.
3. Derive the report path: `.claude/requirement-reviews/<sanitized-branch>.md`.
4. Apply the decision logic (create / read-same / overwrite).
5. Read the target `.rst` file (path from user or invocation args).
6. Read `docs/requirements/01_project_drivers/purpose.rst`,
   `docs/requirements/01_project_drivers/stakeholders.rst`, and
   `docs/requirements/01_project_drivers/users.rst` as consistency references.
7. Run all automated checks; note any output as findings.
8. Perform the full manual content review.
9. If re-run on same commit range: update strikethroughs in the existing report first.
10. Assign each finding a sequential number within this round and a severity level.
11. Write the report (create or append), then tell the user the report path and a brief
    summary (count per severity level). Do not repeat all findings in the chat --
    the report is the canonical record.
