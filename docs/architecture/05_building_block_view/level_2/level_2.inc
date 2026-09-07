Level 2
=======

Interface Layer
---------------

.. uml:: 10_interface_layer.puml
   :align: center

.. list-table::
   :header-rows: 1

   * - Building Block
     - Description
   * - ConnixFactory
     - Factory pattern implementation that integrators use to
       instantiate and configure the ConnixEngine.

Core Layer
----------

.. uml:: 20_core_layer.puml
   :align: center

.. list-table::
   :header-rows: 1

   * - Building Block
     - Description
   * - DomainModel
     - Central data structures representing domain concepts and
       application state.
   * - ConnixEngine
     - Primary orchestration engine that coordinates topology,
       conditions, and task scheduling.
   * - TopologyRegistry
     - Manages system topology and communication node definitions.
   * - ConditionEvaluator
     - Evaluates runtime conditions for event-driven logic.
   * - TaskScheduler
     - Schedules and executes tasks in the communication workflow.

Infrastructure Layer
--------------------

.. uml:: 30_infrastructure_layer.puml
   :align: center

.. list-table::
   :header-rows: 1

   * - Building Block
     - Description
   * - NetworkService
     - Provides protocol implementations (TCP, UDP, UDS) and
       manages data exchange with external applications.
   * - FileSystemService
     - Handles file system operations and persistence.
   * - TimerService
     - Manages timing, delays, and event scheduling.
