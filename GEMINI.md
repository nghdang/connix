# Connix - Project Instructions & Development Guidelines

This file provides foundational mandates, technical instructions,
architectural patterns, and development workflows for the **Connix** project.
It is intended to guide LLM assistants and developers to ensure consistent,
idiomatic code contributions and compliant practices.

---

## 1. Project Overview

**Connix** is a programmable, event-driven network interaction engine designed
to unify multi-protocol communication (including HTTP/REST, TCP/UDP, and Unix
Domain Sockets) in a single high-performance tool.

### Key Technologies
*   **Programming Language:** C++17 (standardized in build configuration)
*   **Compiler:** GCC 13 (default native profile)
*   **Build System:** CMake (minimum version 3.22) with Ninja generator
*   **Dependency/Package Management:** Conan v2
*   **Unit Testing Framework:** Google Test (`gtest/1.16.0` for native builds)
*   **Static Analysis & Linting:** `clang-tidy` (strict custom rules)
*   **Code Formatting:** `clang-format` (LLVM-based style)
*   **Code Coverage:** `lcov` + `genhtml`

### Sub-Module Architecture
The codebase is structured into three main components under `connix/`:
1.  **connix-core** (`connix/connix-core/`): The core library implementing
    clean architecture layers (domain, application, infrastructure). It
    builds as a static library.
2.  **connix-cli** (`connix/connix-cli/`): The command-line interface
    application wrapper.
3.  **connix-gui** (`connix/connix-gui/`): The graphical user interface wrapper
    using Qt/QML (optional).

### Layer Hierarchy
`connix-core` is organized according to clean architecture principles:
*   **Domain (`connix/connix-core/domain/`):** Enterprise and business domain
    entities, models, and contracts.
*   **Application (`connix/connix-core/application/`):** Use cases and
    application orchestration.
*   **Infrastructure (`connix/connix-core/infrastructure/`):** Technical
    services, configuration, logging, and platform-specific transport adapters.

---

## 2. Building, Running, and Testing

The repository provides automated bash utility scripts in the `scripts/`
directory to manage environment setup, building, testing, formatting, and
linting.

### Build Commands

*   **Native Debug Build (with Tests and Mocks):**
    ```bash
    ./scripts/build_application_native.sh
    ```
    *   Automatically runs CMake with debug variables and triggers building of
        all components.
    *   Configures `BUILD_TESTS=ON`. Mock targets are always built and packaged.
    *   To force a clean rebuild, use the `-f` or `--force` flag:
        ```bash
        ./scripts/build_application_native.sh --force
        ```
    *   To specify custom Conan host/build profiles:
        ```bash
        ./scripts/build_application_native.sh \
            --host-profile <profile> \
            --build-profile <profile>
        ```

*   **Release Build (Packaging & Deploying):**
    ```bash
    ./scripts/build_application_release.sh
    ```
    *   Builds with optimization and disables testing (`BUILD_TESTS=OFF`).
        Mock libraries are packaged and delivered with the core library.
    *   Installs the compiled binary artifacts to `build-release/deploy` (or a
        custom prefix folder).

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
*   **Formatting Rules:** Enforced by `clang-format` based on the
    `.clang-format` configuration file (inheriting LLVM styles).
*   **Format Verification & Inline Correction:**
    *   To check formatting:
        ```bash
        ./scripts/run_clang_format.sh
        ```
    *   To apply changes inline:
        ```bash
        ./scripts/run_clang_format.sh --fix
        ```
*   **Linter Checks:** Strict static analysis via `clang-tidy` based on
    `.clang-tidy` settings.
    ```bash
    ./scripts/run_clang_tidy.sh
    ```
    *Note: Running `clang-tidy` requires compiling a native debug build
    (`./scripts/build_application_native.sh`) first to generate the
    compilation database with tests.*
*   **Strict Diagnostics:** Never suppress compiler warnings or linter
    diagnostics merely to make a change pass.
*   **Implementation Separation:** Always separate class member function
    implementations into `*.cpp` files rather than defining them inline in
    header files (except for templates or defaulted destructors).
*   **Namespace Resolution:** Always use full namespace resolution (e.g.,
    explicitly qualify types and symbols with their full namespace path; avoid
    `using namespace` directives and unqualified symbol lookups).
*   **Avoid using `std::optional`:** Avoid using `std::optional` as much as
    possible; prefer concrete default values, empty containers/strings, or
    dedicated encapsulated types to eliminate caller ambiguity.

### Architecture & Design Documentation
*   Treat existing architecture and detailed-design documentation
    (`docs/arch/`, `docs/reqs/`, and component `dd/` folders) as design
    constraints unless the task explicitly requires changing the design.

### Naming & Structure Conventions
*   **File Names:** Consistent PascalCase (e.g. `ConnixClient.hpp`,
    `ConnixClient.cpp`).
*   **Namespaces:** Match the folder structure under `ConnixCore::` (e.g.,
    `ConnixCore::Infrastructure::Logging`,
    `ConnixCore::Infrastructure::Configuration`).
*   **Public API Headers:** Exposed in `api/` or `inc/` directories under
    `ConnixCore/` path.
*   **Internal API Headers:** Exposed in `api-internal/` subdirectories.

### Unit Testing Practices
*   **Directory Mapping:** Tests are located in corresponding `tst`
    subfolders, mirroring the source structure (e.g.
    `connix-core/infrastructure/logging/tst/LoggerTest.cpp` tests
    `connix-core/infrastructure/logging/src/Logger.cpp`).
*   **Test Executable Creation:** CMake includes a custom parser function
    `create_unit_test()` defined in `cmake/helpers.cmake`. Use the specific
    wrappers to declare your tests:
    *   `create_unit_test_connix_core(YourTestFile.cpp)`
    *   `create_unit_test_connix_cli(YourTestFile.cpp)`
    *   `create_unit_test_connix_gui(YourTestFile.cpp)`
*   **Execution Safeguard:** All unit tests must complete execution within
    **30 seconds**. If a test takes longer, CTest will forcibly terminate it.
    Keep networking timeouts in integration or end-to-end tests short, and use
    mocks whenever possible.

### Mocks Usage
*   Mocks are located in the `gmock` directories of their respective modules
    (e.g., `connix-core/infrastructure/network-service/gmock/`).
*   Tests are automatically linked against the core mock libraries
    (`connix-core-mocks`).

### Version Control & Commits
*   The repository uses **conventional commits** for change tracking (e.g.,
    `feat: ...`, `fix: ...`, `docs: ...`, `refactor: ...`). Refer to recent
    commit history to match standard prefixes and messages.
