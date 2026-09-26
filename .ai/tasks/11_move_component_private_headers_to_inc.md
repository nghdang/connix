# Task 11: Move Component-Private Configuration Headers to inc

## Objective
Establish the project's three-tier header distribution pattern in the `configuration` component:
- `api/public/`: Public API headers included in the delivery package.
- `api/internal/`: Internal API headers referred by other components (and tests) but excluded from delivery packaging.
- `inc/`: Private headers used internally inside the component only.

Move `FileReader.hpp`, `JsonValidator.hpp`, and `JsonParser.hpp` from `api/internal/` to `inc/ConnixCore/Infrastructure/Configuration/`, while preserving `ConnixConfig.hpp`, `IFileReader.hpp`, `IJsonValidator.hpp`, and `IJsonParser.hpp` in `api/internal/`.

## Scope
- `connix/connix-core/infrastructure/configuration/inc/`
- `connix/connix-core/infrastructure/configuration/inc/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/api/internal/`
- `GEMINI.md`
- `CLAUDE.md`

## Important Decisions & Assumptions
- Component-private headers (`FileReader.hpp`, `JsonValidator.hpp`, `JsonParser.hpp`) are moved under `inc/ConnixCore/Infrastructure/Configuration/` to maintain consistent include paths (`#include "ConnixCore/Infrastructure/Configuration/FileReader.hpp"`).
- `ConnixConfig.hpp`, `IFileReader.hpp`, `IJsonValidator.hpp`, and `IJsonParser.hpp` remain in `api/internal/` since they are needed by other components, mocks, and test suites.
- `configuration/CMakeLists.txt` is updated to include `add_subdirectory(inc)`.
- `inc/CMakeLists.txt` is configured to register private headers to `connix-core` and expose the include directory.
- `GEMINI.md` and `CLAUDE.md` conventions are updated to document the three-tier header structure.

## Changes Made
- Moved `FileReader.hpp`, `JsonValidator.hpp`, and `JsonParser.hpp` from `api/internal/ConnixCore/Infrastructure/Configuration/` to `inc/ConnixCore/Infrastructure/Configuration/`.
- Kept `ConnixConfig.hpp`, `IFileReader.hpp`, `IJsonValidator.hpp`, and `IJsonParser.hpp` in `api/internal/ConnixCore/Infrastructure/Configuration/`.
- Configured `connix/connix-core/infrastructure/configuration/inc/CMakeLists.txt` to register private headers and export the include directory.
- Added `add_subdirectory(inc)` to `connix/connix-core/infrastructure/configuration/CMakeLists.txt`.
- Updated `GEMINI.md` and `CLAUDE.md` to document the 3-tier header distribution rules.

## Validation Performed
- `./scripts/build_application_native.sh --force`: Clean native rebuild succeeded with all 37 targets.
- `./scripts/run_unit_tests.sh`: All 7 unit test suites passed (100% pass rate) with 100% line coverage (451/451 lines).
- `./scripts/run_clang_tidy.sh`: 0 errors and 0 warnings.
- `./scripts/build_application_release.sh`: Succeeded and verified only `public` headers are packaged in `deploy/include/`.
