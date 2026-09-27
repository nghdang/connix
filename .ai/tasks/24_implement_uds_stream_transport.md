# Task 24: Implement UDS STREAM Transport

## Objective
Implement the concrete `UdsTransport` class inside `connix-core/infrastructure/transport/` supporting stream-oriented IPC communication over Unix Domain Sockets (`AF_UNIX`) and filesystem socket paths, adhering to clean architecture and component detailed design.

## Scope
- `connix/connix-core/infrastructure/transport/inc/ConnixCore/Infrastructure/Transport/UdsTransport.hpp`
- `connix/connix-core/infrastructure/transport/src/UdsTransport.cpp`
- `connix/connix-core/infrastructure/transport/tst/ut/UdsTransportUnitTest.cpp`
- `connix/connix-core/infrastructure/transport/tst/ut/CMakeLists.txt`
- `.ai/tasks/24_implement_uds_stream_transport.md`

## Important Decisions & Assumptions
- **Component-Private Header Placement**: Located `UdsTransport.hpp` under `transport/inc/ConnixCore/Infrastructure/Transport/` following the project's three-tier header architecture (private to `connix-core`, excluded from the public deployment package).
- **Protocol Parameterization**: Supports both `TransportProtocol::UDS_STREAM` (default) and `TransportProtocol::UDS_DATAGRAM` via constructor parameterization. Throws `TransportException` with `UNSUPPORTED_PROTOCOL` if any non-UDS protocol is supplied.
- **UDS Stream Semantics**:
  - `bind`: Validates non-empty filesystem path (`INVALID_ADDRESS`), opens socket with `SocketDomain::UNIX`, `SocketType::STREAM`, and `SocketProtocol::DEFAULT`, and binds to local endpoint path.
  - `listen`: Enforces `BOUND` state, calls `m_socket->listen()`, and transitions to `LISTENING`.
  - `accept`: Validates `LISTENING` state, accepts peer client socket, and returns a new connected `UdsTransport` session instance in `CONNECTED` state.
  - `connect`: Validates destination path, opens socket if closed, and connects to remote UDS path. Closes socket on timeout or failure per `SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`.
  - `send` & `receive`: Bounded stream I/O over active connection. Closes socket descriptor upon timeout (`OPERATION_TIMEOUT`), transitions to `DISCONNECTED` on remote closure.
  - `close`: Idempotent cleanup of socket descriptor and state reset to `CLOSED`.
- **Dependency Inversion**: Interacts exclusively with the `ISocket` interface via constructor injection, enabling mock testing with `MockISocket` without needing real filesystem socket nodes.
- **Rule of 5 Compliance**: Explicitly declared move constructor and move assignment as defaulted, and deleted copy operations to enforce move-only semantics.
- **Virtual Call in Destructor Prevention**: Created a private non-virtual `closeInternal() noexcept` helper invoked by both `~UdsTransport()` and `close()`.
- **Requirements Traceability**:
  - `SW_REQ_PROTOCOL_SUPPORT_UDS`: Implements stream-oriented IPC data transmission and reception over Unix Domain Sockets.
  - `SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`: Closes socket immediately when connection, send, or receive operations time out.

## Changes Made
- Created `UdsTransport.hpp` under `transport/inc/ConnixCore/Infrastructure/Transport/`.
- Implemented all `UdsTransport` methods in `transport/src/UdsTransport.cpp`.
- Created `UdsTransportUnitTest.cpp` in `transport/tst/ut/` with 33 exhaustive test cases covering constructors, protocol validation, empty path validation, stream binding, backlog listening, client session acceptance, connecting, timeout socket closure, stream send/receive, datagram fallback send/receive, and idempotent closure.
- Registered `UdsTransportUnitTest.cpp` in `tst/ut/CMakeLists.txt`.
- Created task tracking log `.ai/tasks/24_implement_uds_stream_transport.md`.

## Validation Performed
- Built native debug target with tests: `./scripts/build_application_native.sh` (clean).
- Ran all unit tests: `./scripts/run_unit_tests.sh` (100% tests passed, 12/12 test suites, 33/33 tests in `UdsTransportUnitTest`).
- Verified line coverage: `./scripts/check_coverage.sh` (100.0% line coverage achieved across all files, 152/152 lines in `UdsTransport.cpp`).
- Ran integration tests: `./scripts/run_integration_tests.sh` (passed).
- Ran static analysis: `./scripts/run_clang_tidy.sh` (0 warnings/errors across all files and tests).
- Checked formatting: `./scripts/run_clang_format.sh` (clean).
- Checked spelling: `./scripts/check_spelling.sh` (213 files checked, 0 errors).
- Built Doxygen docstrings: `./scripts/build_docstring.sh --clean && ./scripts/build_docstring.sh --clean-only` (clean, verified `UdsTransport` documentation generated and wiped).
- Built release target and packaging: `./scripts/build_application_release.sh` (clean, verified deployment).
