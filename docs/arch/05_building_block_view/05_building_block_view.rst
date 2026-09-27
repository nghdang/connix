Building Block View
####################

This section refines Connix's internal structure from the requirements in
``docs/reqs``. The decomposition is independent of implementation details and
uses Clean Architecture: dependencies point from outer rings toward inner
abstractions.

The design supports the CLI, optional Qt/QML GUI, and embeddable
``connix-core`` through the same application use cases. TCP, UDP, Unix Domain
Sockets, and future protocols are represented by interchangeable protocol
ports rather than protocol-specific orchestration.

1st Level Building Block View
=============================

Context
-------

The system context and external interactions are documented in
:doc:`../03_context_scope/03_context_scope`. This section focuses exclusively
on Connix's internal building blocks.

Whitebox Overall System
-----------------------

Level 1 intentionally shows only the four architectural rings. It answers
where responsibilities belong and how dependencies flow, without exposing the
internal services and drivers that are documented at Level 2.

.. uml:: level1_building_blocks.puml
   :align: center
   :caption: Connix Clean Architecture - Level 1 Rings

.. list-table:: 1st Level Building Blocks
   :header-rows: 1
   :widths: 30 70

   * - Ring
     - Responsibility
   * - Entities
     - Framework-independent domain models, values, rules, and execution
       policies.
   * - Use Cases
     - Application workflows that orchestrate domain behavior through
       inward-facing abstractions.
   * - Interface Adapters
     - Controllers, gateways, and presenters that translate external formats
       to use-case contracts.
   * - Frameworks & Drivers
     - Concrete runtimes, protocol implementations, parsers, file I/O, and
       composition-root wiring.

Dependency matrix
------------------

The matrix describes compile-time and architectural dependencies. A mark in a
cell means that the row layer may depend on the column layer. Dependencies point
inward only; an empty cell means that the dependency is not permitted.

.. list-table:: Clean Architecture dependency matrix
   :header-rows: 1
   :stub-columns: 1
   :widths: 28 18 18 22 24

   * - Depends on
     - Entities
     - Use Cases
     - Interface Adapters
     - Frameworks & Drivers
   * - Entities
     - Yes
     - No
     - No
     - No
   * - Use Cases
     - Yes
     - Yes
     - No
     - No
   * - Interface Adapters
     - Yes
     - Yes
     - Yes
     - No
   * - Frameworks & Drivers
     - Yes
     - Yes
     - Yes
     - Yes

The diagonal ``Yes`` entries represent dependencies within a layer, not a
requirement that every building block depend on every other building block.
Frameworks & Drivers contain the composition root, which selects concrete
implementations of inner-layer ports. Entities remain independent of parsers,
UI toolkits, protocol libraries, and operating-system facilities. This matrix
supports adding a protocol without modifying the core orchestration use cases.

2nd Level Building Block View
=============================

Entities
--------

.. uml:: level2_entities.puml
   :align: center
   :caption: Entities - Level 2

.. list-table:: Entities - Level 2
   :header-rows: 1
   :widths: 30 70

   * - Building block
     - Responsibility
   * - Connection model
     - Represents protocol-independent role, endpoint, lifecycle, and socket
       state.
   * - Message and endpoint values
     - Represents messages and endpoint data shared by connection and rule
       behavior.
   * - Event and rule model
     - Represents connection events, timer events, actions, and cycle scope.
   * - Execution policy
     - Represents ONETIME/PERIODIC behavior, trigger eligibility, and
       no-overlap constraints.

Use Cases
---------

.. uml:: level2_use_cases.puml
   :align: center
   :caption: Use Cases - Level 2

.. list-table:: Use Cases - Level 2
   :header-rows: 1
   :widths: 30 70

   * - Building block
     - Responsibility
   * - Configuration use case
     - Applies parser selection, command-line precedence, validation, and
       configuration invariants.
   * - Execution scheduler
     - Starts ONETIME and PERIODIC work, skips overlapping periodic cycles, and
       isolates DUAL legs.
   * - Connection-cycle use case
     - Orchestrates CLIENT, SERVER, and DUAL lifecycles, operation timeouts,
       and socket closure.
   * - Event and rule use case
     - Matches connection and timer events to scoped rule actions.
   * - Observation use case
     - Publishes sent traffic, received traffic, and diagnostic events through
       output ports.

Interface Adapters
------------------

.. uml:: level2_interface_adapters.puml
   :align: center
   :caption: Interface Adapters - Level 2

.. list-table:: Interface Adapters - Level 2
   :header-rows: 1
   :widths: 30 70

   * - Building block
     - Responsibility
   * - CLI controller
     - Maps command-line options and exit outcomes to application requests.
   * - GUI controller
     - Maps Qt/QML commands and view state to the application boundary.
   * - Configuration adapter
     - Selects the parser, maps input fields, and reports source locations.
   * - Protocol gateway
     - Translates the common client/server port to a selected protocol driver.
   * - Output presenter
     - Formats traffic and diagnostics and selects stdout or file output.

Frameworks & Drivers
--------------------

.. uml:: level2_frameworks_drivers.puml
   :align: center
   :caption: Frameworks and Drivers - Level 2

.. list-table:: Frameworks and Drivers - Level 2
   :header-rows: 1
   :widths: 35 65

   * - Building block
     - Responsibility
   * - Composition root
     - Constructs the application graph and preserves foreground execution.
   * - TCP driver
     - Implements stream communication for TCP endpoints.
   * - UDP driver
     - Implements datagram communication for UDP endpoints.
   * - Unix Domain Socket driver
     - Implements local IPC communication for UDS endpoints.
   * - CLI runtime
     - Hosts terminal invocation and process exit behavior.
   * - Qt/QML runtime
     - Hosts the optional graphical client when enabled.
   * - JSON/YAML and file I/O
     - Provides concrete configuration parsing and output-file operations.
   * - Timer/concurrency runtime
     - Provides periodic timing and concurrent DUAL-leg execution.

3rd Level Building Block View
=============================

Level 3 decomposition is deferred until implementation design identifies a
Level 2 block that needs additional architectural detail. Candidate subjects
are the connection-cycle use case, protocol-driver implementations, and
configuration processing. Keeping those diagrams deferred avoids presenting
implementation detail before the corresponding design decisions are stable.
