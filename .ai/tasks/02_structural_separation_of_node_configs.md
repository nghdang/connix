# Task 02: Structural Separation of Node Configurations

## Objective
Split the generic `NodeConfig` into three distinct, strongly-typed domain models (`ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`) and separate them into distinct maps within `ConnixConfig`. Completely eliminate `std::optional` across node and frame configurations by removing inapplicable fields, providing concrete default values, introducing `FrameType::NONE` for unframed/datagram connections, and nesting the maps under `nodes` in JSON schemas and documents. Delete the now redundant `NodeType` enum.

## Scope
- `connix/connix-core/infrastructure/configuration/res/connix-config.schema.json`
- `docs/connix-config.json`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/NodeType.hpp` (Deleted)
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/NodeConfig.hpp` (Deleted)
- `connix/connix-core/infrastructure/configuration/src/NodeConfig.cpp` (Deleted)
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/FrameType.hpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/FrameConfig.hpp`
- `connix/connix-core/infrastructure/configuration/src/FrameConfig.cpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp`
- `connix/connix-core/infrastructure/configuration/src/ServerNodeConfig.cpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp`
- `connix/connix-core/infrastructure/configuration/src/ClientNodeConfig.cpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp`
- `connix/connix-core/infrastructure/configuration/src/PeerNodeConfig.cpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp`
- `connix/connix-core/infrastructure/configuration/api-internal/ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/ConfigurationProvider.hpp`
- `connix/connix-core/infrastructure/configuration/src/ConfigurationProvider.cpp`
- `connix/connix-core/infrastructure/configuration/src/ConnixConfig.cpp`
- `connix/connix-core/infrastructure/configuration/dd/data_structures.puml`
- `connix/connix-core/infrastructure/configuration/dd/interface_view.puml`
- `connix/connix-core/infrastructure/configuration/dd/detailed_design.md`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationProviderTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/DataStructuresTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationInterfacesTest.cpp`

## Important Decisions & Assumptions
- Nest `serverNodes`, `clientNodes`, and `peerNodes` sub-objects inside the top-level `nodes` map in both JSON and JSON Schema.
- Renamed role `"USER"` to `"PEER"` and categorized them under `peerNodes`.
- Completely removed `NodeType` enum and eliminated `std::optional` in accordance with repository architectural standards:
  - If a configuration is not applicable to a node type, it is completely removed (e.g. no timeouts on servers, no endpoints or maxConnections on clients, no rules/timeouts/buffers on peers).
  - All applicable fields use concrete types (integers, strings, objects).
  - `FrameConfig` is always required and concrete across all nodes; added `FrameType::NONE` (`"type": "NONE"`) to represent un-framed or datagram connections.

## Changes Made
- `connix-config.schema.json`:
  - Added `noneFrame` and integrated it into the `frame` definition.
  - Defined strict `serverNode`, `clientNode`, and `peerNode` schemas with `additionalProperties: false`.
  - Nested `namedServerNodes`, `namedClientNodes`, and `namedPeerNodes` under the `nodes` object definition.
- `docs/connix-config.json`:
  - Restructured `nodes` into `serverNodes`, `clientNodes`, and `peerNodes`.
  - Removed obsolete `"type"` discriminator strings.
  - Added `"frame": {"type": "NONE"}` to datagram nodes that previously omitted frames.
- Deleted obsolete files:
  - `NodeType.hpp`, `NodeConfig.hpp`, and `NodeConfig.cpp`.
- Updated `FrameType` and `FrameConfig`:
  - Added `NONE` to `FrameType`.
  - Refactored `FrameConfig` to eliminate all `std::optional` fields, providing a default constructor and concrete member fields.
- Added domain models:
  - `ServerNodeConfig.hpp` / `ServerNodeConfig.cpp`
  - `ClientNodeConfig.hpp` / `ClientNodeConfig.cpp`
  - `PeerNodeConfig.hpp` / `PeerNodeConfig.cpp`
- Updated core configuration APIs:
  - `IConfigurationProvider.hpp`: replaced `getNodes()` with `getServerNodes()`, `getClientNodes()`, and `getPeerNodes()`.
  - `ConfigurationProvider.hpp` / `ConfigurationProvider.cpp`: updated members and getters for separated node mappings.
  - `ConnixConfig.hpp` / `ConnixConfig.cpp`: updated constructors and getters for separated node mappings.
- Updated documentation and UML diagrams:
  - `data_structures.puml`: updated classes and relationships for `ServerNodeConfig`, `ClientNodeConfig`, `PeerNodeConfig`, removing `NodeType` and optional wrappers.
  - `interface_view.puml`: updated `IConfigurationProvider` and `ConfigurationProvider`.
  - `detailed_design.md`: added section 4.2 detailing the structural separation of node models and elimination of optionals.
- Unit Tests:
  - Updated `ConfigurationInterfacesTest.cpp`, `ConfigurationProviderTest.cpp`, and `DataStructuresTest.cpp` to cover new models, methods, and default states.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: 0 formatting violations.
- `./scripts/build_application_native.sh --force`: Clean native debug build with mocks and tests enabled.
- `./scripts/run_unit_tests.sh`: 100% test pass rate (4/4 test suites, 29/29 tests) with **100% line coverage** (266/266 lines).
- `./scripts/build_application_release.sh`: Clean release build and deployment.
- `./scripts/run_clang_tidy.sh`: 0 warnings in modified files.
