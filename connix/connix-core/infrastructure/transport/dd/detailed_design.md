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
   - Implements the Factory pattern (`ITransportFactory`) to instantiate concrete transport implementations (`TcpTransport`, `UdpTransport`, `UdsTransport`) based on configuration (`TransportProtocol`).
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

---

## 3. Interface View

Details of core object behaviors, public/internal interfaces, concrete implementations,
and dependency relationships are modeled in

```plantuml
!include interface_view.puml
```

### 3.1 Interface Specifications

1. **`ITransport` (Public Interface)**:
   - Serves as the common client/server transport abstraction (`SW_REQ_PROTOCOL_EXTENSIBILITY`).
   - **Lifecycle Management**:
     - `bind(const TransportEndpoint& endpoint)`: Binds the local endpoint for listening or sending.
     - `listen(uint32_t backlog)`: Places a stream-oriented socket into listening state.
     - `accept(uint32_t timeoutMs)`: Awaits an inbound connection within the configured timeout bound (5 s per `SW_REQ_CONNECTION_MANAGEMENT_CONNECT_AND_ACCEPT_TIMEOUT_BOUNDS`) and returns a new connected `ITransport` instance.
     - `connect(const TransportEndpoint& endpoint, uint32_t timeoutMs)`: Establishes an outbound connection to a target endpoint bounded by timeout (5 s per `SW_REQ_CONNECTION_MANAGEMENT_CLIENT_CONNECT_TIMEOUT`).
     - `close()`: Closes the transport and releases underlying socket descriptors immediately (`SW_REQ_CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE`).
   - **I/O Operations**:
     - `send(const vector<uint8_t>& data, uint32_t timeoutMs)`: Transmits data over the established connection bounded by send timeout (5 s per `SW_REQ_CONNECTION_MANAGEMENT_SEND_TIMEOUT_BOUND`).
     - `receive(size_t maxBytes, uint32_t timeoutMs)`: Receives data from the connection bounded by receive timeout (10 s per `SW_REQ_CONNECTION_MANAGEMENT_RECEIVE_TIMEOUT_BOUND`).
   - **State Inspection**:
     - `isOpen()`: Returns whether the transport is currently open.
     - `getState()`: Returns the current `TransportState` (`CLOSED`, `BOUND`, `LISTENING`, `CONNECTING`, `CONNECTED`, `DISCONNECTED`).
     - `getProtocol()`: Returns the active `TransportProtocol`.
     - `getLocalEndpoint()` & `getRemoteEndpoint()`: Return concrete `TransportEndpoint` instances.

2. **`ITransportFactory` & `TransportFactory` (Public Interface & Implementation)**:
   - Provides factory creation of transports via `createTransport(TransportProtocol protocol)`.
   - Provides a dynamic extension point `registerTransport(TransportProtocol protocol, TransportCreator creator)` allowing test mocks or future custom protocols to be registered without editing core orchestration or existing transport files (`SW_REQ_PROTOCOL_EXTENSIBILITY`).

3. **`ISocket` & `Socket` (Internal Interface & Implementation)**:
   - Defines an OS-agnostic abstraction (`ISocket`) for POSIX socket file-descriptor operations.
   - Encapsulates socket lifecycle (`open`, `bind`, `listen`, `accept`, `connect`, `send`, `receive`, `sendTo`, `receiveFrom`, `setOption`, `close`).
   - Isolates POSIX header dependencies (`<sys/socket.h>`, `<sys/un.h>`, `<netinet/in.h>`, `<poll.h>`, `<unistd.h>`) into `Socket.cpp`.
   - Enables mock injection (`MockISocket`) for unit testing without touching physical network hardware or requiring external network ports.

4. **Concrete Transports (`TcpTransport`, `UdpTransport`, `UdsTransport`)**:
   - `TcpTransport`: Encapsulates TCP stream client/server semantics using `SOCK_STREAM` over `AF_INET`/`AF_INET6`.
   - `UdpTransport`: Encapsulates UDP datagram client/server semantics using `SOCK_DGRAM` over `AF_INET`/`AF_INET6`.
   - `UdsTransport`: Encapsulates Unix Domain Socket communication over filesystem socket paths, supporting stream (`SOCK_STREAM`) and datagram (`SOCK_DGRAM`) modes via `AF_UNIX`.

---

## 4. Data Structures

The data structures, enums, exception types, and their relationships are modeled in

```plantuml
!include data_structures.puml
```

### 4.1 Data Structure Specifications

1. **`TransportProtocol` (Public Enum)**:
   - Strongly typed enum representing supported transport protocols:
     - `TCP`: Stream-based transmission control protocol.
     - `UDP`: Datagram-based user datagram protocol.
     - `UDS_STREAM`: Connection-oriented Unix Domain Socket stream.
     - `UDS_DATAGRAM`: Connectionless Unix Domain Socket datagram.

2. **`TransportState` (Public Enum)**:
   - Represents the lifecycle state of a transport instance:
     - `CLOSED`: Transport is not open.
     - `BOUND`: Socket is bound to a local address.
     - `LISTENING`: Stream socket is actively accepting inbound connections.
     - `CONNECTING`: Outbound connection is being established.
     - `CONNECTED`: Active connected state, ready for bidirectional I/O.
     - `DISCONNECTED`: Peer disconnected or connection closed.

3. **`TransportErrorCode` & `TransportException` (Public Types)**:
   - `TransportErrorCode`: Strongly typed enumeration of granular error states:
     - `SOCKET_CREATION_FAILED`: Failed to create socket descriptor.
     - `SOCKET_OPTION_FAILED`: Failed to set socket option (reuse address, timeouts, non-blocking).
     - `BIND_FAILED`: Failed to bind to local address/port or socket path.
     - `LISTEN_FAILED`: Failed to place socket into listen mode.
     - `ACCEPT_FAILED`: Failed to accept incoming connection.
     - `CONNECT_FAILED`: Failed to establish connection to peer.
     - `SEND_FAILED`: Failure occurred during data transmission.
     - `RECEIVE_FAILED`: Failure occurred during data reception.
     - `OPERATION_TIMEOUT`: Socket operation exceeded its timeout bound.
     - `CONNECTION_CLOSED`: Connection was closed by remote peer.
     - `INVALID_ADDRESS`: Malformed IP address or invalid socket path.
     - `UNSUPPORTED_PROTOCOL`: Requested protocol is not registered or supported.
   - `TransportException`: Standard C++ exception (`std::runtime_error`) holding a concrete `TransportErrorCode` and descriptive message, ensuring deterministic error reporting across CLI and GUI.

4. **`TransportEndpoint` (Public Class)**:
   - Concrete, strongly-typed endpoint representing an IP host/port or UDS socket path.
   - Members:
     - `m_address`: `std::string` containing IPv4/IPv6 address or UDS filesystem path.
     - `m_port`: `std::uint16_t` port number (defaults to `0` for UDS or datagram sockets without explicit port).
   - Methods:
     - `getAddress()`: Returns the address string.
     - `getPort()`: Returns the port number.
     - `isUnixDomain()`: Convenience check returning `true` if port is 0 or address is a filesystem path.
     - `toString()`: Formatted endpoint string (`address:port` or `unix:path`).
   - **Zero `std::optional` Mandate**: In accordance with workspace conventions, `m_port` uses `0` as an explicit, concrete default value rather than wrapping in `std::optional<uint16_t>`.

5. **Internal Socket Models (Internal Enums)**:
   - `SocketDomain`: `IPV4` (`AF_INET`), `IPV6` (`AF_INET6`), `UNIX` (`AF_UNIX`).
   - `SocketType`: `STREAM` (`SOCK_STREAM`), `DATAGRAM` (`SOCK_DGRAM`).
   - `SocketProtocol`: `DEFAULT` (`0`), `TCP` (`IPPROTO_TCP`), `UDP` (`IPPROTO_UDP`).
   - `SocketOption`: `REUSE_ADDRESS`, `NON_BLOCKING`, `RECEIVE_TIMEOUT`, `SEND_TIMEOUT`.
