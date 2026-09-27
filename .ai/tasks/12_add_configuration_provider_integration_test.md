# Task 12: Add Integration Test for ConfigurationProvider

## Objective
Split the configuration component test suite into unit tests (`tst/ut/`) and integration tests (`tst/it/`). Implement comprehensive integration tests for `ConfigurationProvider` verifying end-to-end behavior using real concrete components (`FileReader`, `JsonValidator`, `JsonParser`) against actual configuration files and schemas.

## Scope
- `connix/connix-core/infrastructure/configuration/tst/ut/`
- `connix/connix-core/infrastructure/configuration/tst/it/`
- `connix/connix-core/infrastructure/configuration/tst/CMakeLists.txt`
- `connix/connix-core/infrastructure/logging/tst/`
- `connix/connix-core/CMakeLists.txt`
- `scripts/run_integration_tests.sh`
- `.github/workflows/integration.yml`
- `GEMINI.md`
- `CLAUDE.md`

## Important Decisions & Assumptions
- Existing tests in `configuration/tst/` (`ConfigurationInterfacesTest.cpp`, `DataStructuresTest.cpp`, `ConfigurationProviderTest.cpp`, `FileReaderTest.cpp`, `JsonParserTest.cpp`, `JsonValidatorTest.cpp`) were moved to `tst/ut/` and renamed to `<Name>UnitTest.cpp`.
- `logging/tst/LoggerTest.cpp` was renamed to `LoggerUnitTest.cpp` to unify the `<Name>UnitTest` naming convention across the codebase.
- `tst/CMakeLists.txt` is updated to delegate to `add_subdirectory(ut)` and `add_subdirectory(it)`.
- `connix/connix-core/CMakeLists.txt` was updated with `create_integration_test()`, supporting nested include directories (`../..`), `CONNIX_PROJECT_ROOT`, and distinct targets:
  - `connix-core-unit-tests-run` targeting regex `"UnitTest"`.
  - `connix-core-integration-tests-run` targeting regex `"IntegrationTest"`.
- Created `scripts/run_integration_tests.sh` and integrated it into `.github/workflows/integration.yml`.
- Implemented `tst/it/ConfigurationProviderIntegrationTest.cpp` testing real collaboration between `ConfigurationProvider`, `FileReader`, `JsonValidator`, and `JsonParser`:
  - Loading real `docs/connix-config.json` against `res/connix-config.schema.json`.
  - Validating all server nodes, client nodes, peer nodes, timers, filesystems, and actions.
  - Polymorphic usage via `IConfigurationProvider`.
  - Non-existent config and schema error handling (`FILE_NOT_FOUND`).
  - Malformed JSON error handling (`SCHEMA_VALIDATION_FAILED`).
  - Schema mismatch error handling (`SCHEMA_VALIDATION_FAILED`).
  - State replacement upon configuration reload.

## Changes Made
- Reorganized `connix/connix-core/infrastructure/configuration/tst/` into `ut/` and `it/`.
- Created `connix/connix-core/infrastructure/configuration/tst/ut/CMakeLists.txt` and `connix/connix-core/infrastructure/configuration/tst/it/CMakeLists.txt`.
- Created `connix/connix-core/infrastructure/configuration/tst/it/ConfigurationProviderIntegrationTest.cpp`.
- Renamed `connix/connix-core/infrastructure/logging/tst/LoggerTest.cpp` to `LoggerUnitTest.cpp` and updated its CMake target.
- Added `scripts/run_integration_tests.sh`.
- Added Integration Tests step to `.github/workflows/integration.yml`.
- Updated `GEMINI.md` and `CLAUDE.md`.

## Validation Performed
- `./scripts/build_application_native.sh`: Compiled all 39 targets cleanly.
- `./scripts/run_unit_tests.sh`: All 7 unit test suites passed (100% pass rate) with 100% line coverage (451/451 lines).
- `./scripts/run_integration_tests.sh`: All 7 integration test scenarios passed (100% pass rate).
- `./scripts/check_coverage.sh`: Line coverage 100.0% -> Passed.
- `./scripts/run_clang_tidy.sh`: 0 errors and 0 warnings.
- `./scripts/run_clang_format.sh`: All files formatted cleanly.
- `./scripts/build_application_release.sh`: Succeeded and packaged release artifacts to `deploy/`.
