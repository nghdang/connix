# Task 37: Add Runtime View for Transport Component

## Objective
Create the detailed design runtime view diagram (`runtime_view.puml`) for the
`transport` component under `connix-core/infrastructure/transport/dd/`, and
document comprehensive runtime sequence specifications in `detailed_design.md`.

## Scope
- `connix/connix-core/infrastructure/transport/dd/runtime_view.puml`
- `connix/connix-core/infrastructure/transport/dd/detailed_design.md`
- `.ai/tasks/37_add_transport_runtime_view.md`

## Important Decisions & Assumptions
- Modeled the six core dynamic execution scenarios for the Transport
  component corresponding to Use Cases UC1 through UC6:
  - **Scenario 1: Transport Factory Creation (UC1)**:
    - `Client` requests `TransportFactory::createTransport(protocol)` passing
      a strongly typed `TransportProtocol` (`TCP`, `UDP`, `UDS_STREAM`,
      `UDS_DATAGRAM`).
    - `TransportFactory` instantiates internal `Socket` abstraction (`ISocket`)
      and concrete transport (`TcpTransport`, `UdpTransport`, or
      `UdsTransport`) in `TransportState::CLOSED` state.
    - If an unsupported protocol is requested, raises `TransportException`
      with `TransportErrorCode::UNSUPPORTED_PROTOCOL`.
  - **Scenario 2: Outbound Connection Establishment (UC2)**:
    - `Client` calls `ITransport::connect(endpoint, timeoutMs)`.
    - `Transport` transitions state to `CONNECTING`.
    - Opens descriptor if closed via `Socket::open(domain, type, protocol)`
      (POSIX `socket(2)`).
    - `Socket` initiates non-blocking connection via POSIX `connect(2)` and
      monitors write-readiness (`POLLOUT`) via `poll(2)` bounded by 5 seconds
      (`SW_REQ_CONNECTION_MANAGEMENT_CONNECT_AND_ACCEPT_TIMEOUT_BOUNDS`).
    - On success: caches endpoints and transitions to `CONNECTED`.
    - On timeout: closes descriptor immediately via `Socket::close()`, resets
      state to `CLOSED`, and throws `TransportException(OPERATION_TIMEOUT)`
      (`SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`).
  - **Scenario 3: Server Inbound Connection Acceptance (UC3)**:
    - Binds local endpoint with `SocketOption::REUSE_ADDRESS` (POSIX
      `setsockopt(2)` and `bind(2)`), transitioning to `BOUND`.
    - Starts passive listening via POSIX `listen(2)`, transitioning to
      `LISTENING`.
    - `Client` calls `accept(timeoutMs)` (bounded by 5 seconds).
    - `Socket` polls for read-readiness (`POLLIN`) and calls `accept4(2)`
      (or `accept(2)`).
    - Instantiates a new connected `TcpTransport` (or `UdsTransport`)
      wrapping `clientSocket` with local/remote endpoints, returning it to
      `Client`.
  - **Scenario 4: Data Transmission (UC4)**:
    - `Client` calls `send(data, timeoutMs)` (bounded by 5 seconds per
      `SW_REQ_CONNECTION_MANAGEMENT_SEND_TIMEOUT_BOUND`).
    - `Socket` polls for write-readiness (`POLLOUT`) and calls POSIX `send(2)`
      (with `MSG_NOSIGNAL`) or `sendto(2)` for datagrams.
    - On timeout: closes socket descriptor, resets to `CLOSED`, and throws
      `TransportException(OPERATION_TIMEOUT)`.
  - **Scenario 5: Data Reception (UC5)**:
    - `Client` calls `receive(maxBytes, timeoutMs)` (bounded by 10 seconds per
      `SW_REQ_CONNECTION_MANAGEMENT_RECEIVE_TIMEOUT_BOUND`).
    - `Socket` polls for read-readiness (`POLLIN`) and calls POSIX `recv(2)`
      (or `recvfrom(2)` for datagrams).
    - On graceful peer disconnect (`recv(2)` returns 0): transitions to
      `DISCONNECTED` and throws `TransportException(CONNECTION_CLOSED)`.
    - On timeout: closes descriptor, resets to `CLOSED`, and throws
      `TransportException(OPERATION_TIMEOUT)`.
  - **Scenario 6: Transport Termination and Resource Teardown (UC6)**:
    - `Client` calls `close()`.
    - `Socket` closes descriptor via POSIX `close(2)` and unlinks UDS socket
      paths if applicable (`unlink(2)`).
    - Resets state to `CLOSED` and clears cached endpoints.
    - Guarantees idempotent and safe execution from any state.
- Documentation structure alignment:
  - Added Section 6: Runtime View embedding `runtime_view.puml` and
    Section 6.1: Runtime Sequence Specifications.
  - Renumbered Section 6 (Design Decisions) to Section 7.
  - Fixed Static View subsection numbering from 2.2 to 3.2.

## Changes Made
- Created `connix/connix-core/infrastructure/transport/dd/runtime_view.puml`
  containing the sequence diagram for Scenarios 1 to 6.
- Updated `connix/connix-core/infrastructure/transport/dd/detailed_design.md`:
  - Added Section 6 (Runtime View) embedding `runtime_view.puml`.
  - Added detailed sequence specifications for Scenarios 1-6.
  - Renumbered Section 6 (Design Decisions) to Section 7.
  - Corrected Section 3 subsection number to 3.2.
- Created task summary file `.ai/tasks/37_add_transport_runtime_view.md`.

## Validation Performed
- Validated PlantUML syntax across all transport diagrams (`static_view.puml`,
  `data_structures.puml`, `interface_view.puml`, `use_case.puml`,
  `runtime_view.puml`) using `plantuml -syntaxcheck` (clean, 0 errors).
- Verified line lengths of all newly created and modified files using
  `./scripts/check_line_length.sh` (all <= 80 characters).
- Verified spelling across modified files and task tracking records using
  `./scripts/check_spelling.sh` (clean, 0 issues).
- Checked `git status` to ensure clean workspace state.
