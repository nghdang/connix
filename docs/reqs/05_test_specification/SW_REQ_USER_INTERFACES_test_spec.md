# Test Specification: SW_REQ_USER_INTERFACES

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the child requirements
`SW_REQ_USER_INTERFACES_CLI` and `SW_REQ_USER_INTERFACES_GUI` under the parent
`SW_REQ_USER_INTERFACES`. The parent requires user-interface provision; GUI/CLI parity is specified
separately by `SW_REQ_CLI_GUI_CONSISTENCY`.

**Fit Criterion:** A CLI binary is always invocable from a terminal. A build with the GUI CMake
option enabled produces a GUI binary; a build with the option disabled remains compliant without a
GUI binary.

## Test Design Techniques

- Equivalence Partitioning: {GUI option ON, GUI option OFF} as two build-configuration classes.
- Decision Table: build-option × expected artifact presence.

## Test Cases

### TC_UI_01 — CLI binary always produced
*Type: Positive · Priority: High*

Title: CLI binary always produced.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_CLI
Objective: Happy — Verify that cLI binary always produced.
Steps:
   - 1. Establish the precondition: Clean checkout, default CMake config.
   - 2. Configure and build the project with default options.
   - 3. Locate the CLI executable in the build output.

Expectations:
   - 1. `connix-cli` binary exists and is executable from a terminal with no arguments causing a crash.

TestData:
   - Default CMake invocation (GUI option unset).
   - Fixture setup required by the precondition: Clean checkout, default CMake config.

### TC_UI_02 — CLI runs without GUI dependencies
*Type: Positive · Priority: High*

Title: CLI runs without GUI dependencies.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_CLI
Objective: Happy — Verify that cLI runs without GUI dependencies.
Steps:
   - 1. Establish the precondition: Build with GUI option OFF.
   - 2. Build with GUI CMake option explicitly disabled.
   - 3. Run the CLI binary.

Expectations:
   - 1. CLI runs correctly; no Qt/QML runtime dependency is required.

TestData:
   - `-DBUILD_GUI=OFF` (or project's equivalent flag).
   - Fixture setup required by the precondition: Build with GUI option OFF.

### TC_UI_03 — GUI binary built when option enabled
*Type: Positive · Priority: High*

Title: GUI binary built when option enabled.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_GUI
Objective: Happy — Verify that gUI binary built when option enabled.
Steps:
   - 1. Establish the precondition: Qt/QML toolchain available.
   - 2. Configure CMake with the GUI option enabled.
   - 3. Build the project.
   - 4. Locate the GUI executable.

Expectations:
   - 1. `connix-gui` binary is produced in build output.

TestData:
   - `-DBUILD_GUI=ON`.
   - Fixture setup required by the precondition: Qt/QML toolchain available.

### TC_UI_04 — GUI binary omitted when option disabled
*Type: Positive · Priority: High*

Title: GUI binary omitted when option disabled.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_GUI
Objective: Happy — Verify that gUI binary omitted when option disabled.
Steps:
   - 1. Establish the precondition: Default/disabled GUI option.
   - 2. Configure CMake with the GUI option explicitly disabled or left at default.
   - 3. Build the project.
   - 4. Search build output for a GUI binary.

Expectations:
   - 1. No `connix-gui` binary (or equivalent artifact) is present in the build output; build succeeds.

TestData:
   - `-DBUILD_GUI=OFF`.
   - Fixture setup required by the precondition: Default/disabled GUI option.

### TC_UI_05 — GUI launches without command-line arguments
*Type: Positive · Priority: Medium*

Title: GUI launches without command-line arguments.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_GUI
Objective: Happy — Verify that gUI launches without command-line arguments.
Steps:
   - 1. Establish the precondition: GUI built (TC_UI_03).
   - 2. Launch `connix-gui` with no arguments.

Expectations:
   - 1. GUI process starts and presents its interface without requiring CLI flags.

TestData:
   - none.
   - Fixture setup required by the precondition: GUI built (TC_UI_03).

### TC_UI_06 — GUI exposes every CLI capability
*Type: Positive · Priority: High*

Title: GUI exposes every CLI capability.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_GUI
Objective: Happy — Verify that gUI exposes every CLI capability.
Steps:
   - 1. Establish the precondition: GUI built.
   - 2. Enumerate all CLI capabilities (protocols, roles, execution modes, rule configuration, output redirection).
   - 3. For each, confirm an equivalent control/flow exists in the GUI.

Expectations:
   - 1. Every CLI-exposed capability has a discoverable GUI equivalent (parity checked in detail in `SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md`).

TestData:
   - Capability checklist derived from all other `SW_REQ_*` functional requirements.
   - Fixture setup required by the precondition: GUI built.

### TC_UI_07 — Headless/CI build omitting GUI still compliant
*Type: Boundary · Priority: Medium*

Title: Headless/CI build omitting GUI still compliant.
Satisfies:
   - SW_REQ_USER_INTERFACES
   - SW_REQ_USER_INTERFACES_GUI
Objective: Edge — Verify that headless/CI build omitting GUI still compliant.
Steps:
   - 1. Establish the precondition: CI-style build environment without a display server or Qt installed.
   - 2. Run a build with the GUI option disabled in a headless environment (no X server, Qt not installed).

Expectations:
   - 1. Build succeeds; CLI binary is produced; no build failure due to missing GUI toolchain.

TestData:
   - `-DBUILD_GUI=OFF`, no Qt/QML present.
   - Fixture setup required by the precondition: CI-style build environment without a display server or Qt installed.

## Notes

TC_UI_06 is a coverage placeholder — full parity verification against every current and future
capability is the responsibility of `SW_REQ_CLI_GUI_CONSISTENCY`; see that spec and
`OPEN_POINTS.md` for the blocking issue (no GUI or capability list exists yet).
