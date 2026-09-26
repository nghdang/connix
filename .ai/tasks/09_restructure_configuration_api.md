# Task 09: Restructure Configuration API to Public and Internal

## Objective
Merge `connix/connix-core/infrastructure/configuration/api` and `connix/connix-core/infrastructure/configuration/api-internal` into a unified `api/` directory with `public/` and `internal/` subdirectories. Align `connix/connix-core/infrastructure/configuration/gmock` by renaming `gmock/api` to `gmock/public` and `gmock/api-internal` to `gmock/internal`.

## Scope
- `connix/connix-core/infrastructure/configuration/api/public/`
- `connix/connix-core/infrastructure/configuration/api/internal/`
- `connix/connix-core/infrastructure/configuration/api/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/gmock/public/`
- `connix/connix-core/infrastructure/configuration/gmock/internal/`
- `connix/connix-core/infrastructure/configuration/gmock/CMakeLists.txt`
- `connix/connix-core/CMakeLists.txt`
- `GEMINI.md`

## Important Decisions & Assumptions
- Retained the `ConnixCore/Infrastructure/Configuration/` path under `public/` and `internal/` to keep all existing `#include` directives valid and preserve the namespace-to-path hierarchy.
- Updated `connix-core/infrastructure/configuration/api/CMakeLists.txt` to include `${CMAKE_CURRENT_SOURCE_DIR}/public` and `${CMAKE_CURRENT_SOURCE_DIR}/internal` as PUBLIC include directories, and only install public headers.
- Updated `gmock/CMakeLists.txt` to consume headers from `public` and `internal`.
- Updated `create_unit_test` include directories in `connix/connix-core/CMakeLists.txt` to point to `public` and `internal` subdirectories.
- Updated `GEMINI.md` guidelines for public and internal header paths.

## Changes Made
- Moved all headers from `api/ConnixCore` to `api/public/ConnixCore`.
- Moved all headers from `api-internal/ConnixCore` to `api/internal/ConnixCore`.
- Removed `api-internal/` and its `CMakeLists.txt`.
- Renamed `gmock/api` to `gmock/public` and `gmock/api-internal` to `gmock/internal`.
- Updated `api/CMakeLists.txt`, `configuration/CMakeLists.txt`, `gmock/CMakeLists.txt`, and `connix-core/CMakeLists.txt`.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: 0 violations.
- `./scripts/build_application_native.sh --force`: Clean build with all targets regenerated.
- `./scripts/run_unit_tests.sh`: All 7 unit test suites passed (100% pass rate) with 100% line coverage (451/451 lines).
- `./scripts/run_clang_tidy.sh`: 0 errors and 0 warnings.
- `./scripts/build_application_release.sh`: Succeeded and packaged artifacts.
