Core Layer
----------

.. uml:: level_3/core_layer/core_layer.puml
   :align: center

.. list-table:: Detailed Component Interfaces and Implementations
    :header-rows: 1

   * - Element / Component
     - Layer
     - Type
     - Description
   * - IConnixEngine
     - Interface Layer
     - Pure Virtual Interface
     - Defines the primary SDK orchestration entry point for client applications to load topologies, start/stop traffic processing, and register event rules.
   * - INetworkService
     - Interface Layer
     - Pure Virtual Interface
     - Abstract boundary for all socket and network interactions, decoupling the routing engine from the underlying OS-specific network stack.
   * - ITimeService
     - Interface Layer
     - Pure Virtual Interface
     - Abstract boundary for high-precision time scheduling, heartbeat dispatches, and connection timeout tracking.
   * - IPersistenceService
     - Interface Layer
     - Pure Virtual Interface
     - Abstract boundary for writing network stream metrics, PCAP packet dumps, and loading local configurations.
   * - ConnixFactory
     - Interface Layer
     - Concrete Class (Factory)
     - Responsible for instantiating the concrete services and core components, performing constructor dependency injection into the TrafficDirector, and returning a pointer to the abstract IConnixEngine interface.
   * - TrafficDirector
     - Core Layer
     - Concrete Class (Orchestrator)
     - Implements IConnixEngine. Orchestrates all network flows using injected references to INetworkService, ITimeService, and IPersistenceService. It has zero compile-time dependencies on physical/OS-specific sockets.
   * - TopologyRegistry
     - Core Layer
     - Concrete Class
     - Stores and validates the structural topology of the network nodes, verifying that endpoints are sound and no routing loop states exist.
   * - POSIXNetworkService
     - Infrastructure Layer
     - Concrete Class (Implementation)
     - Implements INetworkService. Executes raw socket lifecycle tasks, non-blocking I/O operations, and POSIX epoll event multiplexing on Linux.
   * - POSIXTimeService
     - Infrastructure Layer
     - Concrete Class (Implementation)
     - Implements ITimeService. Manages platform-specific timers (e.g. using timerfd or standard library steady clocks) to schedule background events.
   * - FilePersistenceService
     - Infrastructure Layer
     - Concrete Class (Implementation)
     - Implements IPersistenceService. Writes logs and serialized PCAP data directly to the host filesystem.

Dependency Inversion Mechanism
------------------------------

A core design target of Connix is to achieve complete testability of complex data routing without spawning actual network interfaces.

By applying the **Dependency Inversion Principle (DIP)**:
1. **Core Decoupling**: High-level orchestrators like ``TrafficDirector`` do not import or depend on Linux-specific socket code or file streams. Instead, they depend exclusively on the abstract interfaces (``INetworkService``, ``ITimeService``, ``IPersistenceService``) defined in the Interface Layer.
2. **Infrastructure Implementation**: Lower-level OS-specific components (``POSIXNetworkService``, etc.) reside in the Infrastructure Layer and implement these abstract contracts.
3. **Seamless Testability**: For unit and integration testing of the orchestrator, a mock service layer (e.g., ``MockNetworkService``) can be seamlessly injected into ``TrafficDirector``, allowing full validation of packet routing state machines in a purely deterministic, in-memory environment.

Factory and Dependency Injection (DI)
-------------------------------------

To shield the end-user application from constructor complexity and object graph wiring:
- ``ConnixFactory::createEngine()`` serves as the central orchestration point.
- It instantiates the concrete infrastructure classes (``POSIXNetworkService``, ``POSIXTimeService``, ``FilePersistenceService``).
- It instantiates ``TopologyRegistry`` and ``TrafficDirector``, injecting the concrete service references into ``TrafficDirector``'s constructor.
- It returns a ``std::unique_ptr<IConnixEngine>`` to the client, restricting user visibility strictly to the abstract interfaces.
