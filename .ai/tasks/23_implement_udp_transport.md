# Task 23: Implement UDP Transport

## Objective
Implement the concrete `UdpTransport` class inside `connix-core/infrastructure/transport/` adhering to the clean architecture and component detailed design. `UdpTransport` provides connectionless, datagram-oriented network transport over IPv4 and IPv6, delegating low-level socket descriptor management to an injected `ISocket` interface.

## Scope
- `connix/connix-core/infrastructure/transport/inc/ConnixCore/Infrastructure/Transport/UdpTransport.hpp`
- `connix/connix-core/infrastructure/transport/src/UdpTransport.cpp`
- `connix/connix-core/infrastructure/transport/tst/ut/UdpTransportUnitTest.cpp`
- `connix/connix-core/infrastructure/transport/tst/ut/CMakeLists.txt`
- `.ai/tasks/23_implement_udp_transport.md`

## Important Decisions & Assumptions
- **Component-Private Header Placement**: Located `UdpTransport.hpp` under `transport/inc/ConnixCore/Infrastructure/Transport/` following the project's three-tier header architecture (private to `connix-core`, excluded from public distribution package).
- **Datagram & Connectionless Semantics**:
  - `listen()` and `accept()` explicitly throw `TransportException` with `LISTEN_FAILED` and `ACCEPT_FAILED` because stream listening and peer accepting are unsupported on datagram sockets.
  - `connect()` associates a default destination address on the underlying socket, enabling `send()` and `receive()`.
  - `bind()` sets up local endpoint binding for inbound datagram reception. When bound, `receive()` captures the originating sender endpoint via `receiveFrom()` and updates `m_remoteEndpoint`, enabling subsequent `send()` calls via `sendTo()`.
- **Dependency Inversion**: `UdpTransport` interacts exclusively with the `ISocket` interface via constructor injection, enabling mock testing with `MockISocket` without needing real network sockets.
- **Rule of 5 Compliance**: Explicitly declared move constructor and move assignment as defaulted, and deleted copy constructor and copy assignment to enforce move-only semantics for unique socket resources.
- **Virtual Call in Destructor Prevention**: Created a private non-virtual `closeInternal() noexcept` helper invoked by both `~UdpTransport()` and `close()` to avoid virtual method dispatch during destruction.
- **Requirements Traceability**:
  - `SW_REQ_PROTOCOL_SUPPORT_UDP`: Implements datagram send and receive over UDP.
  - `SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`: Closes socket immediately when connection, send, or receive operations time out.
- **Comprehensive Doxygen Documentation**: Documented all class declarations, constructors, and member methods with Doxygen Javadoc-style docstrings.

## Changes Made
- Created `UdpTransport.hpp` under `transport/inc/ConnixCore/Infrastructure/Transport/`.
- Implemented all `UdpTransport` methods in `transport/src/UdpTransport.cpp`.
- Created `UdpTransportUnitTest.cpp` in `transport/tst/ut/` with 24 exhaustive test cases covering constructors, IPv4/IPv6 binding, unsupported listen/accept exceptions, connecting, send (connected and bound with destination), receive (connected and bound with sender extraction), timeout socket closures, and idempotent closure.
- Registered `UdpTransportUnitTest.cpp` in `tst/ut/CMakeLists.txt`.
- Created task tracking log `.ai/tasks/23_implement_udp_transport.md`.

## Validation Performed
- Built native debug target with tests: `./scripts/build_application_native.sh` (clean).
- Ran all unit tests: `./scripts/run_unit_tests.sh` (100% tests passed, 11/11 test suites, 24/24 tests in `UdpTransportUnitTest`).
- Verified line coverage: `./scripts/check_coverage.sh` (100.0% line coverage achieved across all files, 113/113 lines in `UdpTransport.cpp`).
- Ran integration tests: `./scripts/run_integration_tests.sh` (passed).
- Ran static analysis: `./scripts/run_clang_tidy.sh` (0 warnings/errors across all files and tests).
- Checked formatting: `./scripts/run_clang_format.sh` (clean).
- Checked spelling: `./scripts/check_spelling.sh` (210 files checked, 0 errors).
- Built Doxygen docstrings: `./scripts/build_docstring.sh --clean && ./scripts/build_docstring.sh --clean-only` (clean, verified `UdpTransport` docs generated and wiped).
- Built release target and packaging: `./scripts/build_application_release.sh` (clean, verified deployment).
