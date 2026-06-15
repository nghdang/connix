Functional Requirements
=======================

User Interfaces
---------------

.. id:: SW_REQ USER_INTERFACES
    :desc: The Connix SHALL provide two user interfaces: Command-Line Interface (CLI) and Graphical User Interface (GUI).
    :rationale: Supports both scripting-oriented and visual interaction workflows.

Protocol Support
----------------

.. id:: SW_REQ MULTIPLE_PROTOCOLS
   :desc: The Connix SHALL support communication over REST, TCP, UDP, and Unix Domain Sockets.
   :rationale: Eliminates the need for multiple tools across protocols.

Operating Roles
---------------

.. id:: SW_REQ WORKING_ROLES
    :desc: The Connix SHALL operate in three roles: CLIENT, SERVER, and DUAL.
    :rationale: Enables flexible deployment without additional tooling.

Connection Management
---------------------

.. id:: SW_REQ MULTIPLE_CONNECTIONS
    :desc: The Connix SHALL allow users to switch between active connections without requiring reconnection.
    :rationale: Improves efficiency when managing multiple sessions.

.. id:: SW_REQ STATEFUL_CONNECTION
    :desc: The Connix SHALL maintain stateful connections across interactions.
    :rationale: Supports multi-step workflows without reinitialization.

Event Handling
--------------

.. id:: SW_REQ EVENT_DRIVEN
    :desc: The Connix SHALL support event-driven execution, triggering actions on events such as message receipt, connection changes, or timers.
    :rationale: Enables reactive automation without interrupting sessions.

Rule Engine
-----------

.. id:: SW_REQ RULE_BASED
    :desc: The Connix SHALL allow users to define rules governing message sending and receiving behavior.
    :rationale: Reduces manual intervention and supports automation.

Execution Modes
---------------

.. id:: SW_REQ RUNNING_MODES
    :desc: The Connix SHALL support ONETIME and PERIODIC execution modes.
    :rationale: Allows both single-run and recurring operations.

Configuration
-------------

.. id:: SW_REQ CONFIGURATION_FILE
    :desc: The Connix SHALL accept configuration files in XML, JSON, and YAML formats.
    :rationale: Provides flexibility for defining workflows and reproducible setups.

Monitoring & Logging
--------------------

.. id:: SW_REQ MONITORING
    :desc: The Connix SHALL provide real-time visibility of connection states and events through logging streams.
    :rationale: Enables runtime observability and troubleshooting.
