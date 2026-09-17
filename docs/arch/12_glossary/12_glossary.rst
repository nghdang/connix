Glossary
########

.. list-table:: Connix architecture terms
   :header-rows: 1
   :widths: 25 75

   * - Term
     - Definition
   * - CLIENT
     - Operating role that initiates an outbound connection.
   * - SERVER
     - Operating role that accepts an inbound connection.
   * - DUAL
     - Operating role that runs independent CLIENT and SERVER legs for one
       trigger.
   * - ONETIME
     - Execution mode that performs one configured action and exits.
   * - PERIODIC
     - Execution mode that evaluates timer triggers at a configured interval.
   * - Connection cycle
     - A bounded connect or accept, message exchange, and close sequence.
   * - Rule
     - One supported event paired with one send or receive action within a
       triggering cycle.
   * - Protocol port
     - Framework-independent client/server abstraction implemented by concrete
       protocol drivers.
   * - Composition root
     - The outermost wiring point where concrete adapters and drivers are
       selected and connected to use cases.
   * - connix-core
     - Reusable C++20 static library exposing Connix application capabilities.
   * - Interface adapter
     - Component translating an external representation to or from a use-case
       contract.
   * - UDS
     - Unix Domain Socket, used for local inter-process communication where
       supported by the operating system.
