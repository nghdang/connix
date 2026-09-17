Quality Requirements
#####################

Quality goals
=============

.. list-table:: Quality requirements
   :header-rows: 1
   :widths: 28 32 40

   * - Quality attribute
     - Requirement
     - Architectural response
   * - Determinism
     - Invalid configuration performs no action; execution modes have defined
       trigger behavior.
     - Configuration validation precedes composition and scheduling; explicit
       ONETIME/PERIODIC policies govern execution.
   * - Performance
     - Idle CPU remains below 1% and long-running RSS growth remains within the
       specified limit.
     - Periodic no-overlap and bounded resource ownership prevent accumulation.
   * - Maintainability
     - New protocols do not require changes to core orchestration.
     - Common protocol ports and dependency inversion isolate drivers.
   * - Usability
     - Equivalent CLI and GUI capabilities perform the same action and produce
       the same result.
     - Both interfaces invoke shared use cases.
   * - Portability
     - Windows and Linux deployments are supported within their constraints.
     - Platform-specific behavior remains in outer drivers and runtimes.
   * - Testability
     - Core behavior is testable without real external I/O.
     - Entities and Use Cases depend on abstractions and are exercised with
       Google Test.

Deferred qualities
==================

Security hardening and GUI look-and-feel requirements are not defined for the
current phase. They remain explicit elicitation items rather than implicit
architectural guarantees.
