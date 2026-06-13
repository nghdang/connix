---
name: write-requirement
description: >
  Write or fill in a Volere requirements page for the Connix project. Use this skill when the user asks to
  fill in or write any .rst file under docs/requirements/, or says "write the requirement for X" where X is
  a section name like users, constraints, assumptions, performance, security, usability, operational,
  maintainability, naming conventions, functional requirements, or look and feel.
---

# Write Requirement

Fills in a Volere-template .rst stub file for the Connix requirements docs.

## Volere section map

Use the file path to find which Volere section to write.

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

## Rules

Follow these rules exactly when writing any .rst file.

**US English only.** Do not use UK spellings. Forbidden words:
`organisation`, `behaviour`, `colour`, `analyse`, `centre`, `licence` (noun), `programme`.
Use: `organization`, `behavior`, `color`, `analyze`, `center`, `license`, `program`.

**ASCII only.** Do not use any non-ASCII characters. Common mistakes to avoid:
- Em-dash (`--`) not `--` (U+2014)
- Straight quote (`"`) not `"` or `"` (U+201C/201D)
- Straight apostrophe (`'`) not `'` or `'` (U+2018/2019)
- Three dots (`...`) not `...` (U+2026)

**Line length.** Wrap all lines at 100 characters or fewer.

**Sentence style.** Use simple sentences. One idea per sentence. Avoid compound sentences joined
by "and", "but", or semicolons when the clauses can stand alone.

**Tables.** Use `.. list-table::` for any tabular content.

**Cleanup.** Remove the `.. todo::` line from the stub before saving.

## Workflow

1. Read the target .rst file (get the path from the user or the invocation args).
2. Read `README.md` for project facts.
3. Read `docs/requirements/01_project_drivers/purpose.rst` and
   `docs/requirements/01_project_drivers/stakeholders.rst` as style references.
4. Look up the file in the section map above to understand what content belongs here.
5. Write the file content. Use plain prose and `.. list-table::` where a table helps.
   Base all facts on the README and existing filled pages -- do not invent facts.
6. Save the file.
7. Run every item in the checklist below. If a check fails, fix the file and re-run it.

## Checklist

Run these commands after writing the file. All must return no output (no errors).

```bash
# 1. No non-ASCII characters
grep -Pn '[^\x00-\x7F]' <file>

# 2. No UK English spellings
grep -Pin '\b(organisation|behaviour|colour|analyse|centre|programme)\b' <file>

# 3. No lines over 100 characters
awk 'length>100 {print NR": "length" chars: "$0}' <file>

# 4. No leftover todo directive
grep -n '\.\. todo::' <file>

# 5. Build succeeds
cd docs/requirements && make html
```

Report to the user which checks passed. If `make html` reports errors, fix the RST
and run `make html` again before reporting success.
