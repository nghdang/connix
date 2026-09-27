# Task 07: Configure Clang-Tidy for Test Files

## Objective
Update `scripts/run_clang_tidy.sh` to analyze unit test files by pointing `BUILD_DIR` to `build-native` (where test compilation commands exist). Address all static analysis findings and guideline warnings that emerged across all test suites, keeping code quality uniform across production and test code. Align the GitHub Actions CI workflow with this change.

## Scope
- `scripts/run_clang_tidy.sh`
- `GEMINI.md`
- `.github/workflows/integration.yml`
- `connix/connix-core/infrastructure/logging/inc/ConnixCore/Infrastructure/Logging/Logging.hpp`
- `connix/connix-core/infrastructure/logging/tst/LoggerTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/FileReaderTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/JsonParserTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationInterfacesTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/ConfigurationProviderTest.cpp`
- `connix/connix-core/infrastructure/configuration/tst/DataStructuresTest.cpp`

## Important Decisions & Assumptions
- Switched `BUILD_DIR` in `scripts/run_clang_tidy.sh` to `build-native`.
- Updated `GEMINI.md` to state that running `clang-tidy` requires a native debug build first (`./scripts/build_application_native.sh`).
- Updated `.github/workflows/integration.yml` to move the `Clang Tidy` step into the `native` job immediately following the `Build` step (reusing the native build compilation database and avoiding duplicate builds in CI), and removed `Clang Tidy` from the `release` job.
- Refactored `LOG_ASSERT` in `Logging.hpp` to use an immediately-invoked lambda `[&]() { ... }()` instead of `do ... while (0)` to comply with `cppcoreguidelines-avoid-do-while`.
- Implemented `void TestBody() override {}` on test fixture base `FileReaderTest` so `readability-identifier-naming` does not misidentify it as an abstract interface requiring an `I` prefix.
- Handled `bugprone-unchecked-optional-access` across test files using safe `.value_or(...)` calls.
- Enforced `const` correctness on local variables across all test suites.

## Changes Made
- `scripts/run_clang_tidy.sh`: Updated `BUILD_DIR` to `build-native` and updated error message to `Build native profile is required`.
- `.github/workflows/integration.yml`: Moved `Clang Tidy` step from `release` job to `native` job.
- `GEMINI.md`: Clarified prerequisite build command for `run_clang_tidy.sh`.
- `Logging.hpp`: Updated `LOG_ASSERT` macro.
- Test files: Cleaned up headers, raw string literals, `const` declarations, and optional access patterns.

## Validation Performed
- `./scripts/run_clang_format.sh --fix`: 0 violations.
- `./scripts/build_application_native.sh --force`: Clean build of all targets.
- `./scripts/run_unit_tests.sh`: All 6 test suites passed with **100% line coverage** (439/439 lines).
- `./scripts/run_clang_tidy.sh`: **0 warnings or errors across all files, including test files.**
