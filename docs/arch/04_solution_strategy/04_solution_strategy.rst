Solution Strategy
#################

Architectural Style
===================

Connix uses Clean Architecture with four dependency rings:

* **Entities** contain framework-independent connection, event, rule, and
  execution-policy models.
* **Use Cases** orchestrate configuration, scheduling, connection cycles, rule
  execution, and observation through inward-facing ports.
* **Interface Adapters** translate CLI, Qt/QML, configuration, protocol, and
  output representations into use-case contracts.
* **Frameworks & Drivers** contain concrete protocol I/O, parsers, file I/O,
  timers, runtimes, and composition-root wiring.

Source dependencies point inward. The composition root selects concrete
implementations, allowing ``connix-core`` to remain reusable and allowing
protocol implementations to evolve without changing orchestration logic.

Application Shape
=================

The CLI and optional GUI are separate interface adapters over the same use
cases. The GUI is optional at build and distribution time; disabling it must
not disable a compliant headless CLI or library build. Equivalent CLI and GUI
capabilities use the same application requests and produce the same observable
results.

Execution Strategy
==================

Connix provides two built-in execution modes:

* **ONETIME** performs one configured action and exits.
* **PERIODIC** emits timer events at the configured interval and starts one
  eligible cycle at a time, skipping a trigger while the previous periodic
  cycle is active.

CLIENT, SERVER, and DUAL roles are modeled in the connection-cycle use case.
DUAL starts separate concurrent CLIENT and SERVER legs with independent sockets,
closure, and failure handling.

Protocol Strategy
=================

TCP, UDP, and Unix Domain Socket drivers implement a common client/server
protocol port. The gateway and use cases depend on that abstraction rather than
on a specific networking library. A future protocol can therefore be added by
implementing the existing interface without modifying the core orchestration
engine.

Configuration and Observation
==============================

Configuration is loaded from JSON or YAML. File extensions select the parser;
extensionless input requires an explicit format. Parse failures are reported and
abort execution before configured actions begin. Command-line values override
file values. Traffic and diagnostics are sent to stdout by default or to a
configured output file, with stdout suppression during redirection.

Quality-Driven Decisions
=========================

.. list-table:: Strategy decisions and quality goals
   :header-rows: 1
   :widths: 28 32 40

   * - Quality or requirement
     - Strategy decision
     - Result
   * - Protocol extensibility
     - Common protocol port plus composition-root wiring
     - New protocols do not require changes to core orchestration.
   * - CLI/GUI consistency
     - Separate controllers over shared use cases
     - Interface choice does not change the underlying action or result.
   * - Low resource usage
     - Foreground process, bounded cycles, and periodic no-overlap policy
     - Avoids unbounded socket and trigger accumulation.
   * - Deterministic failures
     - Explicit parser selection and operation-specific timeouts
     - Invalid input and blocked operations have observable outcomes.
   * - Portability
     - Platform-specific behavior isolated in drivers and runtimes
     - Windows and Linux can share the inner architecture.

Fixed Technology Choices
========================

The implementation is constrained to C++20, CMake, Conan v2, and Google Test.
The optional graphical client uses Qt/QML. These technologies are outer-ring
concerns and must not leak into Entities or framework-independent Use Cases.
