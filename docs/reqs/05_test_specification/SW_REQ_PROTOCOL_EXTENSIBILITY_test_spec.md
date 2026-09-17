# Test Specification: SW_REQ_PROTOCOL_EXTENSIBILITY

**Source:** `docs/reqs/04_nonfunctional_requirements/maintainability.rst`

## Requirement Summary

connix-core SHALL expose a common client/server interface that a new protocol implementation can
satisfy without modifying the core orchestration engine.

**Fit Criterion:** A new protocol implementation can be added by implementing the existing
client/server interface, with no changes required to the core orchestration engine.

## Test Design Techniques

- Interface conformance / contract testing (verify a new implementation can satisfy the interface
  in isolation).
- Static analysis / code-review-based check (no orchestration-engine diff required) — this
  requirement is fundamentally an architectural/design-conformance check more than a runtime
  black-box test.

## Test Cases

### TC_EXT_01 — New protocol implementation added without modifying orchestration engine
*Type: Positive · Priority: High*

Title: New protocol implementation added without modifying orchestration engine.
Satisfies:
   - SW_REQ_PROTOCOL_EXTENSIBILITY
Objective: Happy — Verify that new protocol implementation added without modifying orchestration engine.
Steps:
   - 1. Establish the precondition: `ITransport` (`ConnixCore::Communication::Transport`) is implemented in `communication/transport/`; a 4th protocol (e.g. a mock/test protocol) is implemented against it.
   - 2. Implement a new protocol class satisfying `ITransport`.
   - 3. Register/wire it via the documented extension point only.
   - 4. Diff the orchestration engine (`orchestration/engine/`) source before/after.

Expectations:
   - 1. The new protocol works end-to-end (connect/send/receive/close) and the orchestration engine's source files show zero diff.

TestData:
   - New protocol implementation (e.g. `MockProtocolTransport`).
   - Fixture setup required by the precondition: `ITransport` (`ConnixCore::Communication::Transport`) is implemented in `communication/transport/`; a 4th protocol (e.g. a mock/test protocol) is implemented against it.

### TC_EXT_02 — Existing protocols (TCP/UDP/UDS) unaffected after adding a new protocol
*Type: Positive · Priority: High*

Title: Existing protocols (TCP/UDP/UDS) unaffected after adding a new protocol.
Satisfies:
   - SW_REQ_PROTOCOL_EXTENSIBILITY
Objective: Happy — Verify existing transports remain behaviorally unchanged after an extension.
Steps:
   - 1. Build a baseline with `TcpTransport`, `UdpTransport`, and `UdsTransport`; run their localhost exchange fixtures and save event traces, payloads, exit statuses, and orchestration-engine source hash.
   - 2. Add `MockProtocolTransport` implementing `ITransport` through the documented extension point, without editing existing transport or orchestration source files.
   - 3. Re-run the same TCP, UDP, and UDS fixtures and compare their results with the baseline.

Expectations:
   - 1. The baseline tests complete successfully and the baseline artifacts are recorded.
   - 2. The mock transport builds and registers without changing existing transport behavior or the orchestration-engine hash.
   - 3. TCP, UDP, and UDS produce the same event sequence, payloads, exit statuses, and cleanup results as the baseline.

TestData:
   - Existing `TcpTransport`, `UdpTransport`, `UdsTransport` fixtures with `PING`/`PONG` and localhost ephemeral ports/UDS path.
   - New `MockProtocolTransport`; baseline/follow-up traces and source hashes; cleanup all transport fixtures.

### TC_EXT_03 — Interface omission caught at compile/interface-conformance time
*Type: Negative · Priority: Medium*

Title: Interface omission caught at compile/interface-conformance time.
Satisfies:
   - SW_REQ_PROTOCOL_EXTENSIBILITY
Objective: Error — Verify that interface omission caught at compile/interface-conformance time.
Steps:
   - 1. Establish the precondition: An incomplete new-protocol implementation missing a required `ITransport` method.
   - 2. Implement a protocol class that omits a required `ITransport` method (e.g. `close()`).
   - 3. Attempt to build/register it.

Expectations:
   - 1. Build fails or a clear compile-time/interface error is raised, rather than a silent runtime failure.

TestData:
   - incomplete implementation.
   - Fixture setup required by the precondition: An incomplete new-protocol implementation missing a required `ITransport` method.

### TC_EXT_04 — Interface is documented and stable for library integrators
*Type: Positive · Priority: Medium*

Title: Interface is documented and stable for library integrators.
Satisfies:
   - SW_REQ_PROTOCOL_EXTENSIBILITY
Objective: Happy — Verify that interface is documented and stable for library integrators.
Steps:
   - 1. Establish the precondition: `ITransport` public header exists under `communication/transport/`.
   - 2. Review the `ITransport` header for documentation completeness (per the "Library integrator" persona's needs from `01_project_drivers/users.rst`).

Expectations:
   - 1. Interface methods are documented sufficiently for an external integrator to implement a new protocol without reading orchestration-engine internals.

TestData:
   - `ITransport` header.
   - Fixture setup required by the precondition: `ITransport` public header exists under `communication/transport/`.

### TC_EXT_05 — Core orchestration engine has no protocol-specific conditional logic
*Type: Negative · Priority: Medium*

Title: Core orchestration engine has no protocol-specific conditional logic.
Satisfies:
   - SW_REQ_PROTOCOL_EXTENSIBILITY
Objective: Error — Verify that core orchestration engine has no protocol-specific conditional logic.
Steps:
   - 1. Establish the precondition: `orchestration/engine/` source available.
   - 2. Inspect the orchestration engine (`ConnixCore::Orchestration::Engine`) for protocol-specific branches (e.g. `if protocol == TCP`).

Expectations:
   - 1. No protocol-specific conditionals exist in the orchestration engine; all protocol differences are encapsulated behind `ITransport`.

TestData:
   - orchestration engine source.
   - Fixture setup required by the precondition: `orchestration/engine/` source available.

## Notes

This requirement is architectural rather than purely behavioral. The target interface is now
concretely named (OP-04, see `OPEN_POINTS.md`): `ITransport` under `communication/transport/`
(`ConnixCore::Communication::Transport`), per
`docs/arch/05_building_block_view/05_building_block_view.rst`, satisfying both
`SW_REQ_MULTIPLE_PROTOCOLS` and this requirement. It is scheduled early in the build order per
`docs/arch/OPEN_POINTS_FOR_PO.md` (point 3). Full test-case *execution* remains blocked until
`ITransport` actually lands in the source tree — re-validate this spec against it at that point.
