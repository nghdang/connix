Naming Conventions and Definitions
===================================

This section defines all terms, acronyms, and abbreviations used
throughout the Connix requirements specification. Terms are listed
alphabetically.

Terms and Definitions
---------------------

.. list-table::
   :header-rows: 1
   :widths: 22 78

   * - Term
     - Definition
   * - **API** (Application Programming Interface)
     - A defined set of functions, types, and conventions that a software
       component exposes for use by other software components.
   * - **CLI** (Command-Line Interface)
     - A text-based interface in which the user issues commands by typing
       them in a terminal or shell. One of the two interfaces of the
       ``connix`` product.
   * - **CMake**
     - An open-source, cross-platform build system generator. CMake
       reads ``CMakeLists.txt`` files and produces native build files
       (e.g., Makefiles, Visual Studio solution files).
   * - **Conan v2**
     - Version 2 of the Conan C++ package manager. Conan resolves,
       downloads, and links third-party dependencies. It integrates
       with CMake via a generated toolchain file.
   * - **connix**
     - The name of the top-level product, and of its CLI binary. As a
       component, it is the command-line application that provides the
       unified networking interface users invoke directly from a
       terminal.
   * - **connix-core**
     - The reusable C++20 static library that implements all networking
       logic. It is the dependency embedded by library integrators in
       their own applications.
   * - **connix-gui**
     - The optional graphical client built with Qt and QML. It exposes
       the capabilities of ``connix-core`` through a visual interface,
       with feature parity with the CLI.
   * - **Dual role**
     - An operating role in which Connix acts as both a client and a
       server simultaneously for a given interaction.
   * - **Event-driven execution**
     - A mode of operation in which Connix triggers actions in response
       to events, such as message receipt, connection state changes, or
       timers.
   * - **GTest** (Google Test)
     - The Google Test C++ testing framework. Used for all unit and
       integration tests in the Connix project.
   * - **GUI** (Graphical User Interface)
     - A visual interface in which the user interacts with the software
       through windows, buttons, and other graphical controls. Refers to
       ``connix-gui`` in this document.
   * - **Library integrator**
     - A C++ developer or team that embeds ``connix-core`` as a static
       library dependency in their own application to gain programmatic
       access to the supported protocols.
   * - **MIT License**
     - A permissive open-source license. It requires only that the
       copyright notice and license text are retained in distributions.
       Connix is distributed under this license.
   * - **MSVC** (Microsoft Visual C++)
     - The C++ compiler and toolchain provided by Microsoft Visual
       Studio. The primary compiler for Windows builds of Connix.
   * - **One-time execution mode**
     - An execution mode in which Connix performs a single action and
       exits, without repeating it.
   * - **Periodic execution mode**
     - An execution mode in which Connix repeats a configured action on
       a defined interval, built into Connix itself without relying on
       an external scheduler.
   * - **QML** (Qt Modeling Language)
     - A declarative language for describing user interfaces. Used to
       implement the ``connix-gui`` frontend on top of the Qt framework.
   * - **Qt**
     - A cross-platform application framework written in C++. Used
       exclusively for the GUI component ``connix-gui``.
   * - **Rule**
     - A user-defined pairing of one triggering event (message receipt,
       connection-state change, or timer — see Event-driven execution)
       with one send or receive action, scoped to a single connection
       cycle.
   * - **TCP** (Transmission Control Protocol)
     - A connection-oriented transport-layer protocol providing
       reliable, ordered, and error-checked delivery of a byte stream
       over IP networks.
   * - **UDP** (User Datagram Protocol)
     - A connectionless transport-layer protocol that sends datagrams
       without guaranteed delivery or ordering.
   * - **UDS** (Unix Domain Socket)
     - An inter-process communication mechanism on Unix-like systems.
       Communication occurs through the filesystem rather than over a
       network. Also referred to as Unix sockets.
   * - **Volere**
     - The requirements specification template developed by Suzanne and
       James Robertson. Used as the structural framework for the Connix
       requirements documents.

Naming Conventions
------------------

The following conventions apply to identifiers and names throughout the
project.

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Context
     - Convention
   * - File names
     - PascalCase (e.g. ``ConnixClient.hpp``, ``ConnixClient.cpp``).
   * - C++ namespaces
     - Namespaces mirror the source folder path by architectural layer
       and module, e.g. ``ConnixCore::Infrastructure::NetworkService``.
