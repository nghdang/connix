# Test Specification: SW_REQ_SIMPLE_CONNECTION_FLOW

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the child requirements under the parent
`SW_REQ_SIMPLE_CONNECTION_FLOW`: CLIENT children
`SW_REQ_CONNECTION_FLOW_CLIENT_LIFECYCLE`,
`SW_REQ_CONNECTION_FLOW_CLIENT_CONNECT_TIMEOUT`,
`SW_REQ_CONNECTION_FLOW_CLIENT_SEND_TIMEOUT`,
`SW_REQ_CONNECTION_FLOW_CLIENT_RECEIVE_TIMEOUT`,
`SW_REQ_CONNECTION_FLOW_CLIENT_TIMEOUT_RESULT`; SERVER children
`SW_REQ_CONNECTION_FLOW_SERVER_LIFECYCLE`,
`SW_REQ_CONNECTION_FLOW_SERVER_ACCEPT_TIMEOUT`,
`SW_REQ_CONNECTION_FLOW_SERVER_RECEIVE_TIMEOUT`,
`SW_REQ_CONNECTION_FLOW_SERVER_SEND_TIMEOUT`,
`SW_REQ_CONNECTION_FLOW_SERVER_TIMEOUT_RESULT`; DUAL children
`SW_REQ_CONNECTION_FLOW_DUAL_LEGS`, `SW_REQ_CONNECTION_FLOW_DUAL_CONCURRENCY`,
`SW_REQ_CONNECTION_FLOW_DUAL_SOCKET_ISOLATION`,
`SW_REQ_CONNECTION_FLOW_DUAL_FAILURE_ISOLATION`,
`SW_REQ_CONNECTION_FLOW_DUAL_INDEPENDENT_CLOSURE`; shared timeout children
`SW_REQ_CONNECTION_FLOW_TIMEOUT_CONNECT_ACCEPT`,
`SW_REQ_CONNECTION_FLOW_TIMEOUT_RECEIVE`, `SW_REQ_CONNECTION_FLOW_TIMEOUT_SEND`,
`SW_REQ_CONNECTION_FLOW_TIMEOUT_SOCKET_CLOSURE`,
`SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_REPORT`,
`SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_ONETIME`,
`SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_PERIODIC`; per-trigger children
`SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_START`,
`SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_CLOSURE`,
`SW_REQ_CONNECTION_FLOW_PER_TRIGGER_PERIODIC_NO_OVERLAP`; and single-send children
`SW_REQ_CONNECTION_FLOW_SINGLE_SEND_REJECTION`,
`SW_REQ_CONNECTION_FLOW_SINGLE_SEND_ERROR_DETAIL`.

**Fit Criterion:** Each child behavior is tested independently: CLIENT and SERVER lifecycle and
operation bounds, timeout reporting and mode outcomes, DUAL leg presence/concurrency/socket
isolation/failure isolation/closure, per-trigger lifecycle and overlap behavior, and single-send
configuration rejection with diagnostic detail.

## Test Design Techniques

- State Transition Testing: connection states {idle, connect/accept, send, receive, close} per
  role.
- Decision Table: role × execution mode (ONETIME/PERIODIC) × trigger count.

## Test Cases

### TC_FLOW_01 — CLIENT cycle: connect-send-receive-close
*Type: Positive · Priority: High*

Title: CLIENT cycle: connect-send-receive-close.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_CLIENT_LIFECYCLE
Objective: Happy — Verify the complete CLIENT lifecycle with one deterministic exchange.
Steps:
   - 1. Start an in-process TCP echo server on `127.0.0.1` using an ephemeral port; wait until `listen()` succeeds and configure it to return received bytes unchanged.
   - 2. Start one Connix CLIENT in ONETIME mode with protocol TCP, host `127.0.0.1`, the allocated port, and payload `PING`.
   - 3. Capture the Connix lifecycle events and server bytes, then wait for the CLIENT process/cycle to finish.

Expectations:
   - 1. The echo server starts and reports a valid ephemeral port.
   - 2. Connix connects once, sends exactly `PING`, receives exactly `PING`, and reports success.
   - 3. The captured event order is `connect, send, receive, close`; the CLIENT socket is closed and the ONETIME process exits.

TestData:
   - role=CLIENT, mode=ONETIME.
   - Fixture setup required by the precondition: Reachable server echoing input.

### TC_FLOW_02 — SERVER cycle: accept-receive-send-close
*Type: Positive · Priority: High*

Title: SERVER cycle: accept-receive-send-close.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_SERVER_LIFECYCLE
Objective: Happy — Verify the complete SERVER lifecycle with one deterministic exchange.
Steps:
   - 1. Allocate an ephemeral TCP port and start one Connix SERVER in ONETIME mode on `127.0.0.1:<port>` with response payload `PONG`.
   - 2. Wait until the SERVER reports readiness, then connect an in-process TCP client and send exactly `PING`.
   - 3. Capture the SERVER lifecycle events, client response bytes, and socket state until the cycle finishes.

Expectations:
   - 1. The SERVER starts listening successfully on the allocated port.
   - 2. The SERVER accepts exactly one connection and receives exactly `PING`.
   - 3. The SERVER sends exactly `PONG`, records `accept, receive, send, close` in order, closes the connection, and finishes the ONETIME cycle.

TestData:
   - role=SERVER, mode=ONETIME.
   - Fixture setup required by the precondition: A client will connect and send once.

### TC_FLOW_03 — No connection left open after CLIENT cycle
*Type: Positive · Priority: High*

Title: No connection left open after CLIENT cycle.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_CLIENT_LIFECYCLE
   - SW_REQ_CONNECTION_FLOW_TIMEOUT_SOCKET_CLOSURE
Objective: Happy — Verify that all CLIENT sockets are closed after cycle completion.
Steps:
   - 1. Start the deterministic TCP echo fixture from TC_FLOW_01 and record the Connix process ID and allocated port.
   - 2. Run one CLIENT ONETIME cycle with payload `PING` and wait for its process exit.
   - 3. Query `/proc/<pid>/fd` while the process is alive and `ss -tanp` immediately after exit; verify the fixture port is released.

Expectations:
   - 1. The echo fixture and CLIENT start successfully with the expected PID/port.
   - 2. The cycle completes successfully and the process exits without a timeout.
   - 3. No Connix-owned `ESTABLISHED`, `LISTEN`, or socket file descriptor remains, and the ephemeral port can be rebound.

TestData:
   - —.
   - Fixture setup required by the precondition: As TC_FLOW_01.

### TC_FLOW_04 — DUAL role runs independent concurrent cycles
*Type: Positive · Priority: High*

Title: DUAL role runs independent concurrent cycles.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_DUAL_LEGS
   - SW_REQ_CONNECTION_FLOW_DUAL_CONCURRENCY
   - SW_REQ_CONNECTION_FLOW_DUAL_SOCKET_ISOLATION
   - SW_REQ_CONNECTION_FLOW_DUAL_INDEPENDENT_CLOSURE
Objective: Happy — Verify that DUAL starts two independent cycles concurrently on separate sockets.
Steps:
   - 1. Start a peer TCP SERVER on `127.0.0.1:<client-leg-port>` that replies `CLIENT-OK`, and a peer TCP CLIENT prepared to connect to Connix on `127.0.0.1:<server-leg-port>` and send `SERVER-PING`.
   - 2. Start one Connix DUAL process in ONETIME mode with the client leg targeting `<client-leg-port>` and the server leg listening on `<server-leg-port>`.
   - 3. Record both leg event traces, socket file descriptors, start timestamps, and close timestamps while triggering both legs.

Expectations:
   - 1. Both peer fixtures and the DUAL process become ready on their allocated ports.
   - 2. One CLIENT leg connects/sends/receives/closes and one SERVER leg accepts/receives/sends/closes; each leg has a distinct socket descriptor.
   - 3. The two active intervals overlap, neither leg waits for the other to close, and both sockets are closed independently before the ONETIME process exits.

TestData:
   - DUAL/ONETIME; TCP localhost; two dynamically allocated ports.
   - CLIENT leg payload `DUAL-CLIENT-PING`; server-leg peer payload `SERVER-PING`; responses `CLIENT-OK` and `SERVER-OK`.
   - Capture: per-leg event vectors, FD numbers, monotonic timestamps; cleanup closes all peers.

### TC_FLOW_05 — DUAL role: one leg's failure does not block the other
*Type: Boundary · Priority: High*

Title: DUAL role: one leg's failure does not block the other.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_DUAL_FAILURE_ISOLATION
Objective: Edge — Verify that failure of one DUAL leg does not prevent the other leg from completing.
Steps:
   - 1. Allocate two localhost TCP ports; start a healthy peer SERVER on the client-leg port and deliberately leave the server-leg peer port unbound.
   - 2. Start Connix in DUAL/ONETIME mode with the client leg targeting the healthy peer and the server leg configured for the unbound peer condition; capture both leg traces and timestamps.
   - 3. Wait up to `2s` for both legs to report completion and inspect each leg's result.

Expectations:
   - 1. The healthy peer is ready and the failure-leg port is confirmed unbound before Connix starts.
   - 2. The failing leg reports its connection error without blocking the healthy leg; both outcomes are recorded separately.
   - 3. The healthy leg completes its send/receive/close cycle successfully, while the failing leg closes its resources and reports an error before the DUAL process exits.

TestData:
   - Role `DUAL`; mode `ONETIME`; two ephemeral TCP ports; one listening peer and one deliberately unbound target.
   - Healthy-leg payload `PING`, response `PONG`; per-leg event/result collectors and monotonic timestamps; cleanup closes peer and releases both ports.

### TC_FLOW_06 — ONETIME mode executes the cycle exactly once
*Type: Positive · Priority: High*

Title: ONETIME mode executes the cycle exactly once.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_ONETIME
Objective: Happy — Verify that ONETIME performs one cycle and then terminates.
Steps:
   - 1. Start an in-process TCP echo server on `127.0.0.1:<ephemeral-port>` and configure one Connix CLIENT with mode `ONETIME` and payload `PING`.
   - 2. Start Connix, collect cycle-start/close events and server receive count, and wait up to `2s` for process termination.
   - 3. After termination, inspect the event count and verify the peer received no additional connection.

Expectations:
   - 1. The peer and CLIENT start successfully with mode `ONETIME`.
   - 2. Exactly one complete cycle is recorded and the process exits within `2s`.
   - 3. The peer observes exactly one connection and one `PING`; no second cycle starts.

TestData:
   - mode=ONETIME.
   - Fixture setup required by the precondition: Reachable peer.

### TC_FLOW_07 — PERIODIC mode repeats the cycle per trigger, no persistent connection
*Type: Positive · Priority: High*

Title: PERIODIC mode repeats the cycle per trigger, no persistent connection.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_START
   - SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_CLOSURE
   - SW_REQ_CONNECTION_FLOW_PER_TRIGGER_PERIODIC_NO_OVERLAP
   - SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_PERIODIC
Objective: Happy — Verify one closed connection cycle is created for each PERIODIC trigger.
Steps:
   - 1. Start an in-process TCP echo peer on `127.0.0.1:<ephemeral-port>` and configure Connix CLIENT with mode `PERIODIC`, interval `1s`, payload `PING`, and a `4s` test duration.
   - 2. Collect cycle-start/close timestamps, peer connection count, and socket state while Connix runs for four triggers.
   - 3. Inspect the interval between each close and the next connect, then stop Connix and close the peer.

Expectations:
   - 1. The peer and PERIODIC CLIENT start successfully.
   - 2. Four cycles each contain one connect, send, receive, and close; the peer sees four connections and four `PING` payloads.
   - 3. No Connix socket remains established between cycles, cycle intervals do not overlap, and the process stops cleanly after the test duration.

TestData:
   - Protocol `TCP`; host `127.0.0.1`; ephemeral port; mode `PERIODIC`; interval `1s`; duration `4s`; payload `PING`.
   - Capture timestamps, peer count, `/proc/<pid>/fd` or socket-state snapshots, and event vectors; teardown stops process and releases port.

### TC_FLOW_08 — Event/rule-triggered repeat, no persistent connection
*Type: Positive · Priority: Medium*

Title: Event/rule-triggered repeat, no persistent connection.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_START
   - SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_CLOSURE
Objective: Happy — Verify that two message events create two independent connection cycles.
Steps:
   - 1. Start a localhost TCP peer on `127.0.0.1:<ephemeral-port>` and configure Connix with a message rule `message_receipt("TRIGGER") -> start CLIENT cycle(payload="PING")`.
   - 2. Deliver exactly two `TRIGGER` messages to the event source, separated by at least `100ms`, and capture each cycle's event trace and socket state.
   - 3. Wait for both cycles to close and inspect the peer's received payloads and connection count.

Expectations:
   - 1. The peer and message-trigger rule initialize successfully.
   - 2. Each `TRIGGER` produces exactly one connect/send/receive/close cycle; the two cycle IDs are distinct and neither cycle overlaps the other.
   - 3. The peer receives exactly two `PING` payloads over two separate connections, and no connection remains open between events.

TestData:
   - TCP peer `127.0.0.1:<ephemeral-port>`; two event payloads `TRIGGER`; response `PONG`.
   - Rule `message_receipt("TRIGGER") -> CLIENT cycle(payload="PING")`; capture cycle IDs, events, socket state, and peer count.

### TC_FLOW_09 — Configuration causing more than one send per cycle is rejected at config-load time
*Type: Error · Priority: Medium*

Title: Configuration causing more than one send per cycle is rejected at config-load time.
Satisfies:
   - SW_REQ_SIMPLE_CONNECTION_FLOW
   - SW_REQ_CONNECTION_FLOW_SINGLE_SEND_REJECTION
   - SW_REQ_CONNECTION_FLOW_SINGLE_SEND_ERROR_DETAIL
Objective: Error — Verify duplicate send actions are rejected before a CLIENT cycle starts.
Steps:
   - 1. Create `$TEST_TMPDIR/multiple-send.json` with role `CLIENT`, protocol `TCP`, mode `ONETIME`, and two rules both bound to `connect`: `rule_alpha -> send("PING")` and `rule_beta -> send("SECOND")`.
   - 2. Load the configuration with a transport-factory spy and capture validation diagnostics, rule-validation calls, transport calls, and exit status.
   - 3. Confirm that no peer is started and no connection attempt is made after validation fails.

Expectations:
   - 1. The file parses successfully and both rules are identified as send actions for the same CLIENT cycle trigger.
   - 2. Configuration loading fails before transport construction, returns non-zero, and reports both `rule_alpha` and `rule_beta` as conflicting rules.
   - 3. The transport factory and socket layer receive zero calls; no send action or connection cycle is attempted.

TestData:
   - JSON: `{ "role": "CLIENT", "protocol": "TCP", "mode": "ONETIME", "rules": [{ "id": "rule_alpha", "event": "connect", "action": { "type": "send", "payload": "PING" } }, { "id": "rule_beta", "event": "connect", "action": { "type": "send", "payload": "SECOND" } }] }`.
   - Config path `$TEST_TMPDIR/multiple-send.json`; transport-factory/socket spies; capture diagnostic text and exit status; remove file afterward.
