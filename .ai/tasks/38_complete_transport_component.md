# Task 38: Complete Transport Component (Socket and TransportFactory)

## Objective
Complete the `transport` component under `connix-core/infrastructure/transport/`
by implementing the concrete POSIX `Socket` (implementing `ISocket`) and
`TransportFactory` (implementing `ITransportFactory`), achieving 100% test and
code coverage across all methods and error paths.

## Scope
- Implement `TransportFactory.hpp` under
  `connix-core/infrastructure/transport/api/public/`
  `ConnixCore/Infrastructure/Transport/`.
- Implement `TransportFactory.cpp` under
  `connix-core/infrastructure/transport/src/`.
- Implement `Socket.hpp` under
  `connix-core/infrastructure/transport/inc/`
  `ConnixCore/Infrastructure/Transport/`.
- Implement `Socket.cpp` under
  `connix-core/infrastructure/transport/src/`.
- Add unit tests for `Socket` (`SocketUnitTest.cpp`) and `TransportFactory`
  (`TransportFactoryUnitTest.cpp`) under
  `connix-core/infrastructure/transport/tst/ut/`.
- Register the new tests in `tst/ut/CMakeLists.txt`.

## Important Decisions & Architectural Patterns
1. **Concrete POSIX Socket Abstraction (`Socket`)**:
   - Implements `ISocket` interface for POSIX stream and datagram sockets
     across `IPv4`, `IPv6`, and `UNIX` domains.
   - Non-blocking I/O with explicit timeout bounding using `poll(2)` for all
     `connect`, `accept`, `send`, `receive`, `sendTo`, and `receiveFrom`
     operations.
   - Safe RAII lifecycle with `closeInternal()` invoked in destructor and move
     assignment to guarantee automatic descriptor closure and UDS socket file
     unlinking.
   - `SocketAddressBuffer` alignment and size matching `sockaddr_storage`
     with `struct sockaddr` first member, avoiding unsafe casts and union
     access.
   - Fault injection mechanism via `Detail::SyscallHooks` function pointer table
     allowing deterministic unit testing of all POSIX error conditions and
     signal interruptions (`EINTR`, `EAGAIN`, `EPIPE`, `ECONNRESET`, timeouts).

2. **Extensible Transport Factory (`TransportFactory`)**:
   - Implements `ITransportFactory` to construct `TcpTransport`, `UdpTransport`,
     and `UdsTransport` (stream and datagram).
   - Injects default concrete `Socket` instances or user-supplied mock sockets
     via optional `SocketCreator` constructor injection.
   - Supports runtime custom protocol registration via `registerTransport()`
     for extensibility (`SW_REQ_PROTOCOL_EXTENSIBILITY`).

## Changes Made
- Added `TransportFactory.hpp` and `TransportFactory.cpp`.
- Added `Socket.hpp` and `Socket.cpp`.
- Added `TransportFactoryUnitTest.cpp` covering default protocol instantiation,
  custom socket creator injection, custom creator registration, move operations,
  and unsupported protocol rejection.
- Added `SocketUnitTest.cpp` covering IPv4/IPv6/UDS stream and datagram
  loopback communications, zero-byte corner cases, timeouts, invalid addresses,
  and fault-injected kernel syscall error scenarios.
- Registered unit tests in `tst/ut/CMakeLists.txt`.

## Validation Performed
- Line length validation using `./scripts/check_line_length.sh` (0 violations).
- Code formatting check and verification using `./scripts/run_clang_format.sh`
  (0 violations).
- Clang-Tidy static analysis using `./scripts/run_clang_tidy.sh`
  (0 warnings / errors, warnings-as-errors enforced).
- Spelling check using `./scripts/check_spelling.sh` (0 issues).
- Unit tests run via `./scripts/run_unit_tests.sh` (20 of 20 test suites passed,
  100% line coverage across the entire `connix-core` library: 1517/1517 lines).
- Coverage threshold verified with `./scripts/check_coverage.sh`
  (100.0% line coverage -> Passed).
- Integration tests verified via `./scripts/run_integration_tests.sh` (Passed).
- Docstring HTML build verified via `./scripts/build_docstring.sh` (Clean).
- Release build verified via `./scripts/build_application_release.sh`.
- Conan package build verified via `./scripts/build_conan_package.sh`.
