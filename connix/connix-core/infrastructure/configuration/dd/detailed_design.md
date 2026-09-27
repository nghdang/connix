# Detailed Design: Component Configuration

This document specifies the detailed design for the Configuration module inside
`connix-core/infrastructure/configuration/`.

## 1. Introduction

The configuration module is a foundational component responsible for loading,
validating, and parsing the Connix runtime configuration file based on the
`connix-config.schema.json` schema. It must adhere to **SOLID principles**,
**Clean Code** standards, and correct **Design Patterns**.

---

## 2. Use Cases

The primary use cases supported by the Configuration component are modeled in

```plantuml
!include use_case.puml
```

## 3. Static View

The static relationship of modules and interfaces within the Configuration
component is detailed in

```plantuml
!include static_view.puml
```

## 4. Interface View

Details the core object behavior, interfaces, implementations, and dependency
injections are modeled in

```plantuml
!include interface_view.puml
```

Details the strictly immutable domain configuration models, nested structs,
collections, and enums are modeled in

```plantuml
!include data_structures.puml
```

### 4.1 Action Configuration Model Decisions

`ActionConfig` represents executable actions (`SEND` and `RESPOND`). The design reflects the following rules:

1. **Type Discriminator (`getType()`):**
   Non-optional `ActionType` indicating whether the action is proactive (`SEND`) or reactive (`RESPOND`).

2. **Encapsulated Payload (`getPayload()`):**
   Payloads are encapsulated in a dedicated `ActionPayload` object containing a `PayloadType` (`BYTES` or `FILE`) and the concrete `source` string. This eliminates `std::optional` and mutually exclusive validation at the domain layer, mapping cleanly to a nested JSON object `{"payload": {"type": "...", "source": "..."}}`.

3. **Node Routing Semantics (`getSourceNode()`, `getTargetNode()`):**
   - Both `sourceNode` and `targetNode` are stored as concrete `std::string` values (defaulting to empty string `""`):
     - If `sourceNode` is not empty, it specifies the transmitting node; otherwise, the current active node is used.
     - If `targetNode` is not empty, it specifies the destination node; otherwise, the peer of the active connection is used.
   - For `SEND` actions in configuration, explicit source and target nodes are required.
   - For `RESPOND` actions, source and target are optional in configuration and default to `""`, cleanly reusing the same data structure without `std::optional`.

4. **Execution Controls (`getExecutionDelay()`, `getExecutionTimeout()`, `getMaxPending()`):**
   - Both `SEND` and `RESPOND` actions operate under execution flow controls:
     - `executionDelay`: Delay in milliseconds before transmitting (default: `0`).
     - `executionTimeout`: Timeout in milliseconds bounding the send or response operation (default: `5000`).
     - `maxPending`: Maximum allowable queued or in-flight actions (default: `100`).
   - Stored as concrete `std::uint32_t` values with default parameters.

### 4.2 Structural Separation of Node Models

Node configurations are cleanly separated into three distinct domain classes: `ServerNodeConfig`, `ClientNodeConfig`, and `PeerNodeConfig`. This eliminates loose groupings of optional fields, enforces invariants at compile time, and completely avoids `std::optional` in accordance with repository standards:

1. **`ServerNodeConfig`**:
   - Represents a listening socket server.
   - Requires concrete `NodeTransport`, `Endpoint`, and `FrameConfig`.
   - Defines concrete execution parameters: `maxConnections`, `bufferSize`, and an `onReceived` rule collection.
   - Excludes timeouts which do not apply to server listeners.

2. **`ClientNodeConfig`**:
   - Represents an outbound initiating network client.
   - Requires concrete `NodeTransport` and `FrameConfig`.
   - Defines concrete execution and timeout parameters: `bufferSize`, `connectionTimeout`, `idleTimeout`, and an `onReceived` rule collection.
   - Excludes listening endpoints and `maxConnections`.

3. **`PeerNodeConfig`**:
   - Represents an external target peer endpoint.
   - Requires concrete `NodeTransport`, `Endpoint`, and `FrameConfig`.
   - Excludes server connection limits, buffer sizes, timeouts, and rules.

4. **Elimination of `NodeType` and Optional Frames**:
   - The `NodeType` enum is deleted because the separated mappings within `ConnixConfig` inherently categorize each node.
   - `FrameConfig` now includes `FrameType::NONE` to represent datagram or non-framed connections, removing the need to wrap `FrameConfig` in `std::optional`.

---

## 5. Runtime View

The runtime interactions between Configuration modules during configuration
loading, validation, and retrieval are modeled in

```plantuml
!include runtime_view.puml
```
