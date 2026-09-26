# Detailed Design: Component Configuration

This document specifies the detailed design for the Configuration module inside
`connix-core/infrastructure/configuration/`.

## 1. Introduction

The configuration module is a foundational infrastructure component responsible for
loading, validating, and parsing the Connix runtime configuration file based on the
`connix-config.schema.json` schema. It provides strongly-typed, immutable configuration
domain models to application orchestration and domain layers, encapsulating file I/O,
JSON schema validation, and token deserialization. It strictly adheres to
**SOLID principles**, **Clean Code** standards, and **Clean Architecture**.

The component satisfies:
- **`SW_REQ_CONFIGURATION_FILE`**: Connix SHALL load configuration from a file.
- **`SW_REQ_CONFIGURATION_JSON_FILE`**: Connix SHALL accept a configuration file in JSON format.
- **`SW_REQ_CONFIGURATION_UNSUPPORTED_FILE`**: Connix SHALL reject a configuration file that is not in a supported format.
- **`SW_REQ_CONFIGURATION_MALFORMED_FILE`**: Connix SHALL reject malformed configuration files and abort without silent fallback.

---

## 2. Use Cases

The primary use cases supported by the Configuration component are modeled in

```plantuml
!include use_case.puml
```

### 2.1 Use Case Specifications

1. **`Load Configuration (UC1)`**:
   - Executed during engine initialization or bootstrap by an external caller (`Client`).
   - Accepts filesystem paths to the configuration file and the JSON Schema definition.
   - Orchestrates reading both files from disk, validating JSON structure and semantics against the schema, parsing configuration objects into domain representations, and caching them in memory.
   - Enforces fail-fast behavior: any missing file, I/O error, schema violation, or syntax error raises a typed `ConfigurationException`, preventing startup with an invalid state (`SW_REQ_CONFIGURATION_MALFORMED_FILE`).

2. **`Get Configuration (UC2)`**:
   - Accessed throughout the runtime lifecycle by engine subsystems (Transport, Rule Engine, Event Loop, Filesystem Watcher).
   - Provides granular, read-only accessors for engine name, node configurations (`ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`), timers (`TimerConfig`), filesystem monitors (`FilesystemConfig`), and executable actions (`ActionConfig`).
   - Returns const references to pre-parsed immutable models, guaranteeing zero-copy overhead and safe concurrent read access post-initialization.

---

## 3. Static View

The static relationship of modules and interfaces within the Configuration
component is detailed in

```plantuml
!include static_view.puml
```

### 3.1 Module Responsibilities

1. **`Client`**:
   - External consumer of the configuration component (such as the application orchestration layer or bootstrap runner).
   - Triggers configuration loading via `IConfigurationProvider::load()` and retrieves domain configuration models for engine initialization.

2. **`ConfigurationProvider`**:
   - Implements `IConfigurationProvider` as the public facade and central coordinator of the configuration subsystem.
   - Depends on `IFileReader`, `IJsonValidator`, and `IJsonParser` via constructor injection (Dependency Injection).
   - Orchestrates the sequential 3-stage loading pipeline: reading configuration and schema text, validating schema compliance, and deserializing domain models.
   - Caches the parsed configuration structures (`ConnixConfig`) and exposes const reference accessors to callers.

3. **`FileReader`**:
   - Implements `IFileReader` to perform operating-system filesystem operations (`<filesystem>`, `<fstream>`, `<sstream>`).
   - Encapsulates file existence checks, regular file validation, and file reading into a single string.
   - Maps operating-system and filesystem errors into `ConfigurationException` with specific error codes (`FILE_NOT_FOUND`, `FILE_READ_FAILED`).

4. **`JsonValidator`**:
   - Implements `IJsonValidator` to validate raw JSON configuration strings against the canonical JSON Schema (`connix-config.schema.json`).
   - Leverages `nlohmann_json_schema_validator` to verify structural constraints, required properties, types, patterns, and limits before parsing.
   - Maps validation failures and syntax errors into `ConfigurationException` with `SCHEMA_VALIDATION_FAILED`.

5. **`JsonParser`**:
   - Implements `IJsonParser` to deserialize validated JSON strings into strongly typed, immutable domain models (`ConnixConfig`).
   - Leverages `nlohmann_json` for DOM traversal, mapping fields into concrete classes (`ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`, `TimerConfig`, `FilesystemConfig`, `ActionConfig`).
   - Maps parsing errors into `ConfigurationException` with `JSON_PARSE_FAILED`.

6. **`nlohmann_json` (Third-Party Dependency)**:
   - High-performance, modern C++ JSON library (`nlohmann/json.hpp`) providing DOM parsing, serialization, and type-safe value extraction.

7. **`nlohmann_json_schema_validator` (Third-Party Dependency)**:
   - Modern C++ JSON Schema validator (`nlohmann/json-schema.hpp`) compliant with JSON Schema Draft 7, used to validate JSON documents against schemas.

### 3.2 Design Principles & Architectural Alignment

- **Clean Architecture:** `Configuration` belongs to the Infrastructure / Frameworks & Drivers layer. It implements inward-facing abstractions (`IConfigurationProvider`, `IFileReader`, `IJsonValidator`, `IJsonParser`), preventing third-party library dependencies (`nlohmann::json`, `nlohmann::json_schema`) and OS filesystem details from leaking into Domain Entities or Application Use Cases.
- **Single Responsibility Principle (SRP):** Each module has a single, well-defined responsibility:
  - `FileReader` is exclusively responsible for filesystem I/O.
  - `JsonValidator` is exclusively responsible for schema compliance verification.
  - `JsonParser` is exclusively responsible for mapping JSON tokens into domain data structures.
  - `ConfigurationProvider` is exclusively responsible for orchestrating the loading pipeline and serving cached configuration data.
- **Open-Closed Principle (OCP):** Additional configuration formats (e.g., YAML as anticipated by `SW_REQ_CONFIGURATION_YAML_FILE`) can be introduced by providing new implementations of `IFileReader`, `IJsonValidator`, or `IJsonParser` without modifying the core `ConfigurationProvider` orchestration logic or higher-level consumers.
- **Dependency Inversion Principle (DIP):** Callers depend exclusively on abstract interfaces (`IConfigurationProvider`). `ConfigurationProvider` depends exclusively on interfaces (`IFileReader`, `IJsonValidator`, `IJsonParser`), enabling isolated unit testing with Google Mock (`MockIFileReader`, `MockIJsonValidator`, `MockIJsonParser`) without reading physical files from disk.
- **Zero `std::optional` Mandate:** Conforms to project standards by eliminating `std::optional` in domain models where concrete defaults (such as empty strings `""`, default integers, or dedicated types like `ActionPayload` and `FrameType::NONE`) eliminate caller ambiguity and ensure deterministic configuration state.

---

## 4. Interface View

Details of core object behaviors, public/internal interfaces, concrete implementations,
and dependency relationships are modeled in

```plantuml
!include interface_view.puml
```

### 4.1 Interface Specifications

1. **`IConfigurationProvider` & `ConfigurationProvider` (Public Interface & Implementation)**:
   - Serves as the primary public contract for configuration management (`SW_REQ_CONFIGURATION_FILE`).
   - **Configuration Lifecycle Management**:
     - `load(const std::string& configPath, const std::string& schemaPath)`: Sequentially reads the configuration file and schema file via `IFileReader`, validates the JSON string against the schema via `IJsonValidator`, parses the document into `ConnixConfig` via `IJsonParser`, and populates internal cached structures. Throws `ConfigurationException` on failure.
   - **Configuration Queries & Accessors**:
     - `getName()`: Returns a const reference to the engine name string (`std::string`).
     - `getServerNodes()`: Returns a const reference to the map of server node configurations (`std::unordered_map<std::string, ServerNodeConfig>`).
     - `getClientNodes()`: Returns a const reference to the map of client node configurations (`std::unordered_map<std::string, ClientNodeConfig>`).
     - `getPeerNodes()`: Returns a const reference to the map of external peer node configurations (`std::unordered_map<std::string, PeerNodeConfig>`).
     - `getTimers()`: Returns a const reference to the map of timer configurations (`std::unordered_map<std::string, TimerConfig>`).
     - `getFilesystems()`: Returns a const reference to the map of filesystem watcher configurations (`std::unordered_map<std::string, FilesystemConfig>`).
     - `getActions()`: Returns a const reference to the map of executable action configurations (`std::unordered_map<std::string, ActionConfig>`).

2. **`IFileReader` & `FileReader` (Internal Interface & Implementation)**:
   - Defines an OS-agnostic abstraction (`IFileReader`) for file reading operations.
   - **File Operations**:
     - `readAll(const std::string& filePath)`: Reads the entire content of the specified file path into a `std::string`. Throws `ConfigurationException` with `FILE_NOT_FOUND` if the path does not exist, or `FILE_READ_FAILED` if the path is not a regular file or cannot be opened.
   - Enables mock injection (`MockIFileReader`) for unit testing without filesystem access.

3. **`IJsonValidator` & `JsonValidator` (Internal Interface & Implementation)**:
   - Defines an abstraction (`IJsonValidator`) for validating JSON strings against JSON schemas.
   - **Validation Operations**:
     - `validate(const std::string& jsonStr, const std::string& schemaStr)`: Parses `schemaStr` and `jsonStr` into JSON DOMs, compiles the schema via `nlohmann::json_schema::json_validator`, and executes validation against `jsonStr`. Throws `ConfigurationException` with `SCHEMA_VALIDATION_FAILED` on syntax or validation errors.
   - Enables mock injection (`MockIJsonValidator`) for unit testing validation error paths.

4. **`IJsonParser` & `JsonParser` (Internal Interface & Implementation)**:
   - Defines an abstraction (`IJsonParser`) for deserializing JSON strings into strongly typed `ConnixConfig` models.
   - **Parsing Operations**:
     - `parse(const std::string& jsonStr)`: Deserializes the JSON document into a concrete `ConnixConfig` container. Maps node objects, timers, filesystems, and actions into their respective domain classes. Re-throws existing `ConfigurationException` instances or catches `std::exception` and wraps them in `ConfigurationException` with `JSON_PARSE_FAILED`.
   - Enables mock injection (`MockIJsonParser`) for unit testing parser orchestration.

5. **`ConnixConfig` (Internal Container Class)**:
   - Immutable data container returned by `IJsonParser::parse()` and consumed by `ConfigurationProvider::load()`.
   - Holds the parsed configuration name and unordered maps of server nodes, client nodes, peer nodes, timers, filesystems, and actions.
   - Provides getter accessors matching `IConfigurationProvider` query methods.

---

## 5. Data Structures

The data structures, enums, exception types, and their relationships are modeled in

```plantuml
!include data_structures.puml
```

### 5.1 Data Structure Specifications

1. **Configuration Enums (Public Enums)**:
   - `FrameType`: Framing strategy for network payloads:
     - `NONE`: No framing; raw stream or datagram payload.
     - `FIXED_SIZE`: Fixed byte-length framing.
     - `LENGTH_PREFIX`: Variable-length framing with length prefix field.
   - `ByteOrder`: Endianness for multi-byte frame headers:
     - `BIG_ENDIAN`: Network byte order (most significant byte first).
     - `LITTLE_ENDIAN`: Least significant byte first.
   - `NodeTransport`: Underlying transport protocol:
     - `TCP`: Stream-oriented TCP transport.
     - `UDP`: Datagram-oriented UDP transport.
     - `UDS_STREAM`: Stream-oriented Unix Domain Socket.
     - `UDS_DATAGRAM`: Datagram-oriented Unix Domain Socket.
   - `ActionType`: Execution mode of configured actions:
     - `SEND`: Proactive transmission of data to a destination peer.
     - `RESPOND`: Reactive response transmission to the active peer in an incoming event cycle.
   - `PayloadType`: Source format of action payloads:
     - `BYTES`: Inline hex or text byte payload.
     - `FILE`: File path reference providing payload content.

2. **`ConfigurationErrorCode` & `ConfigurationException` (Public Types)**:
   - `ConfigurationErrorCode`: Strongly typed enumeration of granular configuration error states:
     - `FILE_NOT_FOUND`: Target configuration or schema file path does not exist on disk.
     - `FILE_READ_FAILED`: File path exists but is not a regular file or cannot be opened.
     - `SCHEMA_VALIDATION_FAILED`: Configuration document violates the JSON schema.
     - `JSON_PARSE_FAILED`: Malformed JSON syntax or invalid data mapping during deserialization.
   - `ConfigurationException`: Standard C++ exception (`std::runtime_error`) holding a concrete `ConfigurationErrorCode` and descriptive message, ensuring deterministic error reporting across CLI and GUI without fallback (`SW_REQ_CONFIGURATION_MALFORMED_FILE`).

3. **`Endpoint` (Public Class)**:
   - Strongly typed network address representation.
   - Members:
     - `m_address`: `std::string` containing IPv4/IPv6 host address or UDS filesystem path.
     - `m_port`: `std::optional<std::uint16_t>` port number (populated for IP sockets, `std::nullopt` for Unix Domain Sockets).
   - Methods:
     - `getAddress()`: Returns const reference to address string.
     - `getPort()`: Returns const reference to optional port.

4. **`FrameConfig` (Public Class)**:
   - Configures message boundary framing parameters.
   - Members:
     - `m_type`: `FrameType` (`NONE`, `FIXED_SIZE`, `LENGTH_PREFIX`).
     - `m_size`: `std::uint32_t` fixed frame size (when `m_type == FIXED_SIZE`).
     - `m_lengthOffset`: `std::uint32_t` byte offset where length prefix begins.
     - `m_lengthSize`: `std::uint32_t` length prefix field size in bytes (e.g., 2 or 4).
     - `m_byteOrder`: `ByteOrder` endianness of length prefix (`BIG_ENDIAN` or `LITTLE_ENDIAN`).
   - Methods:
     - `getType()`: Returns configured frame type.
     - `getSize()`: Returns fixed frame size in bytes.
     - `getLengthOffset()`: Returns byte offset of length field.
     - `getLengthSize()`: Returns length field size in bytes.
     - `getByteOrder()`: Returns byte order enum.

5. **`EventRule` (Public Class)**:
   - Configures pattern-matching triggers and triggered action lists for incoming packet inspection.
   - Members:
     - `m_patterns`: `std::vector<std::string>` list of matching byte patterns or hex strings.
     - `m_offset`: `std::uint32_t` byte offset where pattern matching is evaluated.
     - `m_actions`: `std::vector<std::string>` ordered list of action names to trigger upon match.
   - Methods:
     - `getPatterns()`: Returns const reference to pattern list.
     - `getOffset()`: Returns byte offset integer.
     - `getActions()`: Returns const reference to action name list.

6. **Node Configuration Models (`ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`) (Public Classes)**:
   - Dedicated, structurally separated classes enforcing compile-time invariants per node role:
     - `ServerNodeConfig`:
       - Members: `m_transport` (`NodeTransport`), `m_endpoint` (`Endpoint`), `m_frame` (`FrameConfig`), `m_maxConnections` (`std::uint32_t`), `m_bufferSize` (`std::uint32_t`), `m_onReceived` (`std::vector<EventRule>`).
       - Methods: `getTransport()`, `getEndpoint()`, `getFrame()`, `getMaxConnections()`, `getBufferSize()`, `getOnReceived()`.
     - `ClientNodeConfig`:
       - Members: `m_transport` (`NodeTransport`), `m_frame` (`FrameConfig`), `m_bufferSize` (`std::uint32_t`), `m_connectionTimeout` (`std::uint32_t`), `m_idleTimeout` (`std::uint32_t`), `m_onReceived` (`std::vector<EventRule>`).
       - Methods: `getTransport()`, `getFrame()`, `getBufferSize()`, `getConnectionTimeout()`, `getIdleTimeout()`, `getOnReceived()`.
     - `PeerNodeConfig`:
       - Members: `m_transport` (`NodeTransport`), `m_endpoint` (`Endpoint`), `m_frame` (`FrameConfig`).
       - Methods: `getTransport()`, `getEndpoint()`, `getFrame()`.

7. **`TimerConfig` & `FilesystemConfig` (Public Classes)**:
   - `TimerConfig`:
     - Members: `m_interval` (`std::uint32_t`), `m_singleShot` (`bool`), `m_onTimeout` (`std::vector<std::string>`).
     - Methods: `getInterval()`, `isSingleShot()`, `getOnTimeout()`.
   - `FilesystemConfig`:
     - Members: `m_path` (`std::string`), `m_debounce` (`std::uint32_t`), `m_onModified` (`std::vector<std::string>`).
     - Methods: `getPath()`, `getDebounce()`, `getOnModified()`.

8. **`ActionConfig` & `ActionPayload` (Public Classes)**:
   - `ActionPayload`:
     - Members: `m_type` (`PayloadType`), `m_source` (`std::string`).
     - Methods: `getType()`, `getSource()`.
   - `ActionConfig`:
     - Members: `m_type` (`ActionType`), `m_payload` (`ActionPayload`), `m_sourceNode` (`std::string`), `m_targetNode` (`std::string`), `m_executionDelay` (`std::uint32_t`), `m_executionTimeout` (`std::uint32_t`), `m_maxPending` (`std::uint32_t`).
     - Methods: `getType()`, `getPayload()`, `getSourceNode()`, `getTargetNode()`, `getExecutionDelay()`, `getExecutionTimeout()`, `getMaxPending()`.
     - Constants: `DEFAULT_EXECUTION_DELAY = 0U`, `DEFAULT_EXECUTION_TIMEOUT = 5000U`, `DEFAULT_MAX_PENDING = 100U`.

9. **`ConnixConfig` (Internal Class)**:
   - Aggregates `m_name`, `m_serverNodes`, `m_clientNodes`, `m_peerNodes`, `m_timers`, `m_filesystems`, and `m_actions`.
   - Accessible via const reference getters.

### 5.2 Architectural Design Decisions

1. **Action Configuration Model Decisions**:
   - **Type Discriminator (`getType()`):** Non-optional `ActionType` indicating whether the action is proactive (`SEND`) or reactive (`RESPOND`).
   - **Encapsulated Payload (`getPayload()`):** Payloads are encapsulated in a dedicated `ActionPayload` object containing a `PayloadType` (`BYTES` or `FILE`) and the concrete `source` string. This eliminates `std::optional` and mutually exclusive validation at the domain layer, mapping cleanly to a nested JSON object `{"payload": {"type": "...", "source": "..."}}`.
   - **Node Routing Semantics (`getSourceNode()`, `getTargetNode()`):**
     - Both `sourceNode` and `targetNode` are stored as concrete `std::string` values (defaulting to empty string `""`):
       - If `sourceNode` is not empty, it specifies the transmitting node; otherwise, the current active node is used.
       - If `targetNode` is not empty, it specifies the destination node; otherwise, the peer of the active connection is used.
     - For `SEND` actions in configuration, explicit source and target nodes are required.
     - For `RESPOND` actions, source and target are optional in configuration and default to `""`, cleanly reusing the same data structure without `std::optional`.
   - **Execution Controls (`getExecutionDelay()`, `getExecutionTimeout()`, `getMaxPending()`):**
     - Both `SEND` and `RESPOND` actions operate under execution flow controls:
       - `executionDelay`: Delay in milliseconds before transmitting (default: `0`).
       - `executionTimeout`: Timeout in milliseconds bounding the send or response operation (default: `5000`).
       - `maxPending`: Maximum allowable queued or in-flight actions (default: `100`).
     - Stored as concrete `std::uint32_t` values with default parameters.

2. **Structural Separation of Node Models**:
   - Node configurations are cleanly separated into three distinct domain classes: `ServerNodeConfig`, `ClientNodeConfig`, and `PeerNodeConfig`. This eliminates loose groupings of optional fields, enforces invariants at compile time, and completely avoids `std::optional` in accordance with repository standards:
     - `ServerNodeConfig`: Represents a listening socket server. Requires concrete `NodeTransport`, `Endpoint`, and `FrameConfig`. Defines concrete execution parameters: `maxConnections`, `bufferSize`, and an `onReceived` rule collection. Excludes timeouts which do not apply to server listeners.
     - `ClientNodeConfig`: Represents an outbound initiating network client. Requires concrete `NodeTransport` and `FrameConfig`. Defines concrete execution and timeout parameters: `bufferSize`, `connectionTimeout`, `idleTimeout`, and an `onReceived` rule collection. Excludes listening endpoints and `maxConnections`.
     - `PeerNodeConfig`: Represents an external target peer endpoint. Requires concrete `NodeTransport`, `Endpoint`, and `FrameConfig`. Excludes server connection limits, buffer sizes, timeouts, and rules.
     - **Elimination of `NodeType` and Optional Frames:** The `NodeType` enum is deleted because the separated mappings within `ConnixConfig` inherently categorize each node. `FrameConfig` includes `FrameType::NONE` to represent datagram or non-framed connections, removing the need to wrap `FrameConfig` in `std::optional`.

3. **Zero `std::optional` Mandate Compliance**:
   - Conforms strictly to repository conventions avoiding `std::optional` for domain fields:
     - Using `std::string` defaulting to `""` for routing identifiers instead of `std::optional<std::string>`.
     - Using `FrameType::NONE` in `FrameConfig` instead of `std::optional<FrameConfig>`.
     - Partitioning node configurations structurally into distinct types (`ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`) instead of a single polymorphic or optional-laden class.
     - Using concrete default integers for execution parameters (`DEFAULT_EXECUTION_DELAY = 0U`, `DEFAULT_EXECUTION_TIMEOUT = 5000U`, `DEFAULT_MAX_PENDING = 100U`).

---

## 6. Runtime View

The runtime interactions between Configuration modules during configuration
loading, validation, and retrieval are modeled in

```plantuml
!include runtime_view.puml
```

### 6.1 Runtime Sequence Specifications

1. **Use Case: Load Configuration**:
   - **Invocation**: External consumer (`Client`) invokes `ConfigurationProvider::load(configPath, schemaPath)`.
   - **File Reading**:
     - `ConfigurationProvider` calls `FileReader::readAll(configPath)` to read the configuration JSON string (`jsonStr`).
     - `ConfigurationProvider` calls `FileReader::readAll(schemaPath)` to read the schema JSON string (`schemaStr`).
     - If either file does not exist, `FileReader` throws `ConfigurationException` with `FILE_NOT_FOUND`.
     - If either file cannot be opened or is not a regular file, `FileReader` throws `ConfigurationException` with `FILE_READ_FAILED`.
   - **Schema Validation**:
     - `ConfigurationProvider` delegates validation to `JsonValidator::validate(jsonStr, schemaStr)`.
     - `JsonValidator` parses the schema document, initializes the `nlohmann::json_schema::json_validator`, and validates `jsonStr` against the compiled schema.
     - If the schema is invalid or the document violates schema rules, `JsonValidator` throws `ConfigurationException` with `SCHEMA_VALIDATION_FAILED`.
   - **Parsing & Deserialization**:
     - `ConfigurationProvider` delegates parsing to `JsonParser::parse(jsonStr)`.
     - `JsonParser` walks the JSON DOM and constructs strongly typed domain instances (`ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`, `TimerConfig`, `FilesystemConfig`, `ActionConfig`), returning an aggregate `ConnixConfig`.
     - If parsing fails due to syntax errors or type mismatches, `JsonParser` throws `ConfigurationException` with `JSON_PARSE_FAILED`.
   - **State Persistence**:
     - `ConfigurationProvider` populates its member variables (`m_name`, `m_serverNodes`, `m_clientNodes`, `m_peerNodes`, `m_timers`, `m_filesystems`, `m_actions`) by moving/copying from `ConnixConfig`.
     - Loading completes successfully and execution control returns to `Client`.

2. **Use Case: Get Configuration**:
   - **Access Patterns**:
     - Following a successful `load()`, higher-level orchestration layers and engine components query configuration state using accessor methods:
       - `getName()`: Returns engine identifier.
       - `getServerNodes()`: Returns map of server nodes for socket listener creation.
       - `getClientNodes()`: Returns map of client nodes for outbound connection establishment.
       - `getPeerNodes()`: Returns map of external target endpoints.
       - `getTimers()`: Returns map of timer definitions for periodic rule execution.
       - `getFilesystems()`: Returns map of filesystem paths for directory watching.
       - `getActions()`: Returns map of action payloads and routing configurations for rule triggers.
   - **Immutability & Performance**:
     - Accessors return const references directly to internal cached containers.
     - Zero heap allocations and zero data copying during query execution.
     - Post-initialization access is read-only and thread-safe.
