Users
=====

This section describes the hands-on users of Connix -- the people who directly interact with the product.
Stakeholder interests are covered in the Stakeholders section; this section focuses on user characteristics
that shape the product's design.

.. list-table::
   :header-rows: 1
   :widths: 20 25 25 15 15

   * - User
     - Subject Matter Expertise
     - Technical Expertise
     - Frequency of Use
     - Priority

   * - **CLI user**
       (network or systems developer)
     - Understands networking protocols: HTTP/REST, TCP, UDP, and Unix Domain Sockets.
       Knows how to read raw socket output and interpret HTTP responses.
     - Comfortable with the command line and shell scripting. Familiar with tools such as
       ``curl``, ``netcat``, and ``socat``. C++ knowledge not required.
     - Daily. Uses ``connix-cli`` as a primary tool in development and debugging workflows.
     - High. This is the primary user the CLI is designed for.

   * - **Library consumer**
       (C++ application integrator)
     - Understands the protocol they need (REST, TCP/UDP, or Unix sockets) at the
       application level.
     - Experienced C++ developer. Familiar with CMake and package managers such as
       Conan v2. Reads library headers and documentation to integrate the API.
     - Regular. Uses ``connix-core`` as a dependency in their own project build.
     - High. The library API must be stable and well-documented for this user.

   * - **GUI user**
     - Has basic networking knowledge -- knows what HTTP requests and TCP connections
       are, but does not need to write shell scripts or code.
     - Comfortable with desktop applications. Does not require command-line skills.
     - Occasional to regular. Uses ``connix-gui`` for interactive exploration of
       networked services.
     - Medium. The GUI is an optional component and secondary to the CLI and library.

Priorities are relative to each other and reflect the order in which conflicting design
decisions should be resolved in favor of a specific user group.
