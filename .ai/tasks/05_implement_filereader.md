# Task 05: Implement FileReader and ConfigurationException

## Objective
Implement the `IFileReader` interface via a concrete `FileReader` class to enable file read operations in the configuration module. Introduce `ConfigurationException` and `ConfigurationErrorCode` to provide domain-specific error reporting rather than throwing bare standard library exceptions.

## Scope
- `connix/connix-core/infrastructure/configuration/api/ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp`
- `connix/connix-core/infrastructure/configuration/api-internal/ConnixCore/Infrastructure/Configuration/FileReader.hpp`
- `connix/connix-core/infrastructure/configuration/src/FileReader.cpp`
- `connix/connix-core/infrastructure/configuration/tst/FileReaderTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationProviderTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/dd/data_structures.puml`

## Important Decisions & Assumptions
- Created `ConfigurationException` inheriting from `std::runtime_error` carrying a typed `ConfigurationErrorCode` enum (`FILE_NOT_FOUND`, `FILE_READ_FAILED`, `SCHEMA_VALIDATION_FAILED`, `JSON_PARSE_FAILED`).
- `FileReader` verifies path existence and checks that it points to a regular file via `std::filesystem`, preventing undefined operations on directories or special device nodes.
- Updated `ConfigurationProviderTest.cpp` to expect `ConfigurationException` for file reading failures.

## Changes Made
- Introduced `ConfigurationException.hpp` in `api/` and updated CMake installation targets accordingly.
- Implemented `FileReader.hpp` in `api-internal/` and `FileReader.cpp` in `src/`.
- Created `FileReaderTest.cpp` covering valid reads, missing files, directory paths, and unreadable files.
- Registered `FileReaderTest.cpp` in `tst/CMakeLists.txt`.
- Documented `ConfigurationException` and `ConfigurationErrorCode` in `data_structures.puml`.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: 0 violations.
- `./scripts/build_application_native.sh --force`: Native debug compilation and link passed cleanly.
- `./scripts/run_unit_tests.sh`: All 5 test suites passed (100% pass rate) with **100% line coverage** (286/286 lines).
- `./scripts/build_application_release.sh`: Succeeded. Verified `ConfigurationException.hpp` was installed under `deploy/include/`.
- `./scripts/run_clang_tidy.sh`: 0 warnings or errors.
