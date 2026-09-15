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

Fit Criterion: Connix ships a CLI binary invocable from a terminal, and
a GUI binary invocable without command-line arguments, both exposing
the same set of capabilities.

Protocol Support
-----------------

.. sw-req:: Protocol Support
    :id: SW_REQ_MULTIPLE_PROTOCOLS
    :desc: The Connix SHALL support communication over TCP, UDP, and Unix
        Domain Sockets.
    :rationale: Eliminates the need for multiple tools across these
        protocols.

Fit Criterion: Connix can complete a send/receive exchange over a TCP
socket, a UDP socket, and a Unix Domain Socket, using the same
invocation pattern for each.

Operating Roles
-----------------

.. sw-req:: Operating Roles
    :id: SW_REQ_WORKING_ROLES
    :desc: The Connix SHALL operate in three roles: CLIENT, SERVER, and
        DUAL.
    :rationale: Enables flexible deployment without additional tooling.

Fit Criterion: A single Connix binary can be configured to run as
CLIENT, as SERVER, or as DUAL, selectable through configuration or
command-line arguments without a separate build.

Connection Management
-----------------------

.. sw-req:: Connection Management
    :id: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: In CLIENT role, the Connix SHALL follow a connect, send one
        message, receive a response, then close cycle. In SERVER role,
        the Connix SHALL follow an accept, receive a message, send a
        response, then close cycle. In ONETIME execution mode, the
        cycle SHALL run exactly once. In PERIODIC execution mode, or
        when triggered by an event or rule, the Connix SHALL repeat the
        entire cycle on each trigger; it SHALL NOT keep a connection
        open across triggers.
    :rationale: Keeps the connection model simple and consistent across
        roles and execution modes; multi-connection switching and
        stateful sessions spanning multiple cycles are out of scope.

Fit Criterion: In CLIENT role, each configured trigger (a single run in
ONETIME mode, or each interval/event/rule match in PERIODIC mode)
produces exactly one connect-send-receive-close cycle, with no
connection left open afterward. In SERVER role, each trigger produces
exactly one accept-receive-send-close cycle.

Event Handling
---------------

.. sw-req:: Event Handling
    :id: SW_REQ_EVENT_DRIVEN
    :desc: The Connix SHALL support event-driven execution, triggering
        actions on events such as message receipt, connection changes,
        or timers.
    :rationale: Enables reactive automation without requiring manual
        intervention.

Fit Criterion: Configuring an action against a message-receipt,
connection-change, or timer event causes Connix to run that action
when the event occurs, with no manual step in between.

Rule Engine
------------

.. sw-req:: Rule Engine
    :id: SW_REQ_RULE_BASED
    :desc: The Connix SHALL allow users to define rules governing
        message sending and receiving behavior.
    :rationale: Reduces manual intervention and supports automation.

Fit Criterion: A user-defined rule that matches a specific message
condition causes Connix to perform the configured send or receive
action, without additional manual input.

Execution Modes
-----------------

.. sw-req:: Execution Modes
    :id: SW_REQ_RUNNING_MODES
    :desc: The Connix SHALL support ONETIME and PERIODIC execution
        modes, both built into Connix without requiring an external
        scheduler.
    :rationale: Allows both single-run and recurring operations from a
        single tool.

Fit Criterion: Connix run in ONETIME mode performs its configured
action exactly once and exits. Connix run in PERIODIC mode repeats its
configured action on the configured interval without an external
scheduler.

Configuration
--------------

.. sw-req:: Configuration
    :id: SW_REQ_CONFIGURATION_FILE
    :desc: The Connix SHALL accept configuration files in JSON and YAML
        formats.
    :rationale: Provides flexibility for defining workflows and
        reproducible setups using common formats.

Fit Criterion: Connix accepts a configuration file written in JSON and
an equivalent configuration file written in YAML, producing the same
behavior from both.

Traffic Visibility
--------------------

.. sw-req:: Traffic Visibility
    :id: SW_REQ_TRAFFIC_OUTPUT
    :desc: The Connix SHALL print sent and received traffic, and its own
        log output, to the console (stdout) by default. The Connix
        SHALL allow the user to redirect this output to a file.
    :rationale: Enables users to observe traffic and diagnostic
        messages during development and debugging, on the console by
        default, with file output available for a persistent record.

Fit Criterion: Running Connix without any output configuration prints
both traffic and log messages to the console. A configuration option
or command-line flag redirects that same output to a file instead.
