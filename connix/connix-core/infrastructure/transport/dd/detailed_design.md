# Detailed Design: Component Transport

This document specifies the detailed design for the Transport module inside
`connix-core/infrastructure/transport/`.

## 1. Introduction

The transport module is a foundational infrastructure component responsible for
providing unified multi-protocol network communication across TCP, UDP, and
Unix Domain Sockets (UDS). It encapsulates operating-system socket primitives,
enforces connection lifecycle semantics, and adheres to **SOLID principles**,
**Clean Code** standards, and **Clean Architecture**.

The component satisfies:
- **`SW_REQ_PROTOCOL_SUPPORT`**: Support for TCP, UDP, and Unix Domain Socket protocols.
- **`SW_REQ_PROTOCOL_SUPPORT_TCP`**: Stream-oriented communication over TCP.
- **`SW_REQ_PROTOCOL_SUPPORT_UDP`**: Datagram-oriented communication over UDP.
- **`SW_REQ_PROTOCOL_SUPPORT_UDS`**: Stream and datagram communication over Unix Domain Sockets.
- **`SW_REQ_PROTOCOL_EXTENSIBILITY`**: Extensible protocol architecture enabling additional protocols without modifying core orchestration logic.

---

## 2. Static View

The static relationship of modules and interfaces within the Transport
component is detailed in

```plantuml
!include static_view.puml
```

### 2.1 Module Responsibilities

1. **`Client`**:
   - External consumer of the transport component (such as the connection-cycle use case or application orchestration layer in Clean Architecture).
   - Requests transport instances via `ITransportFactory` and performs bounded connection, transmission, reception, and teardown cycles via `ITransport`.

2. **`TransportFactory`**:
   - Implements the Factory pattern (`ITransportFactory`) to instantiate concrete transport implementations (`TcpTransport`, `UdpTransport`, `UdsTransport`) based on configuration (`NodeTransport`).
   - Enables protocol extensibility (`SW_REQ_PROTOCOL_EXTENSIBILITY`) by providing a centralized creation point where new protocol transports can be registered or instantiated without affecting higher-level orchestration logic.
   - Instantiates or injects underlying `Socket` instances into the created transport objects.

3. **`TcpTransport`**:
   - Implements stream-based, connection-oriented transport semantics over TCP (`SW_REQ_PROTOCOL_SUPPORT_TCP`).
   - Implements `ITransport`.
   - Delegates low-level socket descriptor creation, binding, connection, listening, accepting, transmitting, receiving, and closing to `Socket`.

4. **`UdpTransport`**:
   - Implements datagram-based, connectionless transport semantics over UDP (`SW_REQ_PROTOCOL_SUPPORT_UDP`).
   - Implements `ITransport`.
   - Manages packet-oriented transmission and reception using `Socket`.

5. **`UdsTransport`**:
   - Implements local inter-process communication (IPC) over Unix Domain Sockets (`SW_REQ_PROTOCOL_SUPPORT_UDS`).
   - Implements `ITransport`.
   - Supports both stream (`UDS_STREAM`) and datagram (`UDS_DATAGRAM`) modes using `Socket`.

6. **`Socket`**:
   - Encapsulates OS socket file descriptors and low-level POSIX socket operations (`socket()`, `bind()`, `listen()`, `connect()`, `accept()`, `send()`, `recv()`, `poll()`, `close()`).
   - Implements `ISocket` to provide an abstraction barrier (Dependency Inversion Principle) between transport protocol logic and operating-system socket facilities.
   - Allows deterministic unit testing and fault injection via Google Mock (`MockISocket`) without requiring actual OS network resources.

7. **`Operating System (POSIX Sockets)`**:
   - The underlying Linux network subsystem and standard POSIX socket APIs (`<sys/socket.h>`, `<netinet/in.h>`, `<sys/un.h>`, `<poll.h>`, `<unistd.h>`).

### 2.2 Design Principles & Architectural Alignment

- **Clean Architecture:** `Transport` belongs to the Infrastructure / Frameworks & Drivers layer. It implements inward-facing abstractions (`ITransport`, `ITransportFactory`), preventing operating-system networking details from leaking into Domain Entities or Application Use Cases.
- **Open-Closed Principle (OCP):** Additional protocols can be introduced by creating new classes implementing `ITransport` and registering them with `TransportFactory`, leaving core orchestration engines untouched.
- **Single Responsibility Principle (SRP):** Protocol-specific semantics are handled by `TcpTransport`, `UdpTransport`, and `UdsTransport`. Socket file-descriptor lifecycle and system call interactions are encapsulated within `Socket`. Construction and dependency wiring are isolated in `TransportFactory`.
- **Dependency Inversion Principle (DIP):** Callers depend exclusively on abstract interfaces (`ITransport`, `ITransportFactory`). Transports depend on the `ISocket` abstraction, ensuring full testability with Google Test without physical network dependencies.
- **Zero `std::optional` Mandate:** Conforms to the project guideline avoiding `std::optional` by utilizing concrete types, explicit default states, and strongly typed configuration parameters.
