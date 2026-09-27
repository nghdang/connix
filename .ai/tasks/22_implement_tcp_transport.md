# Task 22: Implement TCP Transport

## Objective
Implement the concrete `TcpTransport` class inside `connix-core/infrastructure/transport/` adhering to the clean architecture and component detailed design. `TcpTransport` provides stream-oriented network transport over IPv4 and IPv6, delegating low-level socket descriptor management to an injected `ISocket` interface.

## Scope
- `connix/connix-core/infrastructure/transport/inc/ConnixCore/Infrastructure/Transport/TcpTransport.hpp`
- `connix/connix-core/infrastructure/transport/inc/CMakeLists.txt`
- `connix/connix-core/infrastructure/transport/src/TcpTransport.cpp`
- `connix/connix-core/infrastructure/transport/CMakeLists.txt`
- `connix/connix-core/infrastructure/transport/tst/ut/TcpTransportUnitTest.cpp`
- `connix/connix-core/infrastructure/transport/tst/ut/CMakeLists.txt`
- `.ai/tasks/22_implement_tcp_transport.md`

## Important Decisions & Assumptions
- **Component-Private Header Placement**: Located `TcpTransport.hpp` under `transport/inc/ConnixCore/Infrastructure/Transport/` following the project's three-tier header architecture (private to `connix-core`, excluded from public distribution package).
- **Dependency Inversion**: `TcpTransport` interacts exclusively with the `ISocket` interface via constructor injection, enabling mock testing with `MockISocket` without needing real network ports.
- **Rule of 5 Compliance**: Explicitly declared move constructor and move assignment as defaulted, and deleted copy constructor and copy assignment to enforce move-only semantics for unique socket resources.
- **Virtual Call in Destructor Prevention**: Created a private non-virtual `closeInternal() noexcept` helper invoked by both `~TcpTransport()` and `close()` to avoid virtual method dispatch during destruction.
- **Requirements Traceability**:
  - `SW_REQ_PROTOCOL_SUPPORT_TCP`: Implements stream-based send and receive over TCP.
  - `SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`: Closes socket immediately when connection, send, or receive operations time out.
  - `SW_REQ_CONNECTION_MANAGEMENT_CLIENT` & `SW_REQ_CONNECTION_MANAGEMENT_SERVER`: Supports outbound connection (`connect`) and inbound listener lifecycle (`bind`, `listen`, `accept`).
- **Comprehensive Doxygen Documentation**: Documented all class declarations, constructors, and member methods with Doxygen Javadoc-style docstrings.

## Changes Made
- Created `connix-core/infrastructure/transport/inc/CMakeLists.txt` and registered it in `transport/CMakeLists.txt`.
- Created `TcpTransport.hpp` under `transport/inc/ConnixCore/Infrastructure/Transport/`.
- Implemented all `TcpTransport` methods in `transport/src/TcpTransport.cpp`.
- Created `TcpTransportUnitTest.cpp` in `transport/tst/ut/` with 27 exhaustive test cases covering constructors, IPv4/IPv6 binding, listening, client acceptance, connecting, transmission, reception, timeout handling, error transitions, and idempotent closure.
- Registered `TcpTransportUnitTest.cpp` in `tst/ut/CMakeLists.txt`.
- Created task tracking log `.ai/tasks/22_implement_tcp_transport.md`.

## Validation Performed
- Built native debug target with tests: `./scripts/build_application_native.sh` (clean).
- Ran all unit tests: `./scripts/run_unit_tests.sh` (100% tests passed, 10/10 test suites, 27/27 tests in `TcpTransportUnitTest`).
- Verified line coverage: `./scripts/check_coverage.sh` (100.0% line coverage achieved across all files, 116/116 lines in `TcpTransport.cpp`).
- Ran integration tests: `./scripts/run_integration_tests.sh` (passed).
- Ran static analysis: `./scripts/run_clang_tidy.sh` (0 warnings/errors across entire codebase and tests).
- Checked formatting: `./scripts/run_clang_format.sh` (clean).
- Checked spelling: `./scripts/check_spelling.sh` (207 files checked, 0 errors).
- Built Doxygen docstrings: `./scripts/build_docstring.sh --clean` (clean, verified `TcpTransport` docs generated).
- Built release target and packaging: `./scripts/build_application_release.sh` (clean, verified deployment).
