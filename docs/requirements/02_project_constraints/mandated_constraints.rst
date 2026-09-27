Mandated Constraints
====================

Solution Constraints
--------------------

The following constraints are imposed on the solution and cannot be changed.

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Constraint
     - Rationale
   * - The implementation language is C++20.
     - C++20 features (concepts, ranges, coroutines) are needed for the core library design.
       No earlier standard is permitted.
   * - CMake is the build system.
     - Library consumers embed ``connix-core`` via CMake's ``find_package`` or
       ``FetchContent`` mechanisms. CMake must be used so those integration paths work.
   * - Conan v2 is the package manager.
     - Conan v2 provides hermetic, reproducible dependency resolution. It integrates directly
       with CMake via a generated toolchain file and must not be replaced with an alternative.
   * - Google Test (GTest) is the testing framework.
     - Google Test is the only approved framework for unit and integration tests. It integrates
       cleanly with CMake via ``FetchContent`` or Conan.
   * - The optional GUI component uses Qt and QML.
     - Qt/QML is the only approved GUI toolkit. No other UI framework may be introduced for
       ``connix-gui``.
   * - The license is MIT.
     - The MIT license allows ``connix-core`` to be reused by library consumers and
       open-source contributors without restriction. The license must not be changed.

Implementation Environment Constraints
---------------------------------------

Connix must build and run correctly on the following target platforms.

.. list-table::
   :header-rows: 1
   :widths: 20 30 50

   * - Platform
     - Minimum Version
     - Compiler Requirements
   * - Windows
     - Windows 10 (64-bit)
     - MSVC 2022 is the primary compiler. Clang-cl is also supported.
   * - Linux
     - Any distribution shipping glibc 2.31 or later (e.g., Ubuntu 20.04)
     - GCC 11 or later is the primary compiler. Clang 14 or later is also supported.

The development toolchain (compiler, CMake, Conan) must be installable on both platforms without
requiring administrator privileges beyond a standard developer workstation setup.

No 32-bit target is required. ARM targets are out of scope for the initial release.

Partner and Collaborator Constraints
-------------------------------------

Connix is an open-source project. The following rules apply to contributions and integrations.

- External contributors must submit changes via pull requests. They must follow the project
  contribution guidelines before any change is merged.
- ``connix-core`` must expose a stable public C++ API. Any breaking change to the public API
  requires a major version bump in the project version number.
- No third-party dependency may be added without a corresponding entry in the Conan recipe and
  a documented rationale explaining why the dependency is necessary.

Schedule Constraints
--------------------

There is no externally imposed deadline. The project is maintained by a single author.
No release date is set in advance.
