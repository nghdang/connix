# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Connix** is a programmable, event-driven network interaction engine that unifies multi-protocol
communication (TCP/UDP, Unix Domain Sockets) and automation in a single C++ tool. The repo has
three components under `connix/`:

- **connix-core**: static library with networking, transport, and logging layers
- **connix-cli**: CLI application (`add_executable(connix-cli)`, entry point in `src/Bootstrap/`)
- **connix-gui**: optional Qt/QML GUI (`add_executable(connix-gui)`, entry point in `src/Bootstrap/`)

Note: `CMAKE_CXX_STANDARD` is set to **17** in the root `CMakeLists.txt`. Other docs in this repo
(README.md, GEMINI.md) describe the project as C++20 — that is aspirational/inaccurate; treat the
root `CMakeLists.txt` as the source of truth for the actual standard in use.

## Build and Development Commands

### Build (Debug with Tests and Mocks)
```bash
./scripts/build_application_native.sh
# Or with a specific profile:
./scripts/build_application_native.sh --host-profile debug/gcc13_x86_64
```
Use `-f`/`--force` to wipe `build-native/` and rebuild clean. Debug builds pass
`-DBUILD_MOCKS=ON -DBUILD_TESTS=ON` to CMake.

### Release Build
```bash
./scripts/build_application_release.sh
```
Builds with `-DCMAKE_BUILD_TYPE=Release` (mocks/tests off by default) and installs to
`build-release/deploy`. Required before running clang-tidy (generates the compile database in
`build-release/`).

### Run Unit Tests
```bash
./scripts/run_unit_tests.sh
```
Requires `build-native/` to already exist (run the native build first). Runs the
`connix-core-unit-tests-run` CMake target (`ctest -j1 --tests-regex "UnitTest" --verbose`), then
the `gen-code-coverage` target (lcov + genhtml). Coverage report: `build-native/coverage/index.html`.

### Run a Single Test File
```bash
cd build-native
ctest -R "UnitTest<Name>" --verbose
# e.g.: ctest -R "UnitTestLoggerTest" --verbose
```

### Code Formatting
```bash
./scripts/run_clang_format.sh          # check (--dry-run)
./scripts/run_clang_format.sh --fix    # apply (-i)
```
Runs clang-format over every `.hpp`/`.cpp` under `connix/`, per `.clang-format` (LLVM-based).

### Static Analysis (Linting)
```bash
./scripts/build_application_release.sh  # required first, generates compile_commands.json
./scripts/run_clang_tidy.sh
```
Runs `run-clang-tidy-*.py` from `build-release/` with `-header-filter="(connix)"`, per the rules
in `.clang-tidy`.

## Architecture

### Source layout (connix-core)

The implemented `connix-core` modules are currently under
`connix/connix-core/infrastructure/`:

```
connix/connix-core/
├── domain/                           # currently no implementations
├── application/                      # currently no implementations
└── infrastructure/
    ├── configuration/                # configuration APIs, implementation, resources, mocks, tests
    └── logging/                      # logging API, implementation, tests
```

Each infrastructure module has its own `CMakeLists.txt` and subdirectories. Public and
internal headers, implementations, resources, mocks, and tests are declared through the
module’s CMake files; follow the neighboring module structure when adding files.

`docs/arch/` and `docs/reqs/` document the intended architecture and requirements. Treat them
as design constraints, but check the source tree before assuming a documented component has
already been implemented.

### CMake structure

- Root `CMakeLists.txt`: sets up the Conan toolchain (`cmake/conan_provider.cmake`), C++ standard,
  `BUILD_MOCKS`/`BUILD_TESTS` options (auto-ON for native Debug builds), `--coverage` compile/link
  flags, and the `gen-code-coverage` target (lcov + genhtml). Ends with `add_subdirectory(connix)`.
- `connix/CMakeLists.txt`: adds `connix-core`, `connix-cli`, `connix-gui` subdirectories.
- `connix/connix-core/CMakeLists.txt`: defines the `connix-core` static library, the
  `connix-core-mocks` shared library (if `BUILD_MOCKS`), and the `create_unit_test(<file>.cpp)`
  CMake function used to register tests (defined **inline in this file**, not in a separate
  `cmake/helpers.cmake` — no such file exists on this branch). The function derives a
  `UnitTest<Name>` target, links `connix-core` + `gtest::gtest` (+ `connix-core-mocks` if that
  target exists), and enforces a 30-second timeout per test via `set_tests_properties`.

### Testing

- Test naming: `UnitTest<Name>`, created via `create_unit_test()` in each module's `tst/CMakeLists.txt`.
- Tests mirror the module's `src/` (e.g. `common/logging/tst/LoggerTest.cpp` tests
  `common/logging/src/Logger.cpp`).
- 30-second timeout per test is a hard limit enforced by `create_unit_test()` — design tests to
  fail fast and mock anything that would otherwise block on real I/O.

### Dependencies

- Conan v2, with `gtest/1.16.0` required only for native (Linux) builds — see `conanfile.py`.
- Conan profiles live in `conan_profiles/{debug,release}/gcc13_x86_64`.
