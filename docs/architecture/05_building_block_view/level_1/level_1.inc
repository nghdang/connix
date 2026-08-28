Level 1
=======

.. uml:: level_1/level_1.puml
   :align: center

.. list-table::
    :header-rows: 1

   * - Building Block
     - Description
   * - Interface Layer
     - Exposes the SDK boundary (contracts, configuration parameters, and the central engine interface) and provides factories to instantiate the system without
       revealing implementation details.
   * - Core Layer
     - Contains the core logic of the system, orchestrating network configurations, verifying network topology safety, and dispatching stream traffic based on
       user-defined rules.
   * - Infrastructure Layer
     - Encapsulates OS-specific system boundaries, executing raw socket I/O (using POSIX APIs/epoll), managing timer callbacks, and handling persistent logging or PCAP
       packet captures.
