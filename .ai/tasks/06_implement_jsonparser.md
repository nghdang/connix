# Task 06: Implement JsonParser

## Objective
Implement the `IJsonParser` interface using the `nlohmann_json` library to deserialize JSON configuration strings into the strongly-typed `ConnixConfig` domain model. Integrate `nlohmann_json/3.11.3` into Conan and CMake, configure proper exception mapping, and achieve 100% test coverage.

## Scope
- `conanfile.py`
- `CMakeLists.txt`
- `connix/connix-core/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/api-internal/ConnixCore/Infrastructure/Configuration/JsonParser.hpp`
- `connix/connix-core/infrastructure/configuration/src/JsonParser.cpp`
- `connix/connix-core/infrastructure/configuration/tst/JsonParserTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/CMakeLists.txt`

## Important Decisions & Assumptions
- Added `nlohmann_json/3.11.3` to `conanfile.py` and linked `nlohmann_json::nlohmann_json` to `connix-core`.
- Handled parsing and mapping for all top-level sections: `nodes` (`serverNodes`, `clientNodes`, `peerNodes`), `timers`, `filesystems`, and `actions`.
- Wrapped JSON errors into `ConfigurationException` with `ConfigurationErrorCode::JSON_PARSE_FAILED`.
- Excluded `*nlohmann/*` from `LCOV_ARG2` in root `CMakeLists.txt` to keep coverage reports focused strictly on project code.

## Changes Made
- Added `nlohmann_json/3.11.3` dependency to `conanfile.py`.
- Linked `nlohmann_json::nlohmann_json` in `connix/connix-core/CMakeLists.txt`.
- Created `JsonParser.hpp` and `JsonParser.cpp` implementing the JSON deserialization logic.
- Created `JsonParserTest.cpp` covering valid full configurations, invalid JSON, unknown transport, unknown frame types, and malformed rules.
- Registered `JsonParserTest.cpp` in `tst/CMakeLists.txt`.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: 0 violations.
- `./scripts/build_application_native.sh --force`: Clean native debug build.
- `./scripts/run_unit_tests.sh`: All 6 test suites passed (100% pass rate) with **100% line coverage** (438/438 lines).
- `./scripts/build_application_release.sh`: Succeeded.
- `./scripts/run_clang_tidy.sh`: 0 warnings or errors.
