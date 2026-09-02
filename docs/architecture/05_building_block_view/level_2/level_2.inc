Level 2
=======

.. uml:: level_2/level_2.puml
   :align: center

.. list-table:: Level 2 Building Blocks
    :header-rows: 1

   * - Building Block
     - Layer
     - Description
   * - ConnixContract
     - Interface Layer
     - Exposes public abstract interfaces, event subscriber contracts, and configuration schemas that client applications interact with, ensuring compile-time decoupling and ABI stability.
   * - ConnixFactory
     - Interface Layer
     - Serves as the centralized abstract factory/facade to instantiate nodes, register configurations, and bootstrap the engine without exposing concrete implementations.
   * - TopologyRegistry
     - Core Layer
     - Manages the active network topologies, validates node connections and routing configurations, and prevents structural conflicts (e.g. duplicate port bindings).
   * - TrafficDirector
     - Core Layer
     - Orchestrates the active packet flow, evaluating user-defined dynamic rules, managing stream states, and dispatching events between transport endpoints.
   * - NetworkService
     - Infrastructure Layer
     - Implements socket-level lifecycle and standard protocol transports (TCP, UDP, UDS) using POSIX APIs and an event-driven multiplexer (e.g., epoll).
   * - TimeService
     - Infrastructure Layer
     - Manages high-resolution hardware timers, scheduled events, connection timeouts, and delayed packet dispatching.
   * - PersistenceService
     - Infrastructure Layer
     - Handles file system I/O, persistence of network rules, custom packet logging, and PCAP stream captures.
