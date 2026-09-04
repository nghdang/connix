# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Connix** is a programmable, event-driven network interaction engine that unifies multi-protocol communication (HTTP/REST, TCP/UDP, Unix Domain Sockets) in a single C++17 tool. The architecture consists of:

- **connix-core**: Static library with networking, transport, and logging layers
- **connix-cli**: CLI application frontend
- **connix-gui**: Optional Qt/QML GUI frontend

## Build and Development Commands

### Build (Debug with Tests and Mocks)
```bash
./scripts/build_application_native.sh
# Or with a specific profile:
./scripts/build_application_native.sh --host-profile debug/gcc13_x86_64
```

By default, debug builds automatically enable `BUILD_MOCKS=ON` and `BUILD_TESTS=ON`. Use `-f` or `--force` to clean rebuild.

### Run Unit Tests
```bash
./scripts/run_unit_tests.sh
```
Runs all unit tests (prefixed with `UnitTest*`) and generates code coverage reports in `build-native/coverage/`.

### Run a Single Test File
```bash
./scripts/build_application_native.sh  # build first
cd build-native
ctest -R "UnitTest<FileName>" --verbose
# e.g.: ctest -R "UnitTestLoggerTest" --verbose
```

### Code Formatting
```bash
./scripts/run_clang_format.sh
```
Applies LLVM-style formatting per `.clang-format` rules.

### Static Analysis (Linting)
```bash
./scripts/build_application_release.sh  # required before linting
./scripts/run_clang_tidy.sh
```

### Code Coverage Report
Coverage is auto-generated when running tests. View results at `build-native/coverage/index.html` after test runs.

## Architecture and Key Layers

### Communication Stack (Event-Driven Data Flow)
Located in `connix/connix-core/src/Communication/`, organized as a vertical stack:

**Data flow:** `TransportLayer` → `ConnectionLayer` → `NodeLayer`

- **TransportLayer** (`TransportLayer/`): Low-level protocol implementations (TCP, UDP, UDS)
  - `BaseTransport`: Abstract base defining the transport contract
  - Concrete implementations: `TcpTransport`, `UdpTransport`, `UdsTransport`
  - Responsibility: Encoding/decoding over wire protocols
  
- **ConnectionLayer** (`ConnectionLayer/`): Manages connections above transport
  - Wraps transport instances and handles lifecycle (open, close, error states)
  - Buffers incoming/outgoing data; routes by connection ID
  - Responsibility: Connection-level state and I/O buffering
  
- **NodeLayer** (`NodeLayer/`): High-level programmable node abstraction
  - Coordinates communication across multiple transports and connections
  - Triggers events and coordinates async operations
  - Responsibility: Application-facing orchestration and event dispatch

### Logging
Located in `connix/connix-core/src/Logging/`:

- Centralized logging infrastructure
- Logger API exposed via `connix-core` public headers

### Public API
Header files in `connix/connix-core/inc/ConnixCore/` define the public interface. This is the contract consumers depend on.

### Mocks
Mocks library (`connix/connix-core/mock/`) provides test doubles for unit testing layers in isolation. Enabled with `BUILD_MOCKS=ON` (automatic in debug).

## Directory Structure Reference

```
connix/
├── connix-core/              # Core networking library (static)
│   ├── src/                  # Implementation (Communication, Logging layers)
│   ├── inc/ConnixCore/       # Public API headers
│   ├── api/                  # Internal API definitions
│   ├── mock/                 # Test mocks (enabled in debug builds)
│   └── tst/ut/               # Unit tests
├── connix-cli/               # CLI frontend
├── connix-gui/               # Qt/QML GUI frontend
scripts/                       # Build and development scripts
conan_profiles/               # Conan profiles for cross-compilation
```

## Testing

**Test Naming & Discovery:** Tests use the `UnitTest` prefix. CMake defines a helper function `create_unit_test()` in `connix/connix-core/tst/CMakeLists.txt` that generates executables named `UnitTest<FileName>`.

**Test Timeout:** 30 seconds per test (hard limit). Long-running tests will be forcibly terminated. Design tests to fail fast rather than waiting indefinitely; integration tests that spawn network operations should use short timeouts or mocks.

**Test Organization:** Tests mirror the source structure:
- `tst/ut/Communication/TransportLayer/` tests `src/Communication/TransportLayer/`
- `tst/ut/Logging/` tests `src/Logging/`

## Build Configuration

### CMake Options
- `BUILD_MOCKS`: Generate mock libraries (ON in debug, OFF in release/cross-compile)
- `BUILD_TESTS`: Enable unit test targets (ON in debug, OFF otherwise)
- `CMAKE_BUILD_TYPE`: `Debug` or `Release`

### Conan & Dependencies
- **Dependency Manager:** Conan v2
- **Main Dependency:** Google Test (`gtest/1.16.0`) for native builds
- **Profiles:** Located in `conan_profiles/` directory with host/build variants
- **Default Native Profile:** `debug/gcc13_x86_64`

The `conanfile.py` detects if this is a native build (Linux) and conditionally requires gtest. Cross-compilation (sysroot set) disables mocks and tests.

## Code Style and Quality

- **Style Guide:** LLVM (`.clang-format`)
- **Lint Config:** `.clang-tidy` (strict static analysis)
- **C++ Standard:** C++17 (set in CMakeLists.txt)
- **Compiler:** GCC 13 (default native profile)
- **Code Coverage:** lcov + genhtml (tracks branch coverage)

## Common Workflows

### Adding a New Transport
1. Create `src/Communication/TransportLayer/MyTransport.cpp/h`
2. Inherit from `BaseTransport`
3. Add unit test in `tst/ut/Communication/TransportLayer/MyTransportTest.cpp`
4. Update the relevant `CMakeLists.txt` if needed (usually auto-discovered via glob)
5. Update public API in `inc/ConnixCore/` if exposing to consumers

### Adding Unit Tests
1. Create test file in `tst/ut/<MatchingPath>/YourTest.cpp`
2. Include gtest headers and mocks if needed
3. Define test classes/functions (gtest macro-based)
4. Build and run: `./scripts/build_application_native.sh && ./scripts/run_unit_tests.sh`

### Cross-Compilation
Pass a custom profile to the build script:
```bash
./scripts/build_application_native.sh --host-profile cross/arm64 --build-profile debug/gcc13_x86_64
```

## Known Constraints

- Tests must complete within 30 seconds or be killed
- Native debug builds auto-enable mocks and tests; release/cross-compile builds disable them
- Code coverage only generated for native debug builds (used for CI/CD gates)

## Domain-Driven Architecture Evolution

Connix is adopting domain-driven design patterns to improve clarity and extensibility:

- **Schemas** (`connix/connix-core/src/Schema/`): Data structure definitions for domain concepts
- **Entities** (`connix/connix-core/src/Entity/`): Domain-aligned abstractions of core concepts
- **Levels 1 & 2**: Layered domain model reflecting conceptual hierarchy and concerns

When adding new features, consider:
1. Define schemas for new data types first
2. Create entities that compose or extend existing schemas
3. Integrate at the appropriate domain level (1 = foundational, 2 = composite)
4. Update Communication stack integration points as needed

## Performance and Profiling

Use standard Linux tools with the debug build:
```bash
# Example: profile a test with perf
perf record -g ./build-native/UnitTest<Name>
perf report
```

## Git and Version Control

The project uses conventional commits. Check recent commit history to understand the branching strategy and merge process:
```bash
git log --oneline -20
git branch -a
```

Current branch: check `git status` for your working branch.

## IDE and Editor Setup

- **VSCode Config:** `.vscode/settings.json` (project-specific settings)
- **Clang-Format:** Automatically applies LLVM style
- **Clang-Tidy:** Provides IDE integration warnings via compile_commands.json (symlinked at root)

The symlink `compile_commands.json` -> `build-native/compile_commands.json` aids IDE autocomplete and lint integration after a native build.
