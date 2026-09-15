Functional Requirements
=======================

User Interfaces
----------------

.. sw-req:: User Interfaces
    :id: SW_REQ_USER_INTERFACES
    :desc: The Connix SHALL provide two user interfaces: Command-Line
        Interface (CLI) and Graphical User Interface (GUI).
    :rationale: Supports both scripting-oriented and visual interaction
        workflows, with the GUI providing feature parity with the CLI.

Protocol Support
-----------------

.. sw-req:: Protocol Support
    :id: SW_REQ_MULTIPLE_PROTOCOLS
    :desc: The Connix SHALL support communication over TCP, UDP, and Unix
        Domain Sockets.
    :rationale: Eliminates the need for multiple tools across these
        protocols.

Operating Roles
-----------------

.. sw-req:: Operating Roles
    :id: SW_REQ_WORKING_ROLES
    :desc: The Connix SHALL operate in three roles: CLIENT, SERVER, and
        DUAL.
    :rationale: Enables flexible deployment without additional tooling.

Connection Management
-----------------------

.. sw-req:: Connection Management
    :id: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: The Connix SHALL follow a simple connection flow per
        invocation: connect, send one message, receive a response, then
        close.
    :rationale: Keeps the connection model simple; multi-connection
        switching and stateful sessions are out of scope.

Event Handling
---------------

.. sw-req:: Event Handling
    :id: SW_REQ_EVENT_DRIVEN
    :desc: The Connix SHALL support event-driven execution, triggering
        actions on events such as message receipt, connection changes,
        or timers.
    :rationale: Enables reactive automation without requiring manual
        intervention.

Rule Engine
------------

.. sw-req:: Rule Engine
    :id: SW_REQ_RULE_BASED
    :desc: The Connix SHALL allow users to define rules governing
        message sending and receiving behavior.
    :rationale: Reduces manual intervention and supports automation.

Execution Modes
-----------------

.. sw-req:: Execution Modes
    :id: SW_REQ_RUNNING_MODES
    :desc: The Connix SHALL support ONETIME and PERIODIC execution
        modes, both built into Connix without requiring an external
        scheduler.
    :rationale: Allows both single-run and recurring operations from a
        single tool.

Configuration
--------------

.. sw-req:: Configuration
    :id: SW_REQ_CONFIGURATION_FILE
    :desc: The Connix SHALL accept configuration files in JSON and YAML
        formats.
    :rationale: Provides flexibility for defining workflows and
        reproducible setups using common formats.

Traffic Visibility
--------------------

.. sw-req:: Traffic Visibility
    :id: SW_REQ_TRAFFIC_OUTPUT
    :desc: The Connix SHALL print sent and received traffic to the
        console or to a file.
    :rationale: Enables users to observe traffic during development and
        debugging.
