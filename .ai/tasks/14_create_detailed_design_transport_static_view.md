# Task 14: Create Detailed Design - Static View of Transport Component

## Objective
Create the detailed design documentation and static component diagram (`static_view.puml`) for the `transport` component under `connix-core/infrastructure/transport/dd/`.

## Scope
- `connix/connix-core/infrastructure/transport/dd/static_view.puml`
- `connix/connix-core/infrastructure/transport/dd/detailed_design.md`
- `.ai/tasks/14_create_detailed_design_transport_static_view.md`

## Important Decisions & Assumptions
- Located the component under `connix/connix-core/infrastructure/transport/` with namespace `ConnixCore::Infrastructure::Transport`, matching the Clean Architecture Infrastructure / Frameworks & Drivers ring and neighboring components (`configuration`, `logging`).
- Defined the static view with:
  - `Client`: Higher-level application / connection-cycle consumer.
  - `TransportFactory`: Central factory providing `ITransportFactory` to instantiate concrete transports and support protocol extensibility (`SW_REQ_PROTOCOL_EXTENSIBILITY`).
  - `TcpTransport`: Stream-oriented transport over TCP (`SW_REQ_PROTOCOL_SUPPORT_TCP`).
  - `UdpTransport`: Datagram-oriented transport over UDP (`SW_REQ_PROTOCOL_SUPPORT_UDP`).
  - `UdsTransport`: Local IPC transport over Unix Domain Sockets (`SW_REQ_PROTOCOL_SUPPORT_UDS`), supporting both stream and datagram socket modes.
  - `Socket`: Abstraction layer encapsulating OS socket file descriptors and low-level POSIX socket operations (`ISocket`), decoupling protocol logic from operating-system calls and allowing isolated mock testing.
  - `Operating System (POSIX Sockets)`: Standard Linux socket APIs (`<sys/socket.h>`, `<netinet/in.h>`, `<sys/un.h>`, `<poll.h>`, `<unistd.h>`).
- Enforced Clean Architecture, SOLID principles, and zero `std::optional` conventions.

## Changes Made
- Created `connix/connix-core/infrastructure/transport/dd/static_view.puml` with PlantUML component diagram modeling `Client`, `Transport` internal modules (`TransportFactory`, `TcpTransport`, `UdpTransport`, `UdsTransport`, `Socket`), and `Operating System` (`POSIX Sockets`).
- Created `connix/connix-core/infrastructure/transport/dd/detailed_design.md` documenting introduction, static view, module responsibilities, requirements traceability, and architectural alignment.
- Documented task progress in `.ai/tasks/14_create_detailed_design_transport_static_view.md`.

## Validation Performed
- Validated PlantUML syntax of `static_view.puml` using `java -jar ~/.local/bin/plantuml.jar -syntaxcheck` (clean, 0 errors).
- Rendered `static_view.puml` to SVG and ASCII text via PlantUML pipe to confirm structural layout and visual hierarchy.
- Checked git status to ensure only intended new files are created.
