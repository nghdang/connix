# Test Specification: SW_REQ_EVENT_DRIVEN

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the event children under the parent `SW_REQ_EVENT_DRIVEN`:
`SW_REQ_EVENT_MESSAGE_RECEIPT`, `SW_REQ_EVENT_CONNECT`, `SW_REQ_EVENT_ACCEPT`,
`SW_REQ_EVENT_CLOSE`, `SW_REQ_EVENT_TIMER`, `SW_REQ_EVENT_DRIVEN_TIMER_SOURCE`, and
`SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE`.

**Fit Criterion:** Each event child is tested independently: message receipt, connect, accept, close,
and timer events are recognized at their defined lifecycle points; PERIODIC uses the event-driven
timer source; and only cycle-start and cycle-close events are exposed within one cycle.

## Test Design Techniques

- Equivalence Partitioning: event kinds {message receipt, connect, accept, close, timer}.
- State Transition Testing: connection-state events observable within one cycle.

## Test Cases

### TC_EVT_01 — Rule fires on message receipt
*Type: Positive · Priority: High*

Title: Rule fires on message receipt.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_MESSAGE_RECEIPT
   - SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE
Objective: Happy — Verify that a message-receipt event invokes its configured response within the same cycle.
Steps:
   - 1. Create a localhost TCP CLIENT/SERVER topology on a dynamically allocated port; configure the SERVER rule `message_receipt(pattern="PING") -> send("PONG")`.
   - 2. Start the SERVER, connect an in-process CLIENT, and send exactly the bytes `PING` once.
   - 3. Capture the SERVER event trace, response bytes, and connection-cycle identifiers until the cycle closes.

Expectations:
   - 1. The SERVER starts listening and accepts the message-receipt rule without a configuration error.
   - 2. The SERVER receives exactly one `PING` message and emits exactly one `message_receipt` event for that message.
   - 3. The `send("PONG")` action executes within the same cycle; the CLIENT receives exactly `PONG` before the connection closes.

TestData:
   - TCP `127.0.0.1:<ephemeral-port>`; one SERVER and one in-process CLIENT; ONETIME mode.
   - Rule: `message_receipt(pattern="PING") -> send("PONG")`.
   - Capture ordered events, cycle ID, and byte buffers; cleanup closes both sockets.

### TC_EVT_02 — Rule fires on connect event
*Type: Positive · Priority: High*

Title: Rule fires on connect event.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_CONNECT
   - SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE
Objective: Happy — Verify that client connect and server accept events trigger their configured rules within the same cycle.
Steps:
   - 1. Create a localhost TCP topology with one Connix CLIENT and one Connix SERVER; allocate an ephemeral port, start the server, and wait until its accept socket is ready.
   - 2. Add a rule to the CLIENT for the `connect` event with action `send("PING")`.
   - 3. Add a rule to the SERVER for the `accept` event with action `send("READY")`.
   - 4. Establish the connection from the CLIENT to the SERVER and capture the event trace and both endpoint outputs.

Expectations:
   - 1. The CLIENT and SERVER fixtures start successfully and the SERVER is listening on `127.0.0.1:<ephemeral-port>`.
   - 2. The CLIENT accepts the `connect` rule and stores the `send("PING")` action without a configuration error.
   - 3. The SERVER accepts the `accept` rule and stores the `send("READY")` action without a configuration error.
   - 4. The trace contains one CLIENT `connect` event and one SERVER `accept` event; both rules execute in the connection cycle, and the peer receives `PING` and `READY` respectively.

TestData:
   - Protocol: TCP over `127.0.0.1` with a dynamically allocated port.
   - Roles: one CLIENT and one SERVER, both ONETIME.
   - Rules: CLIENT `connect -> send("PING")`; SERVER `accept -> send("READY")`.
   - Capture: thread-safe event collector and byte buffers for each endpoint.
   - Cleanup: close both connections and verify the allocated port is released.

### TC_EVT_03 — Rule fires on close event
*Type: Positive · Priority: High*

Title: Rule fires on close event.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_CLOSE
   - SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE
Objective: Happy — Verify that the close event triggers its configured action before the cycle is finalized.
Steps:
   - 1. Create a localhost TCP CLIENT/SERVER topology on a dynamically allocated port and configure the SERVER with `close -> send("BYE")`.
   - 2. Establish one connection, exchange `PING`/`PONG`, then have the CLIENT close its socket.
   - 3. Capture the SERVER event trace and bytes received after the close notification.

Expectations:
   - 1. The SERVER starts listening and accepts exactly one connection.
   - 2. The CLIENT closes successfully after receiving `PONG`.
   - 3. The SERVER trace contains one `close` event followed by the configured `send("BYE")` action, and no second action is emitted.

TestData:
   - TCP `127.0.0.1:<ephemeral-port>`; CLIENT and SERVER; ONETIME mode.
   - Payloads: CLIENT sends `PING`; SERVER replies `PONG`; close rule sends `BYE`.
   - Capture: ordered event vector and SERVER output buffer; cleanup closes the server and removes the fixture.

### TC_EVT_04 — Rule fires on timer interval
*Type: Positive · Priority: High*

Title: Rule fires on timer interval.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_TIMER
   - SW_REQ_EVENT_DRIVEN_TIMER_SOURCE
Objective: Happy — Verify that a timer rule executes once for each scheduled interval.
Steps:
   - 1. Create a fake clock and configure a timer rule with interval `2s` and action `send("HEARTBEAT")`.
   - 2. Start the event engine with the fake clock at `2026-01-01T00:00:00Z` and advance it by `2s`, `2s`, and `2s` in separate operations.
   - 3. Read the event and action collectors after each clock advance.

Expectations:
   - 1. The rule and timer source are created successfully with no real-time sleep required.
   - 2. Each `2s` clock advance produces exactly one timer event.
   - 3. The action collector contains exactly three `send("HEARTBEAT")` actions, ordered by the three timer events, with no duplicate action.

TestData:
   - Fake clock start `2026-01-01T00:00:00Z`; advances `[2s, 2s, 2s]`.
   - Rule: `timer(interval=2s) -> send("HEARTBEAT")`.
   - Capture: timer-event vector and action vector; no external socket required.

### TC_EVT_05 — PERIODIC-mode interval and timer event share one mechanism
*Type: Positive · Priority: High*

Title: PERIODIC-mode interval and timer event share one mechanism.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_TIMER
   - SW_REQ_EVENT_DRIVEN_TIMER_SOURCE
Objective: Happy — Verify that PERIODIC triggers and timer events are produced by one deterministic timer source.
Steps:
   - 1. Create a fake clock and configure PERIODIC mode with interval `2s`, plus a timer rule `timer(interval=2s) -> record("TIMER")`.
   - 2. Advance the fake clock by `2s` three times while recording cycle-start events and timer events.
   - 3. Compare timestamps and sequence numbers for each cycle-start event and timer event.

Expectations:
   - 1. PERIODIC mode and the timer rule register successfully against the same injected clock.
   - 2. Each clock advance produces one cycle trigger and one corresponding timer event, with no extra timer event.
   - 3. The paired events have the same trigger timestamp/sequence for all three advances; no independent drift occurs.

TestData:
   - Fake clock start `2026-01-01T00:00:00Z`; advances `[2s, 2s, 2s]`.
   - Mode: PERIODIC, interval `2s`; rule: timer `2s` -> `record("TIMER")`.
   - Capture: cycle-start and timer-event records containing timestamp and sequence number.

### TC_EVT_06 — Only start/end connection-state events observable within one cycle
*Type: Boundary · Priority: Medium*

Title: Only start/end connection-state events observable within one cycle.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_CONNECT
   - SW_REQ_EVENT_ACCEPT
   - SW_REQ_EVENT_CLOSE
   - SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE
Objective: Edge — Verify the exact connection-state event scope exposed during one cycle.
Steps:
   - 1. Create a localhost TCP CLIENT/SERVER topology and register recording rules for `connect`, `accept`, `close`, `send`, and `receive` events.
   - 2. Execute one CLIENT/SERVER exchange: CLIENT sends `PING`; SERVER replies `PONG`; then close both sockets.
   - 3. Read the ordered event records for the single cycle ID.

Expectations:
   - 1. Both endpoints start and all recording rules are accepted.
   - 2. The exchange completes with `PING` and `PONG` and exactly one cycle ID.
   - 3. The exposed connection-state event records contain only the permitted cycle-start (`connect`/`accept`) and cycle-close (`close`) events; no mid-cycle state event is exposed.

TestData:
   - TCP `127.0.0.1:<ephemeral-port>`; CLIENT/SERVER; ONETIME mode.
   - Payloads `PING` and `PONG`; recording rules for all five event names.
   - Capture ordered events with cycle IDs; cleanup closes sockets and releases the port.

### TC_EVT_07 — No manual intervention required between event and action
*Type: Positive · Priority: Medium*

Title: No manual intervention required between event and action.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE
Objective: Happy — Verify that a configured event action executes without user confirmation.
Steps:
   - 1. Create a localhost TCP CLIENT/SERVER topology and configure the CLIENT rule `connect -> send("PING")`.
   - 2. Start the SERVER, launch the CLIENT, and establish one connection without sending keyboard input after process start.
   - 3. Capture the CLIENT event/action trace and process standard input/output for the complete cycle.

Expectations:
   - 1. The CLIENT and SERVER start successfully with the rule accepted.
   - 2. The CLIENT connect event is delivered automatically and `send("PING")` executes without a prompt or confirmation request.
   - 3. The trace contains the connect event and send action in order; standard input is unused and the cycle completes normally.

TestData:
   - TCP `127.0.0.1:<ephemeral-port>`; CLIENT and SERVER; ONETIME mode.
   - Rule: `connect -> send("PING")`; no interactive input.
   - Capture: ordered event/action trace and stdout/stderr; cleanup closes both sockets.

### TC_EVT_08 — Event kind not matching any configured rule produces no action
*Type: Negative · Priority: Medium*

Title: Event kind not matching any configured rule produces no action.
Satisfies:
   - SW_REQ_EVENT_DRIVEN
   - SW_REQ_EVENT_MESSAGE_RECEIPT
   - SW_REQ_EVENT_CONNECT
   - SW_REQ_EVENT_ACCEPT
   - SW_REQ_EVENT_CLOSE
   - SW_REQ_EVENT_TIMER
Objective: Error — Verify that event kind not matching any configured rule produces no action.
Steps:
   - 1. Establish the precondition: Rule configured only for message-receipt event.
   - 2. Trigger a close event (for which no rule exists).

Expectations:
   - 1. No rule action executes; Connix does not error, simply takes no rule-driven action for the unmatched event.

TestData:
   - Event=close, no matching rule.
   - Fixture setup required by the precondition: Rule configured only for message-receipt event.
