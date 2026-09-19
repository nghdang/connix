# Detailed Design: Component Configuration

This document specifies the detailed design for the Configuration module inside `connix-core/infrastructure/configuration/`. 

## 1. Introduction

The configuration module is a foundational component responsible for loading, validating, and parsing the Connix runtime configuration file based on the `connix-config.schema.json` schema. It must adhere to **SOLID principles**, **Clean Code** standards, and correct **Design Patterns**.

---

## 2. Architectural Principles (SOLID & Clean Code)

### Single Responsibility Principle (SRP)
Each class has a single, well-defined reason to change:
- **`IFileReader` (and `FileReader`)**: Responsible solely for reading raw contents from the filesystem.
- **`IJsonValidator` (and `JsonValidator`)**: Responsible solely for validating a JSON string against a schema JSON string.
- **`IJsonParser` (and `JsonParser`)**: Responsible solely for transforming validated JSON content into the typed C++ representation (`ConnixConfig`).
- **`IConfigurationProvider` (and `ConfigurationProvider`)**: Responsible for orchestrating the overall lifecycle: calling the file reader, validating with the validator, parsing with the parser, and caching the resulting configuration.

### Open-Closed Principle (OCP)
The system is designed to allow extensions without modifying the orchestrator (`ConfigurationProvider`):
- If the schema or parser library changes, we can implement a new `IJsonParser` (e.g., using `RapidJSON` or `simdjson`) without altering `ConfigurationProvider` or `FileReader`.
- Custom runtime validation rules (beyond schema) can be added as decorators on `IJsonValidator` or additional validations within the orchestration layer.

### Liskov Substitution Principle (LSP)
All interface implementations (`FileReader`, `JsonValidator`, `JsonParser`, `ConfigurationProvider`) can be seamlessly substituted with their GMock counterparts (`MockFileReader`, `MockJsonValidator`, `MockJsonParser`, `MockConfigurationProvider`) during testing without breaking the caller's expectations.

### Interface Segregation Principle (ISP)
Interfaces are small, cohesive, and tightly focused:
- `IFileReader` exposes only file-reading capabilities.
- `IJsonValidator` exposes only schema-validation.
- `IJsonParser` exposes only parsing.
- `IConfigurationProvider` exposes only the loading trigger and read-only access to configuration.

### Dependency Inversion Principle (DIP)
High-level policy components (like `ConfigurationProvider`) do not depend on low-level details (like file system calls or specific JSON libraries). Instead, they depend on abstract interfaces (`IFileReader`, `IJsonValidator`, `IJsonParser`). 

Third-party dependencies (`nlohmann_json` and `nlohmann_json_schema_validator`) are entirely encapsulated inside their respective wrappers (`JsonParser` and `JsonValidator`), preventing library pollution from leaking into the core application logic.

---

## 3. Design Patterns Applied

### 1. Adapter / Wrapper Pattern
We wrap the external libraries `nlohmann::json` and `nlohmann_json_schema_validator` using the `IJsonParser` and `IJsonValidator` interfaces. This decouples our application from third-party APIs. If these external packages are updated with breaking changes or completely replaced, the modifications are restricted solely to the adapter implementations.

### 2. Dependency Injection (DI)
The `ConfigurationProvider` utilizes constructor injection to acquire its dependencies:
```cpp
ConfigurationProvider(
    IFileReader& fileReader,
    IJsonValidator& jsonValidator,
    IJsonParser& jsonParser
);
```
This avoids global singletons, making the module extremely modular and easily testable with Google Mock.

---

## 4. Static View (Component Structure)

The static relationship of modules and interfaces within the Configuration component is detailed in `static_view.puml`. The detailed class contracts are separated cleanly into two UML diagrams:
- **Interface View (`interface_view.puml`)**: Details the core object behavior, interfaces, implementations, and dependency injections.
- **Data Structures (`data_structures.puml`)**: Details the strictly immutable domain configuration models, nested structs, collections, and enums.

Instead of mapping folders directly, the architecture represents the component's logical module structure:

1. **ConfigurationProvider**: Exposes the client-facing `IConfigurationProvider` interface implemented by `ConfigurationProvider`.
2. **FileReader**: Decouples the configuration module from filesystem operations with the `IFileReader` interface and concrete `FileReader` implementation.
3. **JsonValidator**: Wraps JSON validation routines around the `IJsonValidator` interface and `JsonValidator` implementation.
4. **JsonParser**: Provides JSON deserialization capabilities mapped through `IJsonParser` and the concrete `JsonParser`.
5. **Third-Party Libraries**: `nlohmann_json` and `nlohmann_json_schema_validator` are treated as external utility modules wrapped cleanly within our adapters to limit dependency leaks.

---

## 5. Sequence View (Load & Validate Cycle)

The typical load process flows as follows:

1. **Client** calls `IConfigurationProvider::load(configPath, schemaPath)`.
2. **`ConfigurationProvider`** requests raw config content: `IFileReader::readAll(configPath)`.
3. **`ConfigurationProvider`** requests raw schema content: `IFileReader::readAll(schemaPath)`.
4. **`ConfigurationProvider`** invokes validation: `IJsonValidator::validate(configJson, schemaJson)`.
   - If invalid, `IJsonValidator` throws an exception with validation details.
5. **`ConfigurationProvider`** invokes parsing: `IJsonParser::parse(configJson)`.
   - `IJsonParser` converts the JSON keys to strongly-typed nested structures (`ConnixConfig`).
6. **`ConfigurationProvider`** stores the successful `ConnixConfig` inside `activeConfig`.
7. **Client** retrieves configuration via `IConfigurationProvider::getConfig()`.

---

## 6. Data Immutability

To guarantee thread safety and prevent unintended runtime configuration mutations, the parsed configuration data structures (`ConnixConfig` and all nested structs/objects) are designed to be **Strictly Immutable**:

- **Encapsulated Member Variables**: All internal fields within the configuration objects (e.g., `address`, `port`, `onReceived`) are declared `private`.
- **Read-Only Public Interface**: Fields are accessible exclusively through `public const` getter methods (e.g., `getAddress() const`, `getNodes() const`).
- **Parameterized Construction**: Objects can only be constructed and populated at the time of parsing through explicit parameterized constructors. No setter methods are provided.
- **Const References**: The `IConfigurationProvider` returns the root configuration strictly via `const ConnixConfig&`, ensuring callers cannot mutate the runtime system state.
