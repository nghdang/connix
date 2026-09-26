# Task 08: Implement JsonValidator

## Objective
Implement the `IJsonValidator` interface using the `json-schema-validator` library (`nlohmann_json_schema_validator`) to validate JSON configuration strings against JSON schema specifications. Integrate `json-schema-validator/2.4.0` into Conan and CMake, configure proper exception mapping to `ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED`, and achieve 100% test coverage with zero warnings.

## Scope
- `conanfile.py`
- `connix/connix-core/CMakeLists.txt`
- `connix/connix-core/infrastructure/configuration/api-internal/ConnixCore/Infrastructure/Configuration/JsonValidator.hpp`
- `connix/connix-core/infrastructure/configuration/src/JsonValidator.cpp`
- `connix/connix-core/infrastructure/configuration/tst/JsonValidatorTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/CMakeLists.txt`

## Important Decisions & Assumptions
- Added `json-schema-validator/2.4.0` dependency to `conanfile.py` and linked `nlohmann_json_schema_validator` to `connix-core`.
- Supported Draft 7 and Draft 2020-12 compatible constructs (`$defs`, `properties`, `required`, type checking, constraints) provided by `json-schema-validator`.
- Wrapped all JSON schema and parsing exceptions into `ConfigurationException` with `ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED`.
- Ensured `misc-include-cleaner` compliance by including `<nlohmann/json_fwd.hpp>` and `<nlohmann/json-schema.hpp>`.
- Preserved full namespace resolution and standard class separation (`*.hpp` declarations, `*.cpp` implementations).

## Changes Made
- Added `json-schema-validator/2.4.0` requirement to `conanfile.py`.
- Added `find_package(nlohmann_json_schema_validator REQUIRED)` and linked `nlohmann_json_schema_validator` to `connix-core` in `connix/connix-core/CMakeLists.txt`.
- Created `JsonValidator.hpp` declaring `JsonValidator` inheriting from `IJsonValidator`.
- Created `JsonValidator.cpp` implementing JSON schema parsing and instance validation.
- Created `JsonValidatorTest.cpp` with 10 comprehensive test cases covering valid schemas, schemas with definitions, syntax errors, semantic schema errors, missing required fields, type mismatches, pattern violations, additional property rejections, and polymorphic usage.
- Registered `JsonValidatorTest.cpp` in `connix/connix-core/infrastructure/configuration/tst/CMakeLists.txt`.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: 0 formatting violations.
- `./scripts/build_application_native.sh --force`: Clean native debug compilation and linking.
- `./scripts/run_unit_tests.sh`: All 7 test suites passed (100% pass rate) with **100% line coverage** (452/452 lines).
- `./scripts/build_application_release.sh`: Clean release build, test exclusion, and deployment packaging.
- `./scripts/run_clang_tidy.sh`: **0 warnings and 0 errors across all codebase and test files.**
