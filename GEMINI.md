# Connix - Project Instructions & Development Guidelines

This file provides foundational mandates, technical instructions, architectural patterns, and development workflows for the **Connix** project. It is intended to guide LLM assistants and developers to ensure consistent, idiomatic code contributions and compliant practices.

---

## 1. Project Overview

**Connix** is a programmable, event-driven network interaction engine designed to unify multi-protocol communication (including HTTP/REST, TCP/UDP, and Unix Domain Sockets) in a single high-performance tool. 

### Key Technologies
*   **Programming Language:** C++17 (standardized in the build configuration, though ready for C++20 features)
*   **Compiler:** GCC 13 (default native profile)
*   **Build System:** CMake (minimum version 3.22) with Ninja generator
*   **Dependency/Package Management:** Conan v2
*   **Unit Testing Framework:** Google Test (`gtest/1.16.0` for native builds)
*   **Static Analysis & Linting:** `clang-tidy` (strict custom rules)
*   **Code Formatting:** `clang-format` (LLVM-based style)
*   **Code Coverage:** `lcov` + `genhtml`

### Sub-Module Architecture
The codebase is structured into three main components under the `connix/` directory:
1.  **connix-core** (`connix/connix-core/`): The core library implementing low-level networking, transport, connection handling, logging, schemas, and entities. It builds as a static library.
2.  **connix-cli** (`connix/connix-cli/`): The command-line interface application wrapper.
3.  **connix-gui** (`connix/connix-gui/`): The graphical user interface wrapper using Qt/QML (optional).

### Architecture & Layer Hierarchy
The core network communications are organized as a vertical, event-driven stack:
$$\text{TransportLayer} \longrightarrow \text{ConnectionLayer} \longrightarrow \text{NodeLayer}$$

*   **TransportLayer (`connix/connix-core/src/Communication/TransportLayer/`):**
    *   Defines the contract via the `BaseTransport` abstract base.
    *   Implements concrete, low-level transport mechanisms: `TcpTransport`, `UdpTransport`, and `UdsTransport`.
    *   Responsible for physical encoding and decoding over wire protocols.
*   **ConnectionLayer (`connix/connix-core/src/Communication/ConnectionLayer/`):**
    *   Wraps transport instances and manages connection lifecycles (opening, closing, transitioning states).
    *   Manages inbound/outbound buffering and routes data via connection IDs (`ConnectionId`).
*   **NodeLayer (`connix/connix-core/src/Communication/NodeLayer/`):**
    *   Provides a high-level programmable node interface to the application.
    *   Orchestrates async execution and routes events across multiple transports/connections.
*   **Domain-Driven Evolution Layers:**
    *   **Schemas (`connix/connix-core/src/Schema/`):** Holds plain data structures representing domain concepts.
    *   **Entities (`connix/connix-core/src/Entity/`):** Domain-aligned components encapsulating core business behavior.
    *   **Level 1 & 2 Concerns:** Foundational domain concerns (Level 1) vs. composite domain concerns (Level 2).

---

## 2. Building, Running, and Testing

The repository provides automated bash utility scripts in the `scripts/` directory to manage environment setup, building, testing, formatting, and linting.

### Build Commands

*   **Native Debug Build (with Tests and Mocks):**
    ```bash
    ./scripts/build_application_native.sh
    ```
    *   Automatically runs CMake with debug variables and triggers building of all components.
    *   Configures `BUILD_MOCKS=ON` and `BUILD_TESTS=ON`.
    *   To force a clean rebuild, use the `-f` or `--force` flag:
        ```bash
        ./scripts/build_application_native.sh --force
        ```
    *   To specify custom Conan host/build profiles:
        ```bash
        ./scripts/build_application_native.sh --host-profile <profile> --build-profile <profile>
        ```

*   **Release Build (Packaging & Deploying):**
    ```bash
    ./scripts/build_application_release.sh
    ```
    *   Builds with optimization and disables testing/mocks (`BUILD_MOCKS=OFF` and `BUILD_TESTS=OFF`).
    *   Installs the compiled binary artifacts to `build-release/deploy` (or a custom prefix folder).

### Running Unit Tests

*   **Run All Tests (and generate coverage report):**
    ```bash
    ./scripts/run_unit_tests.sh
    ```
    *   Requires a native debug build to be completed first.
    *   Runs all tests with the prefix `UnitTest` via CTest.
    *   Generates coverage reports at `build-native/coverage/index.html`.

*   **Run a Single Unit Test Target:**
    ```bash
    cd build-native
    ctest -R "UnitTest<ClassName>" --verbose
    # Example: ctest -R "UnitTestLoggerTest" --verbose
    ```

---

## 3. Development & Contribution Conventions

### Code Style & Formatting
*   **Formatting Rules:** Enforced by `clang-format` based on the `.clang-format` configuration file (inheriting LLVM styles).
*   **Format Verification & Inline Correction:**
    *   To check formatting:
        ```bash
        ./scripts/run_clang_format.sh
        ```
    *   To apply changes inline:
        ```bash
        ./scripts/run_clang_format.sh --fix
        ```
*   **Linter Checks:** Strict static analysis via `clang-tidy` based on `.clang-tidy` settings.
    ```bash
    ./scripts/run_clang_tidy.sh
    ```
    *Note: Running `clang-tidy` requires compiling a release build (`./scripts/build_application_release.sh`) first to generate the compilation database.*

### Naming & Structure Conventions
*   **File Names:** Consistent PascalCase (e.g. `ConnixClient.hpp`, `ConnixClient.cpp`).
*   **Namespaces:** Match the folder structure under `ConnixCore::` (e.g., `ConnixCore::Common::Logging`, `ConnixCore::Infrastructure::NetworkService`).
*   **Public API Headers:** Exposed in `connix/connix-core/inc/ConnixCore/`.
*   **Internal API Headers:** Exposed in `api-internal` subdirectories (e.g., `connix/connix-core/common/logging/api-internal/`).

### Unit Testing Practices
*   **Directory Mapping:** Tests are located in corresponding `tst` subfolders, mirroring the source structure (e.g. `connix-core/common/logging/tst/LoggerTest.cpp` tests `connix-core/common/logging/src/Logger.cpp`).
*   **Test Executable Creation:** CMake includes a custom parser function `create_unit_test()` defined in `cmake/helpers.cmake`. Use the specific wrappers to declare your tests:
    *   `create_unit_test_connix_core(YourTestFile.cpp)`
    *   `create_unit_test_connix_cli(YourTestFile.cpp)`
    *   `create_unit_test_connix_gui(YourTestFile.cpp)`
*   **Execution Safeguard:** All unit tests must complete execution within **30 seconds**. If a test takes longer, CTest will forcibly terminate it. Keep networking timeouts in integration or end-to-end tests short, and use mocks whenever possible.

### Mocks Usage
*   Mocks are located in the `mock` directories of their respective modules (e.g., `connix-core/infrastructure/network-service/mock/`).
*   Tests in debug mode are automatically linked against the core mock libraries if declared via the `MOCKS` argument in `create_unit_test()`.

### Version Control & Commits
*   The repository uses **conventional commits** for change tracking (e.g., `feat: ...`, `fix: ...`, `docs: ...`, `refactor: ...`). Refer to recent commit history to match standard prefixes and messages.

---

## 4. Operational Instructions for LLMs

*   **Strict Precedence:** The configurations defined in this `GEMINI.md` file must always take absolute priority.
*   **Do Not Suppress Warnings:** Never bypass compiler warnings or linter exceptions. Ensure all new logic passes `./scripts/run_clang_format.sh` and `./scripts/run_clang_tidy.sh`.
*   **Verify Changes:** Every code modification MUST be thoroughly verified. Always execute `./scripts/build_application_native.sh` followed by `./scripts/run_unit_tests.sh` to ensure the integrity of the compilation and that all tests remain green.
