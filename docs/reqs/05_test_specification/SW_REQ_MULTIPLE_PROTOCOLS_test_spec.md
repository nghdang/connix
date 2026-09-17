# Test Specification: SW_REQ_MULTIPLE_PROTOCOLS

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the protocol children under the parent
`SW_REQ_MULTIPLE_PROTOCOLS`: `SW_REQ_PROTOCOL_TCP`, `SW_REQ_PROTOCOL_UDP`, and
`SW_REQ_PROTOCOL_UDS`.

**Fit Criterion:** Each protocol child is tested independently: Connix completes the specified
send/receive exchange when configured for TCP, UDP, and UDS.

## Test Design Techniques

- Equivalence Partitioning: protocol classes {TCP, UDP, UDS} and {invalid/unsupported protocol}.
- Error Guessing: unreachable endpoints, malformed addresses, permission errors on UDS paths.

## Test Cases

### TC_PROTO_01 — TCP send/receive exchange
*Type: Positive · Priority: High*

Title: TCP send/receive exchange.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_TCP
Objective: Happy — Verify one deterministic TCP send/receive exchange.
Steps:
   - 1. Start an in-process TCP peer on `127.0.0.1` with an ephemeral port; accept one connection and configure it to reply `PONG` after receiving `PING`.
   - 2. Start Connix CLIENT with protocol `TCP`, host `127.0.0.1`, the peer's allocated port, mode `ONETIME`, and payload `PING`.
   - 3. Capture the peer's received bytes, Connix's transmitted/received bytes, and the lifecycle event trace.

Expectations:
   - 1. The peer binds/listens successfully and reports its allocated port.
   - 2. Connix connects to the TCP peer and transmits exactly `PING`.
   - 3. Connix receives exactly `PONG`; the trace contains one connect, send, receive, and close in order, and the cycle exits successfully.

TestData:
   - Protocol `TCP`; host `127.0.0.1`; dynamically allocated ephemeral port.
   - Payload `PING`; peer response `PONG`; mode `ONETIME`.
   - Capture buffers and ordered event collector; close peer and verify port release in teardown.

### TC_PROTO_02 — UDP send/receive exchange
*Type: Positive · Priority: High*

Title: UDP send/receive exchange.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_UDP
Objective: Happy — Verify one deterministic UDP datagram exchange.
Steps:
   - 1. Start an in-process UDP peer bound to `127.0.0.1` on an ephemeral port; configure it to reply `PONG` to one received datagram `PING`.
   - 2. Start Connix CLIENT with protocol `UDP`, host `127.0.0.1`, the peer's allocated port, mode `ONETIME`, and payload `PING`.
   - 3. Capture the peer datagram, Connix response datagram, event trace, and process exit status.

Expectations:
   - 1. The UDP peer binds successfully and reports its allocated port.
   - 2. The peer receives exactly one datagram containing `PING` from Connix.
   - 3. Connix receives exactly one datagram containing `PONG`, records the UDP exchange, and exits successfully without requiring a persistent connection.

TestData:
   - Protocol `UDP`; host `127.0.0.1`; dynamically allocated UDP port.
   - Datagram payload `PING`; response `PONG`; mode `ONETIME`.
   - Capture datagram buffers and event collector; close the peer and verify the port is released.

### TC_PROTO_03 — UDS send/receive exchange
*Type: Positive · Priority: High*

Title: UDS send/receive exchange.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_UDS
Objective: Happy — Verify one deterministic Unix Domain Socket exchange.
Steps:
   - 1. Create a unique socket path under the test temporary directory, start an in-process UDS peer at that path, and wait until the socket file exists and accepts connections.
   - 2. Start Connix CLIENT with protocol `UDS`, the unique socket path, mode `ONETIME`, and payload `PING`; configure the peer to reply `PONG`.
   - 3. Capture the peer bytes, Connix bytes, lifecycle events, socket-file state, and exit status.

Expectations:
   - 1. The UDS peer creates the socket file and becomes ready without colliding with another test.
   - 2. Connix connects through the socket path, sends exactly `PING`, and receives exactly `PONG`.
   - 3. The event trace contains one connect, send, receive, and close in order; Connix exits successfully, the connection is closed, and teardown removes the socket file.

TestData:
   - Protocol `UDS`; unique path `$TEST_TMPDIR/connix-uds-<test-id>.sock`; mode `ONETIME`.
   - Payload `PING`; peer response `PONG`.
   - Capture byte buffers and ordered event collector; cleanup unlinks the socket path.

### TC_PROTO_04 — Same invocation pattern across protocols
*Type: Positive · Priority: Medium*

Title: Same invocation pattern across protocols.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_TCP
   - SW_REQ_PROTOCOL_UDP
   - SW_REQ_PROTOCOL_UDS
Objective: Happy — Verify that one common test flow succeeds for TCP, UDP, and UDS with only transport addressing changed.
Steps:
   - 1. Start three local peers: TCP on an ephemeral port, UDP on an ephemeral port, and UDS at `$TEST_TMPDIR/connix-proto-<test-id>.sock`; configure each to return `PONG` for `PING`.
   - 2. Create three otherwise identical ONETIME CLIENT configurations, changing only `protocol` and its address fields.
   - 3. Execute the three configurations and capture sent/received bytes, lifecycle events, exit status, and cleanup state.

Expectations:
   - 1. All three peers become ready and expose distinct addresses.
   - 2. Each configuration is accepted without transport-specific options beyond its address.
   - 3. Every run sends exactly `PING`, receives exactly `PONG`, completes the expected cycle, exits successfully, and releases its port/socket path.

TestData:
   - Common mode `ONETIME`, payload `PING`, response `PONG`.
   - Addresses: TCP/UDP `127.0.0.1:<ephemeral-port>`; UDS `$TEST_TMPDIR/connix-proto-<test-id>.sock`.
   - Three byte buffers and event collectors; teardown closes peers and unlinks the UDS path.

### TC_PROTO_05 — TCP connection refused
*Type: Negative · Priority: High*

Title: TCP connection refused.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_TCP
Objective: Error — Verify that a TCP connection failure is reported deterministically.
Steps:
   - 1. Reserve an ephemeral TCP port, close the reservation, and verify with a connection probe that no process is listening on `127.0.0.1:<port>`.
   - 2. Start Connix CLIENT in ONETIME mode with protocol `TCP`, host `127.0.0.1`, the closed port, and payload `PING`; capture stderr, exit status, and elapsed time.

Expectations:
   - 1. The connection probe confirms the target port is closed immediately before Connix starts.
   - 2. Connix reports a TCP connection error containing the target address/port, exits non-zero within `2s`, performs no send/receive action, and does not crash.

TestData:
   - Protocol `TCP`; target `127.0.0.1:<reserved-then-closed-port>`; mode `ONETIME`; payload `PING`.
   - Capture stderr, exit status, elapsed time, and event collector; no peer fixture.

### TC_PROTO_06 — UDP send to non-listening port
*Type: Boundary · Priority: Medium*

Title: UDP send to non-listening port.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_UDP
Objective: Edge — Verify UDP behavior when no peer is listening.
Steps:
   - 1. Reserve an ephemeral UDP port, close the reservation, and verify that no test peer is bound to `127.0.0.1:<port>`.
   - 2. Start Connix CLIENT in ONETIME mode with protocol `UDP`, the closed target port, payload `PING`, and receive timeout `200ms`.
   - 3. Capture the send event, any response-timeout event, stderr, exit status, and elapsed time.

Expectations:
   - 1. The target port is unbound immediately before Connix starts.
   - 2. Connix sends exactly one UDP datagram containing `PING`; it does not claim that a peer connection was established.
   - 3. With no response, Connix reports the defined response-timeout result within the configured timeout, exits without hanging, and records no received payload.

TestData:
   - Protocol `UDP`; target `127.0.0.1:<reserved-then-closed-port>`; mode `ONETIME`.
   - Payload `PING`; receive timeout `200ms`; no UDP peer.
   - Capture event vector, stderr, exit status, and elapsed time.

### TC_PROTO_07 — UDS socket path does not exist
*Type: Negative · Priority: High*

Title: UDS socket path does not exist.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_UDS
Objective: Error — Verify that a missing UDS endpoint is reported without a crash.
Steps:
   - 1. Create a unique path `$TEST_TMPDIR/connix-missing-<test-id>.sock`, ensure it does not exist, and verify no process owns that path.
   - 2. Start Connix CLIENT in ONETIME mode with protocol `UDS`, the missing path, and payload `PING`; capture stderr, exit status, and elapsed time.

Expectations:
   - 1. The socket path is absent immediately before Connix starts.
   - 2. Connix reports a UDS endpoint error identifying the path, exits non-zero within `2s`, performs no send/receive action, and does not crash.

TestData:
   - Protocol `UDS`; path `$TEST_TMPDIR/connix-missing-<test-id>.sock`; mode `ONETIME`; payload `PING`.
   - Capture stderr, exit status, elapsed time, and event collector; remove any test artifact in teardown.

### TC_PROTO_08 — Unsupported/invalid protocol value rejected
*Type: Error · Priority: Medium*

Title: Unsupported/invalid protocol value rejected.
Satisfies:
   - SW_REQ_MULTIPLE_PROTOCOLS
   - SW_REQ_PROTOCOL_TCP
   - SW_REQ_PROTOCOL_UDP
   - SW_REQ_PROTOCOL_UDS
Objective: Error — Verify that an unsupported protocol identifier is rejected before transport startup.
Steps:
   - 1. Create a minimal configuration with `protocol: SCTP`, role `CLIENT`, host `127.0.0.1`, port `1`, mode `ONETIME`, and payload `PING`; do not start any peer.
   - 2. Launch Connix with the configuration and capture parser/validation output, exit status, and transport-construction calls.

Expectations:
   - 1. The configuration file is created successfully and no network peer is required because validation must precede transport startup.
   - 2. Connix reports an error containing `SCTP`, exits non-zero, constructs no TCP/UDP/UDS transport, and performs no socket operation.

TestData:
   - Config: `{ "protocol": "SCTP", "role": "CLIENT", "host": "127.0.0.1", "port": 1, "mode": "ONETIME", "payload": "PING" }`.
   - Capture validation output, exit status, and a transport-factory mock; cleanup removes the temporary config.
