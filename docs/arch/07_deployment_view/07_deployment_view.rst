Deployment View
################

Deployment targets
==================

Connix is deployed as a foreground process on Windows 10 64-bit or Linux with
glibc 2.31 or later. A deployment may contain the CLI executable, the optional
GUI executable, and the reusable ``connix-core`` static library.

.. list-table:: Deployment elements
   :header-rows: 1

   * - Element
     - Deployment responsibility
   * - CLI package
     - Runs configured ONETIME or PERIODIC workflows in a terminal.
   * - Optional GUI package
     - Hosts the Qt/QML interface when GUI support is enabled.
   * - Core library package
     - Is linked into an integrator's C++20 application.
   * - Configuration and output files
     - Are supplied or selected by the invoking user or application.
   * - Network / IPC peers
     - Are external endpoints reached through TCP, UDP, or UDS drivers.

Operational topology
====================

Connix does not detach, create a PID file, or provide a built-in daemon. Process
supervision and background execution are responsibilities of the host operating
system. Unix Domain Sockets are a Linux deployment capability and are not
required on Windows.
