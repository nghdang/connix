Constraints
###########

Technical Constraints
=====================

.. list-table:: Technical constraints
   :header-rows: 1
   :widths: 25 75

   * - Constraint
     - Architectural consequence
   * - C++20
     - ``connix-core`` and its consumers use C++20; no earlier language
       standard is permitted.
   * - CMake
     - The build and integration boundary uses CMake, including
       ``find_package`` or ``FetchContent`` consumption of ``connix-core``.
   * - Conan v2
     - Third-party dependencies are resolved through reproducible Conan v2
       profiles and generated CMake toolchains.
   * - Google Test
     - Unit and integration tests use Google Test.
   * - Qt/QML
     - The optional GUI uses Qt and QML and must be independently disabled for
       headless builds.
   * - MIT license
     - The library and reusable components remain distributable under the MIT
       license.

Platform Constraints
====================

.. list-table:: Target platforms
   :header-rows: 1
   :widths: 25 35 40

   * - Platform
     - Minimum environment
     - Protocol limitation
   * - Windows
     - Windows 10 64-bit with MSVC 2022
     - Unix Domain Sockets are not required on Windows.
   * - Linux
     - Distribution with glibc 2.31 or later and GCC 11 or later
     - TCP, UDP, and Unix Domain Sockets are supported.

Operational Constraints
=======================

Connix remains attached to its invoking process and terminal. It provides no
built-in daemon mode, detach option, or PID-file management. Users may apply
operating-system process supervision when background execution is needed.

Organizational Constraints
==========================

The project is maintained by a single author with no externally imposed
release deadline. External contributions are accepted through pull requests.
There is no legacy codebase to migrate.

Architectural Implications
==========================

The optional GUI, protocol implementations, and package integrations must stay
outside the framework-independent Entities and Use Cases rings. Concrete
platform behavior is selected in the composition root so the same core
workflows remain testable with Google Test and embeddable by library users.
