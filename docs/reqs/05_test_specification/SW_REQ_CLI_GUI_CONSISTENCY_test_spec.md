# Test Specification: SW_REQ_CLI_GUI_CONSISTENCY

**Source:** `docs/reqs/04_nonfunctional_requirements/usability.rst`

## Requirement Summary

This test specification covers the parity children under the parent
`SW_REQ_CLI_GUI_CONSISTENCY`: `SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS` and
`SW_REQ_CLI_GUI_CONSISTENCY_RESULTS`.

**Fit Criterion:** Each child behavior is tested independently: equivalent CLI and GUI invocations
perform the same underlying action and produce the same connection behavior and output content.

## Test Design Techniques

- Equivalence Partitioning: capability classes shared between CLI and GUI (protocol, role,
  execution mode, rule configuration).
- Comparison/differential testing: run identical logical configuration through each interface and
  diff observable behavior/output.

## Test Cases

### TC_PARITY_01 — Protocol selection parity (TCP/UDP/UDS)
*Type: Positive · Priority: High*

Title: Protocol selection parity (TCP/UDP/UDS).
Satisfies:
   - SW_REQ_CLI_GUI_CONSISTENCY
   - SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS
   - SW_REQ_CLI_GUI_CONSISTENCY_RESULTS
Objective: Happy — Verify that protocol selection parity (TCP/UDP/UDS).
Steps:
   - 1. Establish the precondition: GUI built; both interfaces available.
   - 2. Run a TCP exchange via CLI.
   - 3. Run the equivalent TCP exchange via GUI with the same target.
   - 4. Compare connection behavior and output content. Repeat for UDP and UDS.

Expectations:
   - 1. GUI and CLI produce identical connection behavior and output content for each protocol.

TestData:
   - Protocol=TCP, UDP, UDS (3 sub-cases).
   - Fixture setup required by the precondition: GUI built; both interfaces available.

### TC_PARITY_02 — Role selection parity (CLIENT/SERVER/DUAL)
*Type: Positive · Priority: High*

Title: Role selection parity (CLIENT/SERVER/DUAL).
Satisfies:
   - SW_REQ_CLI_GUI_CONSISTENCY
   - SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS
   - SW_REQ_CLI_GUI_CONSISTENCY_RESULTS
Objective: Happy — Verify that role selection parity (CLIENT/SERVER/DUAL).
Steps:
   - 1. Establish the precondition: GUI built.
   - 2. Configure CLIENT role via CLI; observe behavior.
   - 3. Configure identical CLIENT role via GUI; observe behavior. Repeat for SERVER and DUAL.

Expectations:
   - 1. Behavior matches between interfaces for each role.

TestData:
   - Role=CLIENT, SERVER, DUAL (3 sub-cases).
   - Fixture setup required by the precondition: GUI built.

### TC_PARITY_03 — Execution mode parity (ONETIME/PERIODIC)
*Type: Positive · Priority: High*

Title: Execution mode parity (ONETIME/PERIODIC).
Satisfies:
   - SW_REQ_CLI_GUI_CONSISTENCY
   - SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS
   - SW_REQ_CLI_GUI_CONSISTENCY_RESULTS
Objective: Happy — Verify that execution mode parity (ONETIME/PERIODIC).
Steps:
   - 1. Establish the precondition: GUI built.
   - 2. Run ONETIME mode via CLI; observe.
   - 3. Run identical ONETIME config via GUI; observe. Repeat for PERIODIC.

Expectations:
   - 1. Behavior matches between interfaces for each mode.

TestData:
   - Mode=ONETIME, PERIODIC (2 sub-cases).
   - Fixture setup required by the precondition: GUI built.

### TC_PARITY_04 — Rule configuration parity
*Type: Positive · Priority: High*

Title: Rule configuration parity.
Satisfies:
   - SW_REQ_CLI_GUI_CONSISTENCY
   - SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS
   - SW_REQ_CLI_GUI_CONSISTENCY_RESULTS
Objective: Happy — Verify that rule configuration parity.
Steps:
   - 1. Establish the precondition: GUI built; GUI exposes rule configuration.
   - 2. Define an identical rule (event + action) via CLI config and via GUI rule editor.
   - 3. Trigger the event in both.
   - 4. Compare resulting action and output.

Expectations:
   - 1. Rule fires identically and produces the same action/output in both interfaces.

TestData:
   - Same rule definition, both interfaces.
   - Fixture setup required by the precondition: GUI built; GUI exposes rule configuration.

### TC_PARITY_05 — GUI-only or CLI-only capability identified as a gap
*Type: Error · Priority: Medium*

Title: GUI-only or CLI-only capability identified as a gap.
Satisfies:
   - SW_REQ_CLI_GUI_CONSISTENCY
   - SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS
   - SW_REQ_CLI_GUI_CONSISTENCY_RESULTS
Objective: Error — Verify that gUI-only or CLI-only capability identified as a gap.
Steps:
   - 1. Establish the precondition: GUI built.
   - 2. Enumerate capabilities exposed by the GUI and by the CLI independently.
   - 3. Diff the two lists.

Expectations:
   - 1. Any capability present in one interface but not the other is flagged as a parity gap requiring resolution (either add to the missing interface or document as an intentional, requirement-violating exception for PO review).

TestData:
   - full capability enumeration.
   - Fixture setup required by the precondition: GUI built.

## Notes

This requirement cannot be exhaustively tested today because (a) no GUI implementation exists to
test against, and (b) no requirement or design document enumerates the full set of capabilities
the GUI will expose. The PO confirmed (OP-03, see `OPEN_POINTS.md`) that a capability checklist
will be produced as a GUI-specific requirement or design doc once GUI design begins, and this test
spec will be updated against it at that time — no action is needed before then.
