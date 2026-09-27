Assumptions
===========

This section lists facts assumed to be true for the duration of the project. The requirements
depend on these assumptions. If any assumption proves false, the affected requirements must be
reviewed and updated.

Development Environment
-----------------------

- A C++20-conforming compiler (GCC 11+, Clang 14+, or MSVC 2022) is available on every
  development and CI machine.
- CMake 3.24 or later is available on every development and CI machine. This is the minimum
  version that provides first-class Conan v2 toolchain integration.
- Conan 2.0 or later is installed and on the PATH on every development and CI machine. It is
  configured with access to the default Conan Center Index remote.
- Python 3.8 or later is available on every development and CI machine. It is required by Conan v2.
- Internet access is available during the initial dependency resolution phase so that Conan can
  download packages from the Conan Center Index. Offline or air-gapped build environments are
  not a supported configuration for the initial release.

Target Platforms
----------------

- Target machines run a 64-bit operating system. Windows 10 (or later) on Windows, and a
  Linux distribution with glibc 2.31 or later on Linux.
- Users on Windows have access to a terminal (Command Prompt, PowerShell, or Windows Terminal)
  that can invoke ``connix`` from the PATH.
- Users on Linux have a POSIX-compliant shell. The standard shell tools (``echo``, ``cat``,
  pipes, redirection) work as expected for composing ``connix`` in scripts.
- Unix Domain Socket (UDS) support is not available on Windows. ``connix`` gracefully reports
  this as an unsupported protocol on that platform rather than failing silently.

Project Organization
--------------------

- The project is maintained by a single author. No external team or organization imposes a
  delivery schedule or mandates a specific workflow.
- There is no legacy codebase to migrate. All code, documentation, and configuration is
  written from scratch.
- The public GitHub repository is the single source of truth for code, issues, and releases.
  No separate issue tracker or project management tool is assumed.
- Contributions from the open-source community are possible but not required for the project
  to succeed. The project must be self-sufficient with a single maintainer.

Third-Party Dependencies
------------------------

- All required third-party libraries are available through the Conan Center Index under
  licenses that are compatible with the MIT license.
- The Qt framework (required for ``connix-gui``) is available under the LGPL v3 license.
  Library consumers who use only ``connix-core`` are not affected by Qt's license terms.
- No third-party dependency requires a paid license or a proprietary tool to build.

Scope Boundaries
----------------

- This specification covers ``connix``, ``connix-core``, and ``connix-gui`` only. No
  server-side component, hosted service, or cloud infrastructure is in scope.
- The requirements assume that network connectivity and OS-level socket support function
  correctly. Connix does not implement its own networking stack below the socket API.
- Security of the underlying TLS implementation is delegated to the chosen TLS library
  (selected via Conan). Connix is not responsible for cryptographic correctness below
  its own API surface.
