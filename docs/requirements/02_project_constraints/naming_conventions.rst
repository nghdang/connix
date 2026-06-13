Naming Conventions and Definitions
===================================

This section defines all terms, acronyms, and abbreviations used throughout the Connix
requirements specification. Terms are listed alphabetically.

Terms and Definitions
---------------------

.. list-table::
   :header-rows: 1
   :widths: 22 78

   * - Term
     - Definition
   * - **API** (Application Programming Interface)
     - A defined set of functions, types, and conventions that a software component exposes for
       use by other software components.
   * - **CLI** (Command-Line Interface)
     - A text-based interface in which the user issues commands by typing them in a terminal or
       shell. The primary interface of the ``connix`` tool.
   * - **CMake**
     - An open-source, cross-platform build system generator. CMake reads ``CMakeLists.txt``
       files and produces native build files (e.g., Makefiles, Visual Studio solution files).
   * - **Conan v2**
     - Version 2 of the Conan C++ package manager. Conan resolves, downloads, and links
       third-party dependencies. It integrates with CMake via a generated toolchain file.
   * - **connix**
     - The name of the top-level product and of its CLI binary. As a component, it is the
       command-line application that provides the unified networking interface users invoke
       directly from a terminal.
   * - **connix-core**
     - The reusable C++20 static library that implements all networking logic. It is the
       dependency embedded by library consumers in their own applications.
   * - **connix-gui**
     - The optional graphical client built with Qt and QML. It exposes the capabilities of
       ``connix-core`` through a visual interface.
   * - **GTest** (Google Test)
     - The Google Test C++ testing framework. Used for all unit and integration tests in the
       Connix project.
   * - **GUI** (Graphical User Interface)
     - A visual interface in which the user interacts with the software through windows,
       buttons, and other graphical controls. Refers to ``connix-gui`` in this document.
   * - **HTTP** (Hypertext Transfer Protocol)
     - The application-layer protocol used for web and REST communication. Connix supports
       HTTP/1.1 and HTTP/2 via its REST protocol handler.
   * - **Library consumer**
     - A C++ developer or team that embeds ``connix-core`` as a static library dependency in
       their own application to gain programmatic access to the supported protocols.
   * - **MIT License**
     - A permissive open-source license. It requires only that the copyright notice and
       license text are retained in distributions. Connix is distributed under this license.
   * - **MSVC** (Microsoft Visual C++)
     - The C++ compiler and toolchain provided by Microsoft Visual Studio. The primary
       compiler for Windows builds of Connix.
   * - **QML** (Qt Modeling Language)
     - A declarative language for describing user interfaces. Used to implement the
       ``connix-gui`` frontend on top of the Qt framework.
   * - **Qt**
     - A cross-platform application framework written in C++. Used exclusively for the GUI
       component ``connix-gui``.
   * - **REST** (Representational State Transfer)
     - An architectural style for networked applications that uses HTTP methods (GET, POST,
       PUT, DELETE, etc.) to operate on resources identified by URLs.
   * - **TCP** (Transmission Control Protocol)
     - A connection-oriented transport-layer protocol providing reliable, ordered, and
       error-checked delivery of a byte stream over IP networks.
   * - **UDP** (User Datagram Protocol)
     - A connectionless transport-layer protocol that sends datagrams without guaranteed
       delivery or ordering.
   * - **UDS** (Unix Domain Socket)
     - An inter-process communication mechanism on Unix-like systems. Communication occurs
       through the filesystem rather than over a network. Also referred to as Unix sockets.
   * - **Volere**
     - The requirements specification template developed by Suzanne and James Robertson.
       Used as the structural framework for the Connix requirements documents.

Naming Conventions
------------------

The following conventions apply to identifiers and names throughout the project.

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Context
     - Convention
   * - CMake
     - cmake.org docs (https://cmake.org/cmake/help/latest/).
   * - C++
     - C++ Core Guidelines (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).
   * - Python
     - PEP 8 (https://peps.python.org/pep-0008/).
   * - Bash
     - Google Shell Style Guide (https://google.github.io/styleguide/shellguide.html).
