# Test Specification: SW_REQ_FOREGROUND_EXECUTION

**Source:** `docs/reqs/04_nonfunctional_requirements/operational.rst`

## Requirement Summary

This test specification covers the operational children under the parent
`SW_REQ_FOREGROUND_EXECUTION`: `SW_REQ_FOREGROUND_EXECUTION_PROCESS`,
`SW_REQ_NO_DAEMON_SERVICE`, `SW_REQ_NO_DETACH_OPTION`, and
`SW_REQ_NO_PID_FILE_MANAGEMENT`.

**Fit Criterion:** Each child behavior is tested independently: Connix remains in the foreground,
provides no daemon service, provides no detach option, and creates or manages no PID file.

## Test Design Techniques

- Equivalence Partitioning: execution modes {ONETIME, PERIODIC} × expected process behavior
  (foreground only).
- Error Guessing / negative testing: search for and attempt any daemonization-like flag.

## Test Cases

### TC_FG_01 — Connix runs attached to controlling terminal (ONETIME)
*Type: Positive · Priority: High*

Title: Connix runs attached to controlling terminal (ONETIME).
Satisfies:
   - SW_REQ_FOREGROUND_EXECUTION
   - SW_REQ_FOREGROUND_EXECUTION_PROCESS
Objective: Happy — Verify that connix runs attached to controlling terminal (ONETIME).
Steps:
   - 1. Establish the precondition: Terminal session available.
   - 2. Launch Connix in ONETIME mode from a terminal.
   - 3. Observe process/terminal association (e.g. via `ps -o tty`).

Expectations:
   - 1. Process remains attached to the controlling terminal for its lifetime.

TestData:
   - mode=ONETIME.
   - Fixture setup required by the precondition: Terminal session available.

### TC_FG_02 — Connix runs attached to controlling terminal (PERIODIC)
*Type: Positive · Priority: High*

Title: Connix runs attached to controlling terminal (PERIODIC).
Satisfies:
   - SW_REQ_FOREGROUND_EXECUTION
   - SW_REQ_FOREGROUND_EXECUTION_PROCESS
Objective: Happy — Verify that connix runs attached to controlling terminal (PERIODIC).
Steps:
   - 1. Establish the precondition: Terminal session available.
   - 2. Launch Connix in PERIODIC mode from a terminal.
   - 3. Observe process/terminal association while it runs across several triggers.

Expectations:
   - 1. Process remains attached to the controlling terminal throughout, including during idle periods between triggers.

TestData:
   - mode=PERIODIC.
   - Fixture setup required by the precondition: Terminal session available.

### TC_FG_03 — Closing the terminal terminates Connix
*Type: Positive · Priority: Medium*

Title: Closing the terminal terminates Connix.
Satisfies:
   - SW_REQ_FOREGROUND_EXECUTION
   - SW_REQ_FOREGROUND_EXECUTION_PROCESS
   - SW_REQ_NO_DAEMON_SERVICE
   - SW_REQ_NO_DETACH_OPTION
Objective: Happy — Verify that closing the terminal terminates Connix.
Steps:
   - 1. Establish the precondition: Connix running in PERIODIC mode from an interactive terminal (no `nohup`/`disown` applied).
   - 2. Launch Connix in PERIODIC mode.
   - 3. Close the terminal (send SIGHUP).

Expectations:
   - 1. Connix process terminates along with the terminal session (no self-daemonizing behavior keeps it alive).

TestData:
   - mode=PERIODIC.
   - Fixture setup required by the precondition: Connix running in PERIODIC mode from an interactive terminal (no `nohup`/`disown` applied).

### TC_FG_04 — No CLI flag or config option exists for daemonization
*Type: Negative · Priority: High*

Title: No CLI flag or config option exists for daemonization.
Satisfies:
   - SW_REQ_FOREGROUND_EXECUTION
   - SW_REQ_NO_DAEMON_SERVICE
   - SW_REQ_NO_DETACH_OPTION
Objective: Error — Verify that no CLI flag or config option exists for daemonization.
Steps:
   - 1. Establish the precondition: Full CLI help/documentation and config schema available.
   - 2. Inspect `--help` output and the full configuration schema for any daemonize/background/detach option.

Expectations:
   - 1. No such option exists in either the CLI flags or the configuration schema.

TestData:
   - `--help` output, config schema.
   - Fixture setup required by the precondition: Full CLI help/documentation and config schema available.

### TC_FG_05 — No PID file is created or managed
*Type: Negative · Priority: Medium*

Title: No PID file is created or managed.
Satisfies:
   - SW_REQ_FOREGROUND_EXECUTION
   - SW_REQ_NO_PID_FILE_MANAGEMENT
Objective: Error — Verify that no PID file is created or managed.
Steps:
   - 1. Establish the precondition: Connix run in any mode.
   - 2. Run Connix in PERIODIC mode.
   - 3. Search the filesystem (working directory, `/var/run`, `/tmp`) for any PID file created by Connix.

Expectations:
   - 1. No PID file is created or managed by Connix at any point during or after execution.

TestData:
   - mode=PERIODIC.
   - Fixture setup required by the precondition: Connix run in any mode.
