# Task 15: Create Detailed Design - Data Structures and Interface View of Transport Component

## Objective
Create the detailed design documentation, data structures diagram (`data_structures.puml`), and interface view diagram (`interface_view.puml`) for the `transport` component under `connix-core/infrastructure/transport/dd/`.

## Scope
- `connix/connix-core/infrastructure/transport/dd/data_structures.puml`
- `connix/connix-core/infrastructure/transport/dd/interface_view.puml`
- `connix/connix-core/infrastructure/transport/dd/detailed_design.md`
- `.ai/tasks/15_create_detailed_design_transport_data_structures_and_interface_view.md`

## Important Decisions & Assumptions
- **Interface Structure (`interface_view.puml`)**:
  - Organized under namespace `ConnixCore::Infrastructure::Transport` into `Public` and `Internal` packages matching the project's Clean Architecture conventions.
  - Defined public `ITransport` providing a common client/server abstraction across TCP, UDP, and Unix Domain Sockets satisfying `SW_REQ_PROTOCOL_EXTENSIBILITY`.
  - Defined public `ITransportFactory` and `TransportFactory` with dynamic protocol registration (`registerTransport`) enabling test mock protocols without changing existing transport or orchestration files.
  - Defined internal `ISocket` and `Socket` encapsulating low-level POSIX socket descriptors and syscalls (`socket`, `bind`, `listen`, `accept`, `connect`, `send`, `recv`, `sendto`, `recvfrom`, `setsockopt`, `close`), decoupling protocol logic from OS APIs and enabling mock injection (`MockISocket`) for unit tests.
  - Modeled concrete transports (`TcpTransport`, `UdpTransport`, `UdsTransport`) realizing `ITransport` and depending on `ISocket`.
- **Data Structures (`data_structures.puml`)**:
  - Defined public enums: `TransportProtocol` (`TCP`, `UDP`, `UDS_STREAM`, `UDS_DATAGRAM`), `TransportState` (`CLOSED`, `BOUND`, `LISTENING`, `CONNECTING`, `CONNECTED`, `DISCONNECTED`), and `TransportErrorCode` covering socket creation, options, binding, listening, connecting, accept, timeouts, and transmission.
  - Defined public `TransportException` inheriting from `std::runtime_error` carrying a strongly typed `TransportErrorCode`.
  - Defined public `TransportEndpoint` encapsulating IP address / socket path and port.
  - **Zero `std::optional` Mandate**: Conforms strictly to workspace conventions by using concrete types (`std::string` address, `std::uint16_t` port defaulting to `0` for UDS/datagram sockets), avoiding `std::optional`.
  - Defined internal socket enums: `SocketDomain`, `SocketType`, `SocketProtocol`, `SocketOption`.

## Changes Made
- Created `connix/connix-core/infrastructure/transport/dd/data_structures.puml` with PlantUML diagram of public and internal transport data structures and relationships.
- Created `connix/connix-core/infrastructure/transport/dd/interface_view.puml` with PlantUML diagram of public and internal interfaces, concrete classes, and dependencies.
- Updated `connix/connix-core/infrastructure/transport/dd/detailed_design.md` adding Section 3 (Interface View) and Section 4 (Data Structures) with comprehensive design rationale, method specifications, and zero `std::optional` adherence.
- Documented task progress in `.ai/tasks/15_create_detailed_design_transport_data_structures_and_interface_view.md`.

## Validation Performed
- Validated PlantUML syntax of `data_structures.puml`, `interface_view.puml`, and `static_view.puml` using `java -jar ~/.local/bin/plantuml.jar -syntaxcheck` (clean, 0 errors).
- Rendered `data_structures.puml` and `interface_view.puml` to SVG and ASCII text via PlantUML pipe to confirm structural correctness, class definitions, and layout clarity.
- Verified `./scripts/run_clang_format.sh` to confirm no source formatting issues.
