# Test Specification: SW_REQ_WORKING_ROLES

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the role children under the parent `SW_REQ_WORKING_ROLES`:
`SW_REQ_ROLE_CLIENT`, `SW_REQ_ROLE_SERVER`, and `SW_REQ_ROLE_DUAL`.

**Fit Criterion:** Each role child is tested independently: selecting CLIENT, SERVER, or DUAL through
configuration or a command-line option starts the corresponding behavior without a separate build.

## Test Design Techniques

- Equivalence Partitioning: role classes {CLIENT, SERVER, DUAL, invalid role}.
- Decision Table: role × selection mechanism (CLI flag vs. config file).

## Test Cases

### TC_ROLE_01 — Run as CLIENT via CLI flag
*Type: Positive · Priority: High*

Title: Run as CLIENT via CLI flag.
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_CLIENT
Objective: Happy — Verify that the CLI role flag starts a CLIENT connection cycle.
Steps:
   - 1. Build one Connix binary and start an in-process TCP echo SERVER on `127.0.0.1:<ephemeral-port>`; wait for its listening readiness.
   - 2. Launch the same binary with `--role client --protocol tcp --host 127.0.0.1 --port <port> --mode onetime --payload PING`.
   - 3. Capture the server bytes, Connix event trace, and process exit status.

Expectations:
   - 1. The binary and echo SERVER start successfully without a role-specific rebuild.
   - 2. Connix starts as CLIENT, connects once, sends `PING`, receives the echoed `PING`, and closes the socket.
   - 3. The trace contains one CLIENT cycle and the process exits successfully.

TestData:
   - One binary; role `CLIENT`; protocol `TCP`; mode `ONETIME`; host `127.0.0.1`; ephemeral port; payload `PING`.
   - Capture ordered lifecycle events, received bytes, exit status; close the peer and release the port in teardown.

### TC_ROLE_02 — Run as SERVER via CLI flag
*Type: Positive · Priority: High*

Title: Run as SERVER via CLI flag.
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_SERVER
Objective: Happy — Verify that the CLI role flag starts a SERVER connection cycle.
Steps:
   - 1. Build one Connix binary and allocate an unused TCP port on `127.0.0.1`.
   - 2. Launch the binary with `--role server --protocol tcp --host 127.0.0.1 --port <port> --mode onetime --payload PONG` and wait for its listening-readiness signal.
   - 3. Connect an in-process TCP CLIENT to `<port>`, send exactly `PING`, and capture the response, SERVER trace, and exit status.

Expectations:
   - 1. The single binary starts successfully in SERVER mode and binds the allocated port.
   - 2. The SERVER accepts exactly one connection and receives exactly `PING`.
   - 3. The SERVER sends `PONG`, closes the connection after the cycle, and reports the expected successful result.

TestData:
   - One binary; role `SERVER`; protocol `TCP`; mode `ONETIME`; host `127.0.0.1`; ephemeral port.
   - Client payload `PING`; expected server response `PONG`; capture event order and exit status; release the port in teardown.

### TC_ROLE_03 — Run as DUAL via CLI flag
*Type: Positive · Priority: High*

Title: Run as DUAL via CLI flag.
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_DUAL
Objective: Happy — Verify that the CLI role flag starts independent CLIENT and SERVER legs.
Steps:
   - 1. Build one Connix binary, allocate two localhost TCP ports, and start a peer SERVER for the CLIENT leg plus a peer CLIENT for the SERVER leg.
   - 2. Launch the binary with `--role dual --mode onetime`, configuring the client leg for the first port and the server leg for the second port.
   - 3. Trigger both legs with `CLIENT-PING` and `SERVER-PING`, and capture each leg's events, socket descriptor, payloads, and close time.

Expectations:
   - 1. Both peers and the DUAL process become ready on their assigned ports.
   - 2. The DUAL process creates one CLIENT leg and one SERVER leg, each using a distinct socket.
   - 3. Both legs complete independently, exchange the expected payloads, and close before the ONETIME process exits.

TestData:
   - One binary; role `DUAL`; mode `ONETIME`; TCP localhost; two ephemeral ports.
   - Payloads `CLIENT-PING` and `SERVER-PING`; peer responses `CLIENT-OK` and `SERVER-OK`.
   - Per-leg event/socket collectors; close all peers and verify both ports are released.

### TC_ROLE_04 — Role selectable via configuration file
*Type: Positive · Priority: High*

Title: Role selectable via configuration file.
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_CLIENT
   - SW_REQ_ROLE_SERVER
   - SW_REQ_ROLE_DUAL
Objective: Happy — Verify that the configuration file selects the requested role.
Steps:
   - 1. Create `$TEST_TMPDIR/server.json` with role `SERVER`, protocol `TCP`, host `127.0.0.1`, an ephemeral port, mode `ONETIME`, and response payload `PONG`.
   - 2. Launch Connix with `--config $TEST_TMPDIR/server.json` and wait for its listening-readiness signal.
   - 3. Connect an in-process CLIENT to the allocated port, send `PING`, and capture the effective role, event trace, response bytes, and exit status.

Expectations:
   - 1. The configuration file is valid and the process reports SERVER readiness on the configured port.
   - 2. The effective role is SERVER; Connix accepts the connection rather than initiating an outbound connection.
   - 3. Connix receives `PING`, sends `PONG`, closes the cycle, and reports success.

TestData:
   - JSON path `$TEST_TMPDIR/server.json`; role `SERVER`; protocol `TCP`; host `127.0.0.1`; ephemeral port; mode `ONETIME`.
   - Client payload `PING`; server response `PONG`; capture effective config/events/output/exit status; remove file and release port.

### TC_ROLE_05 — No separate build required per role
*Type: Positive · Priority: High*

Title: No separate build required per role.
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_CLIENT
   - SW_REQ_ROLE_SERVER
   - SW_REQ_ROLE_DUAL
Objective: Happy — Verify one binary supports CLIENT, SERVER, and DUAL role selection.
Steps:
   - 1. Build one Connix binary once and record its SHA-256 digest.
   - 2. Run that exact binary as CLIENT against a localhost echo peer, then as SERVER with an in-process CLIENT peer, then as DUAL with two localhost peer legs; record the digest, effective role, and result for each run.
   - 3. Compare the three run results and verify no build or binary replacement occurred between runs.

Expectations:
   - 1. The binary builds successfully and its digest is recorded.
   - 2. CLIENT, SERVER, and DUAL each start from the same digest and complete their role-specific localhost exchanges.
   - 3. All three roles succeed without recompilation, and the digest is identical before and after every run.

TestData:
   - One binary path and recorded SHA-256 digest; roles `CLIENT`, `SERVER`, `DUAL`.
   - Local TCP ephemeral-port peers and `PING`/`PONG` payloads; capture role/result records and teardown all peers.

### TC_ROLE_06 — Conflicting role sources (CLI flag vs config file)
*Type: Positive · Priority: Medium*

Title: Conflicting role sources (CLI flag vs config file).
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_CLIENT
   - SW_REQ_ROLE_SERVER
Objective: Happy — Verify that the CLI role overrides a conflicting configuration-file role.
Steps:
   - 1. Create `$TEST_TMPDIR/role-conflict.json` with role `CLIENT`, protocol `TCP`, mode `ONETIME`, host `127.0.0.1`, and an allocated server port.
   - 2. Launch Connix with `--config $TEST_TMPDIR/role-conflict.json --role server`, wait for SERVER readiness, and capture the effective-role record.
   - 3. Connect an in-process CLIENT to the allocated port, send `PING`, and capture the SERVER lifecycle, response bytes, and exit status.

Expectations:
   - 1. The file parses successfully and records `CLIENT` as its file value.
   - 2. The effective role is `SERVER`; Connix listens and does not initiate an outbound CLIENT connection.
   - 3. Connix accepts `PING`, completes the SERVER cycle, and reports success, proving the CLI value took precedence.

TestData:
   - Config path `$TEST_TMPDIR/role-conflict.json`; file role `CLIENT`; CLI role `SERVER`.
   - Protocol `TCP`; host `127.0.0.1`; ephemeral port; mode `ONETIME`; payload/response `PING`/`PONG`.
   - Capture effective configuration, event trace, bytes, and exit status; remove file and release port.

### TC_ROLE_07 — Invalid role value rejected
*Type: Error · Priority: High*

Title: Invalid role value rejected.
Satisfies:
   - SW_REQ_WORKING_ROLES
   - SW_REQ_ROLE_CLIENT
   - SW_REQ_ROLE_SERVER
   - SW_REQ_ROLE_DUAL
Objective: Error — Verify an unsupported role is rejected before transport startup.
Steps:
   - 1. Create a minimal valid configuration targeting `127.0.0.1:1` and set the CLI argument `--role bogus`; do not start a peer on the target address.
   - 2. Launch Connix with the invalid role and capture validation output, exit status, transport-factory calls, and socket calls.

Expectations:
   - 1. The remaining configuration fields are valid, so the role value is the only validation error.
   - 2. Connix reports an error containing `bogus`, exits non-zero, invokes neither the transport factory nor socket layer, and performs no network activity.

TestData:
   - CLI `--role bogus`; protocol `TCP`; host `127.0.0.1`; port `1`; mode `ONETIME`; payload `PING`.
   - Validation/output capture, transport-factory mock, socket mock, and exit-status assertion; no peer fixture.
