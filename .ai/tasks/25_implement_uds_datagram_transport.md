# Task 25: Implement UDS DATAGRAM Transport

## Objective
Implement and verify datagram-oriented IPC communication over Unix Domain Sockets (`UDS_DATAGRAM` via `SOCK_DGRAM`) in `UdsTransport` under `connix-core/infrastructure/transport/`, ensuring support for connectionless packet transfer, bound listener reception with sender endpoint extraction, and connected peer transfer.

## Scope
- `connix/connix-core/infrastructure/transport/inc/ConnixCore/Infrastructure/Transport/UdsTransport.hpp`
- `connix/connix-core/infrastructure/transport/src/UdsTransport.cpp`
- `connix/connix-core/infrastructure/transport/tst/ut/UdsTransportUnitTest.cpp`
- `.ai/tasks/25_implement_uds_datagram_transport.md`

## Important Decisions & Assumptions
- **Unified UDS Abstraction**: Followed the component detailed design (`detailed_design.md` and `interface_view.puml`) where `UdsTransport` handles both stream and datagram semantics via protocol parameterization (`TransportProtocol::UDS_STREAM` vs `TransportProtocol::UDS_DATAGRAM`).
- **Connectionless Datagram IPC Semantics**:
  - `bind`: Validates filesystem socket path, opens socket with `SocketDomain::UNIX`, `SocketType::DATAGRAM`, and `SocketProtocol::DEFAULT`, and binds to local socket path.
  - `listen` & `accept`: Datagram sockets do not support stream listening or peer session acceptance; explicitly throws `TransportException` with `LISTEN_FAILED` and `ACCEPT_FAILED`.
  - `connect`: Binds a default destination path to the datagram socket descriptor, transitions state to `CONNECTED`, and enables `send()` and `receive()`.
  - `send`: Supports both connected datagram mode (via `m_socket->send()`) and bound datagram mode with target peer endpoint (via `m_socket->sendTo()`).
  - `receive`: In connected datagram mode, receives via `m_socket->receive()`. In bound datagram mode, receives incoming datagrams and extracts the sender's filesystem socket path via `m_socket->receiveFrom()`, recording it in `m_remoteEndpoint` to support echo and reply workflows.
  - `close`: Idempotent cleanup of socket descriptor and state reset to `CLOSED`.
- **Requirements Traceability**:
  - `SW_REQ_PROTOCOL_SUPPORT_UDS`: Implements datagram IPC communication over Unix Domain Sockets.
  - `SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`: Closes socket immediately when connection, send, or receive operations time out.

## Changes Made
- Enriched class-level Doxygen docstrings in `UdsTransport.hpp` explicitly detailing both stream (`UDS_STREAM`) and datagram (`UDS_DATAGRAM`) semantics.
- Enhanced `UdsTransportUnitTest.cpp` with additional test cases for UDS datagram workflows:
  - `ConnectDatagramSuccess`: Verifies connecting to a datagram UDS peer opens `SocketType::DATAGRAM` and transitions to `CONNECTED`.
  - `SendConnectedDatagramSuccess`: Verifies datagram transmission over a connected UDS socket.
  - `ReceiveConnectedDatagramSuccess`: Verifies datagram reception over a connected UDS socket.
  - `ReceiveAndReplyBoundDatagram`: Verifies server-side datagram reception extracting the sender path via `receiveFrom()`, followed by replying via `sendTo()`.
- Created task tracking log `.ai/tasks/25_implement_uds_datagram_transport.md`.

## Validation Performed
- Built native debug target with tests: `./scripts/build_application_native.sh` (clean).
- Ran all unit tests: `./scripts/run_unit_tests.sh` (100% tests passed, 12/12 test suites, 37/37 tests in `UdsTransportUnitTest`).
- Verified line coverage: `./scripts/check_coverage.sh` (100.0% line coverage achieved across all files, 152/152 lines in `UdsTransport.cpp`).
- Ran integration tests: `./scripts/run_integration_tests.sh` (passed).
- Ran static analysis: `./scripts/run_clang_tidy.sh` (0 warnings/errors across all files and tests).
- Checked formatting: `./scripts/run_clang_format.sh` (clean).
- Checked spelling: `./scripts/check_spelling.sh` (213 files checked, 0 errors).
- Built Doxygen docstrings: `./scripts/build_docstring.sh --clean && ./scripts/build_docstring.sh --clean-only` (clean, verified `UdsTransport` documentation generated and wiped).
- Built release target and packaging: `./scripts/build_application_release.sh` (clean, verified deployment).
