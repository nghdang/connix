Risks and Technical Debt
#########################

Current risks
=============

.. list-table:: Architectural risks
   :header-rows: 1
   :widths: 28 32 40

   * - Risk
     - Impact
     - Mitigation
   * - Protocol abstraction is too broad
     - Drivers become difficult to implement consistently across stream,
       datagram, and local IPC semantics.
     - Keep the common port focused on lifecycle operations and preserve
       protocol-specific behavior inside each driver.
   * - DUAL concurrency is incorrectly coupled
     - One failed leg could stop or retain resources for the other leg.
     - Use separate leg state, sockets, outcomes, and lifecycle tests.
   * - CLI and GUI behavior diverges
     - Users receive different actions or results for equivalent configuration.
     - Route both interfaces through the same use cases and maintain parity
       scenario tests.
   * - Long-running PERIODIC execution accumulates resources
     - CPU or memory usage violates the defined limits.
     - Enforce no-overlap cycles, prompt closure, and long-duration resource
       tests.
   * - Platform differences leak inward
     - The core library becomes difficult to build or embed across targets.
     - Keep OS, Qt/QML, and networking dependencies in outer drivers.

Technical debt register
=======================

The architecture is documentation-first while implementation design is still
being refined. Detailed Level 3 views for connection-cycle orchestration,
configuration processing, and protocol drivers are intentionally deferred until
those interfaces are stable. Security hardening and GUI look-and-feel are also
open requirements topics.
