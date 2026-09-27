# Task 01: Refactor ActionConfig to Eliminate std::optional and Standardize Schema

## Objective
Refactor `ActionConfig` and `connix-config.schema.json` to completely eliminate the use of `std::optional`. Introduce a new `ActionPayload` class to encapsulate mutually exclusive payload types (`BYTES` vs `FILE`) and standardize routing semantics using empty strings for both `SEND` and `RESPOND` actions. Add the mandate to avoid `std::optional` to `GEMINI.md`.

## Scope
- `connix/connix-core/infrastructure/configuration/res/connix-config.schema.json`
- `docs/connix-config.json`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/PayloadType.hpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/ActionPayload.hpp`
- `connix/connix-core/infrastructure/configuration/src/ActionPayload.cpp`
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/ActionConfig.hpp`
- `connix/connix-core/infrastructure/configuration/src/ActionConfig.cpp`
- `connix/connix-core/infrastructure/configuration/dd/data_structures.puml`
- `connix/connix-core/infrastructure/configuration/dd/detailed_design.md`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationProviderTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/DataStructuresTest.cpp`
- `GEMINI.md`

## Important Decisions & Assumptions
- Added `ActionPayload` with `PayloadType` (`BYTES`, `FILE`) to encapsulate the payload data and remove mutually exclusive optionals.
- `sourceNode` and `targetNode` are stored as concrete `std::string` values defaulting to `""`.
  - Empty `sourceNode` -> active/current node.
  - Empty `targetNode` -> peer node of the active connection.
- `executionDelay` (default: 0), `executionTimeout` (default: 5000), `maxPending` (default: 100) are concrete `std::uint32_t`.
- `ActionConfig` now contains ZERO `std::optional` fields.
- Updated `GEMINI.md` to mandate: "Avoid using `std::optional` as much as possible; prefer concrete default values, empty containers/strings, or dedicated encapsulated types to eliminate caller ambiguity."

## Changes Made
- Added `PayloadType.hpp`, `ActionPayload.hpp`, and `ActionPayload.cpp`.
- Updated `ActionConfig.hpp` and `ActionConfig.cpp` with zero `std::optional` fields.
- Updated `connix-config.schema.json` with `actionPayload` definition and unified `action`.
- Updated `docs/connix-config.json` to use nested `payload` structure for all actions.
- Updated `data_structures.puml` and `detailed_design.md` (Section 4.1).
- Updated `GEMINI.md` with the new rule under Code Style & Formatting.
- Updated `ConfigurationProviderTest.cpp` and `DataStructuresTest.cpp`.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: Clean, 0 format issues.
- `./scripts/build_application_native.sh --force`: Clean native build (GCC 13, Ninja, Debug).
- `./scripts/run_unit_tests.sh`: All 4 test suites passed with **100% line coverage** (219/219 lines).
- `./scripts/build_application_release.sh`: Clean release build and installation.
- `./scripts/run_clang_tidy.sh`: 0 warnings in modified files.
