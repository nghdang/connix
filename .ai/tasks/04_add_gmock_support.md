# Task 04: Add Google Mock (gmock) Support

## Objective
Introduce Google Mock (`gmock`) to the `connix-core` configuration module. Provide GMock implementations for both public and internal configuration interfaces, update `GEMINI.md` to establish the `gmock` folder convention, link `GTest::gmock` in CMake, and configure proper installation rules so only public mock interfaces are installed. Ensure `connix-core-mocks` is built unconditionally without `BUILD_MOCKS` so mocks are delivered along with the library for consumers.

## Scope
- `GEMINI.md`
- `CMakeLists.txt`
- `.vscode/settings.json`
- `scripts/build_application_native.sh`
- `connix/connix-core/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/gmock/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/gmock/api/ConnixCore/Infrastructure/Configuration/MockIConfigurationProvider.hpp`
- `connix/connix-core/infrastructure/configuration/gmock/src/MockIConfigurationProvider.cpp`
- `connix/connix-core/infrastructure/configuration/gmock/api-internal/ConnixCore/Infrastructure/Configuration/MockIFileReader.hpp`
- `connix/connix-core/infrastructure/configuration/gmock/api-internal/ConnixCore/Infrastructure/Configuration/MockIJsonValidator.hpp`
- `connix/connix-core/infrastructure/configuration/gmock/api-internal/ConnixCore/Infrastructure/Configuration/MockIJsonParser.hpp`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationInterfacesTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationProviderTest.cpp`

## Important Decisions & Assumptions
- Standardized on the `gmock/` directory naming convention across `GEMINI.md` and CMake.
- Implemented `MockIConfigurationProvider` with default constructor and destructor in the header, and three static factory functions (`create()`, `createNice()`, `createStrict()`) returning `std::shared_ptr` implemented in `MockIConfigurationProvider.cpp`.
- Removed `BUILD_MOCKS` entirely: `connix-core-mocks` is built unconditionally and delivered alongside the core library for consumers in all configurations.
- Configured CMake install rules to install `MockIConfigurationProvider.hpp` alongside the public API headers to `include/`, and `libconnix-core-mocks.so` to `lib/`, while excluding internal mock headers (`MockIFileReader`, `MockIJsonValidator`, `MockIJsonParser`) from installation.
- Excluded `gmock/*` from code coverage (`LCOV_ARG2`) in root `CMakeLists.txt` so test doubles and GMock library internals are not calculated as untested production code.

## Changes Made
- `GEMINI.md`:
  - Updated mock folder documentation from `mock/` to `gmock/`.
  - Removed references to `BUILD_MOCKS`.
- `CMakeLists.txt`:
  - Removed `BUILD_MOCKS` option and conditional blocks.
  - Required `GTest` unconditionally for all native builds.
  - Updated `LCOV_ARG2` to exclude `gmock/*` from lcov coverage reports.
- `scripts/build_application_native.sh` & `.vscode/settings.json`:
  - Removed `-DBUILD_MOCKS=ON`.
- `connix/connix-core/CMakeLists.txt`:
  - Built `connix-core-mocks` unconditionally.
  - Linked `GTest::gmock` to `connix-core-mocks` and test targets.
  - Configured test target include directories to include `gmock/api` and `gmock/api-internal`.
- `connix/connix-core/infrastructure/configuration/CMakeLists.txt`:
  - Added `add_subdirectory(gmock)` unconditionally.
- `connix/connix-core/infrastructure/configuration/gmock/CMakeLists.txt`:
  - Added `src/MockIConfigurationProvider.cpp` and headers to `connix-core-mocks`.
  - Added public installation rule for `api/ConnixCore/` headers only.
- Added GMock classes:
  - `MockIConfigurationProvider.hpp` and `.cpp` with `create()`, `createNice()`, `createStrict()`.
  - `MockIFileReader.hpp`, `MockIJsonValidator.hpp`, `MockIJsonParser.hpp` in `api-internal/`.
- Unit Tests:
  - Refactored `ConfigurationInterfacesTest.cpp` and `ConfigurationProviderTest.cpp` to use the GMock classes, test factory methods, and use `EXPECT_CALL` expectations.

## Validation Performed
- `./scripts/run_clang_format.sh`: 0 violations.
- `./scripts/build_application_native.sh --force`: Clean native debug build and linkage of `connix-core-mocks` with `GTest::gmock`.
- `./scripts/run_unit_tests.sh`: All 4 test suites passed with **100% line coverage** (266/266 lines).
- `./scripts/build_application_release.sh`: Succeeded. Verified both `libconnix-core-mocks.so` and `MockIConfigurationProvider.hpp` are installed into `deploy/` without any internal mock headers.
- `./scripts/run_clang_tidy.sh`: 0 warnings or errors.
