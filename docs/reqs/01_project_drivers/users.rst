Users
=====

Stakeholder interests are covered in the Stakeholders section; this section
focuses on user characteristics that shape the product's design. Priorities
are relative to each other and reflect the order in which conflicting
design decisions should be resolved in favor of a specific user group.

.. list-table::
   :header-rows: 1
   :widths: 20 25 25 15 15

   * - User
     - Subject Matter Expertise
     - Technical Expertise
     - Frequency of Use
     - Priority

   * - **Network / systems developer**
     - Understands TCP, UDP, and Unix Domain Sockets. Knows how to read raw
       socket output.
     - Comfortable with the command line and shell scripting. Familiar
       with tools such as ``netcat`` and ``socat``.
     - Daily. Uses ``connix`` as a primary tool in development and
       debugging workflows.
     - High.

   * - **QA / test engineer**
     - Understands the protocol under test at the application level.
       Scripts protocol-level test scenarios.
     - Comfortable with scripting and configuration-driven tools.
     - Regular. Uses ``connix`` to automate test scenarios via rules and
       periodic execution.
     - High.

   * - **Non-technical / visual user**
     - Has basic networking knowledge. Does not need to write shell
       scripts or code.
     - Comfortable with desktop applications. Does not require
       command-line skills.
     - Occasional to regular. Uses ``connix-gui`` for interactive
       exploration of networked services.
     - High. The GUI must provide feature parity with the CLI, not a
       reduced subset.

   * - **Library integrator**
     - Understands the protocol they need (TCP, UDP, or Unix Domain
       Sockets) at the application level.
     - Experienced C++ developer. Familiar with CMake and Conan v2.
     - Regular. Uses ``connix-core`` as a dependency in their own project
       build.
     - High. The library API must be stable and well-documented for this
       user.
