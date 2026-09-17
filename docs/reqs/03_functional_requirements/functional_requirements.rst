Functional Requirements
=======================

User Interfaces
----------------

.. sw-req:: User Interfaces
    :id: SW_REQ_USER_INTERFACES
    :desc: The Connix SHALL provide user interfaces through a CLI and, when
        enabled, a GUI. The atomic interface behaviors are specified by the
        linked child requirements below.
    :rationale: Supports scripting-oriented and visual interaction workflows
        without forcing every build or release to include the GUI.

.. sw-req:: CLI Provision
    :id: SW_REQ_USER_INTERFACES_CLI
    :links: SW_REQ_USER_INTERFACES
    :desc: The Connix SHALL provide a Command-Line Interface (CLI).
    :rationale: Supports scripting-oriented and terminal-based workflows.

Fit Criterion: Connix ships a CLI binary that is invocable from a terminal.

.. sw-req:: GUI Provision
    :id: SW_REQ_USER_INTERFACES_GUI
    :links: SW_REQ_USER_INTERFACES
    :desc: Connix SHALL provide the GUI behaviors defined by the linked child
        requirements when GUI support is selected.
    :rationale: Keeps GUI provision and packaging decisions independently
        testable.

.. sw-req:: GUI Provision When Enabled
    :id: SW_REQ_USER_INTERFACES_GUI_PROVISION
    :links: SW_REQ_USER_INTERFACES_GUI
    :desc: Connix SHALL produce a GUI binary when the GUI build option is
        enabled.
    :rationale: Provides the visual interface when requested at build time.

Fit Criterion: A build with the GUI option enabled produces a GUI binary.

.. sw-req:: GUI Optional Build
    :id: SW_REQ_USER_INTERFACES_GUI_OPTIONAL_BUILD
    :links: SW_REQ_USER_INTERFACES_GUI
    :desc: Connix SHALL allow the GUI build option to be disabled without
        preventing a compliant CLI build.
    :rationale: Supports headless CI/CD builds.

Fit Criterion: A build with the GUI option disabled remains compliant and
produces the CLI without a GUI binary.

.. sw-req:: GUI Optional Distribution
    :id: SW_REQ_USER_INTERFACES_GUI_OPTIONAL_DISTRIBUTION
    :links: SW_REQ_USER_INTERFACES_GUI
    :desc: Connix SHALL allow a release to omit the GUI binary when the release
        is intended to be headless.
    :rationale: Allows release packaging to target headless environments.

Fit Criterion: A headless release omits the GUI binary while remaining
compliant. GUI/CLI capability parity is specified by
``SW_REQ_CLI_GUI_CONSISTENCY``.

Protocol Support
-----------------

.. sw-req:: Protocol Support
    :id: SW_REQ_MULTIPLE_PROTOCOLS
    :desc: Connix SHALL provide the protocol behaviors defined by its linked
        protocol requirements.
    :rationale: Keeps protocol support traceable as separate behaviors.

.. sw-req:: TCP Protocol Support
    :id: SW_REQ_PROTOCOL_TCP
    :links: SW_REQ_MULTIPLE_PROTOCOLS
    :desc: Connix SHALL complete a TCP send/receive exchange when configured
        for TCP, for the duration of the exchange.
    :rationale: Supports stream-oriented network communication.

Fit Criterion: A TCP configuration completes a send/receive exchange.

.. sw-req:: UDP Protocol Support
    :id: SW_REQ_PROTOCOL_UDP
    :links: SW_REQ_MULTIPLE_PROTOCOLS
    :desc: Connix SHALL complete a UDP datagram send/receive exchange when
        configured for UDP, for the duration of the exchange.
    :rationale: Supports datagram-oriented network communication.

Fit Criterion: A UDP configuration completes a datagram exchange.

.. sw-req:: UDS Protocol Support
    :id: SW_REQ_PROTOCOL_UDS
    :links: SW_REQ_MULTIPLE_PROTOCOLS
    :desc: Connix SHALL complete a Unix Domain Socket send/receive exchange
        when configured for UDS, for the duration of the exchange.
    :rationale: Supports local inter-process communication.

Fit Criterion: A UDS configuration completes a send/receive exchange.

Operating Roles
-----------------

.. sw-req:: Operating Roles
    :id: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL provide the CLIENT, SERVER, and DUAL role behaviors
        defined by the linked child requirements.
    :rationale: Enables flexible deployment without additional tooling.

.. sw-req:: CLIENT Role
    :id: SW_REQ_ROLE_CLIENT
    :links: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL operate as a CLIENT when the CLIENT role is selected
        by configuration or command-line option, for the configured run.
    :rationale: Enables outbound connection initiation.

Fit Criterion: Selecting CLIENT starts CLIENT behavior without a separate build.

.. sw-req:: SERVER Role
    :id: SW_REQ_ROLE_SERVER
    :links: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL operate as a SERVER when the SERVER role is selected
        by configuration or command-line option, for the configured run.
    :rationale: Enables inbound connection acceptance.

Fit Criterion: Selecting SERVER starts SERVER behavior without a separate build.

.. sw-req:: DUAL Role
    :id: SW_REQ_ROLE_DUAL
    :links: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL operate as DUAL when the DUAL role is selected by
        configuration or command-line option, for the configured run.
    :rationale: Enables simultaneous client and server operation.

Fit Criterion: Selecting DUAL starts both DUAL legs without a separate build.

Connection Management
-----------------------

.. sw-req:: Connection Management
    :id: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: The Connix SHALL provide role- and trigger-specific connection
        cycles as defined by the linked child requirements below.
    :rationale: Keeps connection behavior simple and independently testable
        across roles and execution modes.

.. sw-req:: CLIENT Connection Cycle
    :id: SW_REQ_CONNECTION_FLOW_CLIENT
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: Connix SHALL provide the CLIENT connection behaviors defined by the
        linked child requirements.
    :rationale: Keeps CLIENT lifecycle and timeout behaviors independently
        testable.

.. sw-req:: CLIENT Connection Lifecycle
    :id: SW_REQ_CONNECTION_FLOW_CLIENT_LIFECYCLE
    :links: SW_REQ_CONNECTION_FLOW_CLIENT
    :desc: Connix SHALL complete one connect, send one message, receive, and
        close sequence under CLIENT role when a configured trigger occurs.
    :rationale: Defines the basic outbound request lifecycle.

Fit Criterion: Each CLIENT cycle performs connect, one send, receive, and
close in that order, with no connection left open afterward.

.. sw-req:: CLIENT Connect Timeout
    :id: SW_REQ_CONNECTION_FLOW_CLIENT_CONNECT_TIMEOUT
    :links: SW_REQ_CONNECTION_FLOW_CLIENT
    :desc: Connix SHALL complete or time out the CLIENT connect operation within
        5 seconds after a CLIENT cycle starts.
    :rationale: Prevents an unavailable peer from blocking indefinitely.

Fit Criterion: A CLIENT connect that cannot complete is timed out after 5
seconds.

.. sw-req:: CLIENT Send Timeout
    :id: SW_REQ_CONNECTION_FLOW_CLIENT_SEND_TIMEOUT
    :links: SW_REQ_CONNECTION_FLOW_CLIENT
    :desc: Connix SHALL complete or time out the CLIENT send operation within
        5 seconds after the CLIENT connection is established.
    :rationale: Bounds a blocked outbound operation.

Fit Criterion: A CLIENT send that cannot complete is timed out after 5 seconds.

.. sw-req:: CLIENT Receive Timeout
    :id: SW_REQ_CONNECTION_FLOW_CLIENT_RECEIVE_TIMEOUT
    :links: SW_REQ_CONNECTION_FLOW_CLIENT
    :desc: Connix SHALL complete or time out the CLIENT receive operation within
        10 seconds after the CLIENT message is sent.
    :rationale: Bounds waiting for a response.

Fit Criterion: A CLIENT receive without a response is timed out after 10
seconds.

.. sw-req:: CLIENT Timeout Result
    :id: SW_REQ_CONNECTION_FLOW_CLIENT_TIMEOUT_RESULT
    :links: SW_REQ_CONNECTION_FLOW_CLIENT
    :desc: Connix SHALL report the operation-specific timeout, close the
        affected CLIENT socket, and apply the mode-specific timeout result.
    :rationale: Makes CLIENT timeout behavior deterministic.

.. sw-req:: SERVER Connection Cycle
    :id: SW_REQ_CONNECTION_FLOW_SERVER
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: Connix SHALL provide the SERVER connection behaviors defined by the
        linked child requirements.
    :rationale: Keeps SERVER lifecycle and timeout behaviors independently
        testable.

.. sw-req:: SERVER Connection Lifecycle
    :id: SW_REQ_CONNECTION_FLOW_SERVER_LIFECYCLE
    :links: SW_REQ_CONNECTION_FLOW_SERVER
    :desc: Connix SHALL complete one accept, receive, send, and close sequence
        under SERVER role when a client connection is accepted.
    :rationale: Defines the basic inbound request lifecycle.

Fit Criterion: Each SERVER cycle performs accept, receive, send, and close in
that order, with no connection left open afterward.

.. sw-req:: SERVER Accept Timeout
    :id: SW_REQ_CONNECTION_FLOW_SERVER_ACCEPT_TIMEOUT
    :links: SW_REQ_CONNECTION_FLOW_SERVER
    :desc: Connix SHALL complete or time out the SERVER accept operation within
        5 seconds after a SERVER cycle starts waiting for a client.
    :rationale: Prevents an unavailable client from blocking indefinitely.

Fit Criterion: A SERVER accept without a client is timed out after 5 seconds.

.. sw-req:: SERVER Receive Timeout
    :id: SW_REQ_CONNECTION_FLOW_SERVER_RECEIVE_TIMEOUT
    :links: SW_REQ_CONNECTION_FLOW_SERVER
    :desc: Connix SHALL complete or time out the SERVER receive operation within
        10 seconds after the SERVER connection is accepted.
    :rationale: Bounds waiting for an inbound message.

Fit Criterion: A SERVER receive without a message is timed out after 10
seconds.

.. sw-req:: SERVER Send Timeout
    :id: SW_REQ_CONNECTION_FLOW_SERVER_SEND_TIMEOUT
    :links: SW_REQ_CONNECTION_FLOW_SERVER
    :desc: Connix SHALL complete or time out the SERVER send operation within
        5 seconds after the SERVER message is received.
    :rationale: Bounds a blocked outbound response.

Fit Criterion: A SERVER send that cannot complete is timed out after 5 seconds.

.. sw-req:: SERVER Timeout Result
    :id: SW_REQ_CONNECTION_FLOW_SERVER_TIMEOUT_RESULT
    :links: SW_REQ_CONNECTION_FLOW_SERVER
    :desc: Connix SHALL report the operation-specific timeout, close the
        affected SERVER socket, and apply the mode-specific timeout result.
    :rationale: Makes SERVER timeout behavior deterministic.

.. sw-req:: Connection Operation Timeouts
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUTS
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: Connix SHALL provide the connection timeout behaviors defined by the
        linked child requirements.
    :rationale: Keeps operation bounds and socket cleanup independently testable.

.. sw-req:: Connect and Accept Timeout Bounds
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_CONNECT_ACCEPT
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUTS
    :desc: Connix SHALL time out a connect or accept operation after 5 seconds
        when the operation has not completed.
    :rationale: Prevents a failed peer from blocking a cycle indefinitely.

Fit Criterion: An incomplete connect or accept operation is timed out after 5
seconds.

.. sw-req:: Receive Timeout Bound
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_RECEIVE
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUTS
    :desc: Connix SHALL time out a receive operation after 10 seconds when no
        message has been received.
    :rationale: Bounds waiting for a response or inbound message.

Fit Criterion: A receive operation without a message is timed out after 10
seconds.

.. sw-req:: Send Timeout Bound
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_SEND
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUTS
    :desc: Connix SHALL time out a send operation after 5 seconds when the
        operation has not completed.
    :rationale: Bounds blocked outbound traffic.

Fit Criterion: An incomplete send operation is timed out after 5 seconds.

.. sw-req:: Timeout Socket Closure
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_SOCKET_CLOSURE
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUTS
    :desc: Connix SHALL close the affected socket promptly after any connection
        operation timeout.
    :rationale: Prevents timed-out resources from remaining open.

Fit Criterion: Socket inspection shows the affected socket is closed after a
timeout.

.. sw-req:: Timeout Failure Handling
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUTS
    :desc: Connix SHALL provide the timeout outcomes defined by the linked child
        requirements.
    :rationale: Separates timeout reporting from mode-specific outcomes.

.. sw-req:: Timeout Error Reporting
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_REPORT
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT
    :desc: Connix SHALL report an operation-specific timeout error when a
        connection operation exceeds its bound.
    :rationale: Makes timeout diagnosis deterministic.

Fit Criterion: The error identifies the timed-out operation.

.. sw-req:: ONETIME Timeout Outcome
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_ONETIME
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT
    :desc: Connix SHALL exit non-zero after a timeout during an ONETIME run.
    :rationale: Makes single-run failure observable to the caller.

Fit Criterion: A timed-out ONETIME run exits non-zero.

.. sw-req:: PERIODIC Timeout Outcome
    :id: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_PERIODIC
    :links: SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT
    :desc: Connix SHALL continue future eligible PERIODIC triggers after a
        timed-out cycle has closed.
    :rationale: Keeps recurring execution available after an isolated failure.

Fit Criterion: A later eligible PERIODIC trigger starts after a timed-out cycle.

.. sw-req:: DUAL Connection Cycles
    :id: SW_REQ_CONNECTION_FLOW_DUAL
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: In DUAL role, the Connix SHALL provide the separate CLIENT and
        SERVER behaviors defined by the linked child requirements.
    :rationale: Provides simultaneous client and server operation while keeping
        each DUAL behavior independently testable.

.. sw-req:: DUAL Leg Presence
    :id: SW_REQ_CONNECTION_FLOW_DUAL_LEGS
    :links: SW_REQ_CONNECTION_FLOW_DUAL
    :desc: For each DUAL trigger, the Connix SHALL start one CLIENT-role cycle
        and one SERVER-role cycle.
    :rationale: Defines the two operations that DUAL mode provides.

Fit Criterion: One CLIENT-role cycle and one SERVER-role cycle are observable
for every DUAL trigger.

.. sw-req:: DUAL Concurrent Execution
    :id: SW_REQ_CONNECTION_FLOW_DUAL_CONCURRENCY
    :links: SW_REQ_CONNECTION_FLOW_DUAL
    :desc: Connix SHALL execute the CLIENT-role and SERVER-role cycles started
        by a DUAL trigger concurrently for the duration of both cycles.
    :rationale: Allows both DUAL legs to make progress during the same trigger.

Fit Criterion: Timing observations show that both DUAL legs can be active at
the same time rather than one waiting for the other to finish.

.. sw-req:: DUAL Socket Isolation
    :id: SW_REQ_CONNECTION_FLOW_DUAL_SOCKET_ISOLATION
    :links: SW_REQ_CONNECTION_FLOW_DUAL
    :desc: Connix SHALL assign separate sockets to the CLIENT-role and
        SERVER-role cycles started by a DUAL trigger for the duration of both
        cycles.
    :rationale: Prevents state and I/O from one leg being mixed with the other.

Fit Criterion: Socket inspection identifies distinct sockets for the CLIENT
and SERVER legs of the same DUAL trigger.

.. sw-req:: DUAL Failure Isolation
    :id: SW_REQ_CONNECTION_FLOW_DUAL_FAILURE_ISOLATION
    :links: SW_REQ_CONNECTION_FLOW_DUAL
    :desc: Connix SHALL allow the unaffected DUAL leg to continue when the
        other DUAL leg fails, for the duration of the unaffected cycle.
    :rationale: Keeps the two role behaviors independent when one peer fails.

Fit Criterion: If one peer is unavailable, the other DUAL leg still reaches
its normal completion or error handling independently.

.. sw-req:: DUAL Independent Closure
    :id: SW_REQ_CONNECTION_FLOW_DUAL_INDEPENDENT_CLOSURE
    :links: SW_REQ_CONNECTION_FLOW_DUAL
    :desc: Connix SHALL close each DUAL leg's socket when that leg's own
        cycle completes, without waiting for the other leg.
    :rationale: Prevents one leg's lifetime from keeping the other socket open.

Fit Criterion: Completion of either DUAL leg closes that leg's socket without
waiting for the other leg, and both sockets are closed after both cycles finish.

.. sw-req:: ONETIME Connection Execution
    :id: SW_REQ_CONNECTION_FLOW_ONETIME
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: Connix SHALL create one connection cycle for the action executed by
        ``SW_REQ_RUNNING_MODES_ONETIME`` and SHALL not create an additional
        cycle for that action.
    :rationale: Connects the ONETIME scheduling rule to the connection lifecycle
        without duplicating its execution semantics.

Fit Criterion: The ONETIME action creates one configured connection cycle and
no additional cycle.

.. sw-req:: Per-Trigger Connection Execution
    :id: SW_REQ_CONNECTION_FLOW_PER_TRIGGER
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: Connix SHALL provide the per-trigger behaviors defined by the linked
        child requirements.
    :rationale: Keeps trigger start, closure, and overlap behavior independent.

.. sw-req:: Trigger Starts Cycle
    :id: SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_START
    :links: SW_REQ_CONNECTION_FLOW_PER_TRIGGER
    :desc: Connix SHALL start one complete connection cycle when an eligible
        PERIODIC, event, or rule trigger occurs.
    :rationale: Ensures each eligible trigger produces a cycle.

Fit Criterion: Every eligible interval, event, or rule match starts one
complete connection cycle.

.. sw-req:: Trigger Cycle Closure
    :id: SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_CLOSURE
    :links: SW_REQ_CONNECTION_FLOW_PER_TRIGGER
    :desc: Connix SHALL close a triggered connection cycle before starting the
        next eligible cycle.
    :rationale: Prevents connections from persisting across cycles.

Fit Criterion: No connection from a completed cycle remains open before the
next cycle starts.

.. sw-req:: PERIODIC No Overlap
    :id: SW_REQ_CONNECTION_FLOW_PER_TRIGGER_PERIODIC_NO_OVERLAP
    :links: SW_REQ_CONNECTION_FLOW_PER_TRIGGER
    :desc: Connix SHALL skip a PERIODIC trigger when another PERIODIC cycle is
        still active.
    :rationale: Prevents unbounded overlapping cycles and socket accumulation.

Fit Criterion: An interval firing during an active PERIODIC cycle does not
start an additional cycle.

.. sw-req:: Single Send Per Cycle
    :id: SW_REQ_CONNECTION_FLOW_SINGLE_SEND
    :links: SW_REQ_SIMPLE_CONNECTION_FLOW
    :desc: Connix SHALL provide the single-send behaviors defined by the linked
        child requirements.
    :rationale: Keeps the one-message CLIENT cycle deterministic.

.. sw-req:: Single Send Configuration Rejection
    :id: SW_REQ_CONNECTION_FLOW_SINGLE_SEND_REJECTION
    :links: SW_REQ_CONNECTION_FLOW_SINGLE_SEND
    :desc: Connix SHALL reject a configuration at configuration-load time when
        it would cause more than one send action within one connection cycle.
    :rationale: Prevents undefined rule ordering from changing cycle behavior.

Fit Criterion: A configuration defining multiple sends for one cycle is
rejected before any configured action occurs.

.. sw-req:: Single Send Error Detail
    :id: SW_REQ_CONNECTION_FLOW_SINGLE_SEND_ERROR_DETAIL
    :links: SW_REQ_CONNECTION_FLOW_SINGLE_SEND
    :desc: Connix SHALL identify the conflicting rules in the error reported
        for a multiple-send configuration.
    :rationale: Makes configuration correction possible.

Event Handling
---------------

.. sw-req:: Event Handling
    :id: SW_REQ_EVENT_DRIVEN
    :desc: The Connix SHALL support event-driven execution through the event
        kinds and cycle scope defined by the linked child requirements below.
    :rationale: Enables reactive automation with one consistent triggering
        model.

.. sw-req:: Event Kinds
    :id: SW_REQ_EVENT_DRIVEN_KINDS
    :links: SW_REQ_EVENT_DRIVEN
    :desc: Connix SHALL recognize the event kinds defined by the linked child
        requirements when their corresponding input or connection state occurs.
    :rationale: Keeps each event source independently specified and testable.

Fit Criterion: A rule can be configured against each supported event kind and
fires when that event occurs.

.. sw-req:: Message Receipt Event
    :id: SW_REQ_EVENT_MESSAGE_RECEIPT
    :links: SW_REQ_EVENT_DRIVEN_KINDS
    :desc: Connix SHALL recognize a message-receipt event when a configured
        connection receives a message, for the duration of that connection.
    :rationale: Enables rules triggered by received data.

Fit Criterion: A rule bound to message receipt fires after the configured
message is received.

.. sw-req:: Connect Event
    :id: SW_REQ_EVENT_CONNECT
    :links: SW_REQ_EVENT_DRIVEN_KINDS
    :desc: Connix SHALL recognize a connect event when a CLIENT connection is
        established, at the start of that connection cycle.
    :rationale: Enables rules triggered by outbound connection establishment.

Fit Criterion: A rule bound to connect fires when the CLIENT connection is
established.

.. sw-req:: Accept Event
    :id: SW_REQ_EVENT_ACCEPT
    :links: SW_REQ_EVENT_DRIVEN_KINDS
    :desc: Connix SHALL recognize an accept event when a SERVER connection is
        accepted, at the start of that connection cycle.
    :rationale: Enables rules triggered by inbound connection establishment.

Fit Criterion: A rule bound to accept fires when the SERVER accepts a
connection.

.. sw-req:: Close Event
    :id: SW_REQ_EVENT_CLOSE
    :links: SW_REQ_EVENT_DRIVEN_KINDS
    :desc: Connix SHALL recognize a close event when a connection cycle closes,
        at the end of that cycle.
    :rationale: Enables rules triggered by connection completion.

Fit Criterion: A rule bound to close fires when the connection cycle closes.

.. sw-req:: Timer Event
    :id: SW_REQ_EVENT_TIMER
    :links: SW_REQ_EVENT_DRIVEN_KINDS
    :desc: Connix SHALL recognize a timer event when a configured elapsed-time
        interval expires, for the duration of the configured execution mode.
    :rationale: Enables time-based automation.

Fit Criterion: A rule bound to a timer fires when its configured interval
expires.

.. sw-req:: PERIODIC Timer Event Source
    :id: SW_REQ_EVENT_DRIVEN_TIMER_SOURCE
    :links: SW_REQ_EVENT_DRIVEN
    :desc: Connix SHALL use the event-driven timer mechanism for the interval
        configured by PERIODIC mode, whenever a PERIODIC interval expires.
    :rationale: Prevents inconsistent duplicate scheduling mechanisms.

Fit Criterion: A PERIODIC interval produces the same timer event used by
timer-based rules.

.. sw-req:: Connection Event Scope
    :id: SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE
    :links: SW_REQ_EVENT_DRIVEN
    :desc: Connix SHALL expose only the cycle-start and cycle-close connection
        events during one connect/send/receive/close cycle.
    :rationale: Makes event ordering and cycle boundaries deterministic.

Fit Criterion: Tests observe only the start and close connection-state events
within a single cycle.

Rule Engine
------------

.. sw-req:: Rule Engine
    :id: SW_REQ_RULE_BASED
    :desc: The Connix SHALL support rules that pair triggering events with
        actions and operate within the connection cycle that they trigger.
    :rationale: Reduces manual intervention while keeping connection lifetime
        controlled by the connection-flow requirements.

.. sw-req:: Rule Definition
    :id: SW_REQ_RULE_BASED_DEFINITION
    :links: SW_REQ_RULE_BASED
    :desc: Connix SHALL accept a rule definition containing one triggering
        event and one send or receive action when the configuration is loaded.
    :rationale: Provides a simple, predictable automation unit.

Fit Criterion: A user-defined rule contains exactly one supported event and
one send or receive action.

.. sw-req:: Rule Cycle Scope
    :id: SW_REQ_RULE_BASED_CYCLE_SCOPE
    :links: SW_REQ_RULE_BASED
    :desc: Connix SHALL execute a matching rule's action only during the
        connect/send/receive/close cycle that triggered it and SHALL NOT chain
        the action into a later cycle.
    :rationale: Prevents rules from extending connection lifetime across
        triggers.

Fit Criterion: A matching rule performs its action only within the current
cycle and leaves no action or open connection pending for a later cycle.

Execution Modes
----------------

.. sw-req:: Execution Modes
    :id: SW_REQ_RUNNING_MODES
    :desc: The Connix SHALL support built-in ONETIME and PERIODIC execution
        modes as defined by the linked child requirements below.
    :rationale: Supports both single-run and recurring operation without
        requiring separate tools.

.. sw-req:: ONETIME Mode
    :id: SW_REQ_RUNNING_MODES_ONETIME
    :links: SW_REQ_RUNNING_MODES
    :desc: Connix SHALL perform the configured action once under ONETIME mode
        when the invocation starts, then exit after that action completes.
    :rationale: Supports deterministic single-run operation.

Fit Criterion: A Connix run in ONETIME mode performs its configured action
once and exits.

.. sw-req:: PERIODIC Mode
    :id: SW_REQ_RUNNING_MODES_PERIODIC
    :links: SW_REQ_RUNNING_MODES
    :desc: Connix SHALL perform the configured action at each configured
        PERIODIC interval for the duration of the PERIODIC run, subject to the
        no-overlap rule in ``SW_REQ_CONNECTION_FLOW_PER_TRIGGER``.
    :rationale: Supports recurring operation with bounded trigger behavior.

Fit Criterion: A Connix run in PERIODIC mode repeats its configured action at
the configured interval.

.. sw-req:: Built-in Execution Scheduling
    :id: SW_REQ_RUNNING_MODES_BUILT_IN
    :links: SW_REQ_RUNNING_MODES
    :desc: Connix SHALL provide ONETIME and PERIODIC scheduling internally when
        either mode is selected, without requiring an external scheduler for
        the duration of the run.
    :rationale: Keeps execution behavior portable and self-contained.

Fit Criterion: Both modes run with Connix alone and do not require a separate
scheduler process. The PERIODIC timer-event relationship is specified by
``SW_REQ_EVENT_DRIVEN_TIMER_SOURCE``.

Configuration
--------------

.. sw-req:: Configuration
    :id: SW_REQ_CONFIGURATION_FILE
    :desc: The Connix SHALL provide deterministic configuration-file and
        command-line configuration behavior through the linked child
        requirements below.
    :rationale: Supports reproducible workflows while making configuration
        interpretation deterministic.

.. sw-req:: Configuration Formats
    :id: SW_REQ_CONFIGURATION_FORMATS
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Connix SHALL accept the configuration formats defined by the linked
        child requirements.
    :rationale: Supports common configuration formats as separate behaviors.

.. sw-req:: JSON Configuration
    :id: SW_REQ_CONFIGURATION_JSON
    :links: SW_REQ_CONFIGURATION_FORMATS
    :desc: Connix SHALL accept a JSON configuration when the input is selected
        as JSON, for the duration of configuration loading.
    :rationale: Supports JSON-based configuration.

Fit Criterion: Connix accepts a valid JSON configuration and applies it.

.. sw-req:: YAML Configuration
    :id: SW_REQ_CONFIGURATION_YAML
    :links: SW_REQ_CONFIGURATION_FORMATS
    :desc: Connix SHALL accept a YAML configuration when the input is selected
        as YAML, for the duration of configuration loading.
    :rationale: Supports YAML-based configuration.

Fit Criterion: Connix accepts a valid YAML configuration and applies it.

.. sw-req:: Extension-Based Parser Selection
    :id: SW_REQ_CONFIGURATION_PARSER_SELECTION
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Connix SHALL provide the parser-selection behaviors defined by the
        linked child requirements.
    :rationale: Makes parser selection independently testable.

.. sw-req:: Extension Parser Mapping
    :id: SW_REQ_CONFIGURATION_PARSER_SELECTION_EXTENSION_MAP
    :links: SW_REQ_CONFIGURATION_PARSER_SELECTION
    :desc: Connix SHALL select JSON for ``.json`` files and YAML for ``.yaml``
        or ``.yml`` files when loading configuration.
    :rationale: Makes supported extension mapping deterministic.

Fit Criterion: Each supported extension selects its specified parser without
content sniffing.

.. sw-req:: Unsupported Extension Rejection
    :id: SW_REQ_CONFIGURATION_PARSER_SELECTION_UNSUPPORTED_EXTENSION
    :links: SW_REQ_CONFIGURATION_PARSER_SELECTION
    :desc: Connix SHALL reject a configuration file with an unsupported or
        missing extension unless an explicit format is supplied.
    :rationale: Prevents ambiguous parser selection.

Fit Criterion: An unsupported or extensionless file is rejected unless an
explicit format is supplied.

.. sw-req:: Configuration Parse Errors
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Connix SHALL provide the parse-error behaviors defined by the linked
        child requirements when the selected parser fails.
    :rationale: Keeps diagnostics, abort behavior, and parser fallback separate.

.. sw-req:: Parse Error Reporting
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR_REPORT
    :links: SW_REQ_CONFIGURATION_PARSE_ERROR
    :desc: Connix SHALL report a parse error identifying the configuration file
        and the reason when the selected parser fails.
    :rationale: Makes configuration failures diagnosable.

Fit Criterion: The parse error identifies the file and reason.

.. sw-req:: Parse Error Abort
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR_ABORT
    :links: SW_REQ_CONFIGURATION_PARSE_ERROR
    :desc: Connix SHALL exit non-zero without performing configured actions when
        the selected parser fails.
    :rationale: Prevents invalid configuration from starting execution.

Fit Criterion: Invalid content causes a non-zero exit and no configured action.

.. sw-req:: Parse Error No Fallback
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR_NO_FALLBACK
    :links: SW_REQ_CONFIGURATION_PARSE_ERROR
    :desc: Connix SHALL NOT attempt another parser after the selected parser
        fails.
    :rationale: Preserves extension-based parser selection.

Fit Criterion: An invalid JSON file does not trigger a YAML parsing attempt,
and vice versa.

.. sw-req:: Explicit Format for Extensionless Configuration
    :id: SW_REQ_CONFIGURATION_EXPLICIT_FORMAT
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Configuration supplied through stdin or another extensionless
        source SHALL require an explicit ``--format`` flag naming ``json`` or
        ``yaml``.
    :rationale: Ensures extensionless input is still interpreted deterministically.

Fit Criterion: Extensionless configuration without ``--format`` is rejected;
with ``--format json`` or ``--format yaml`` it is parsed accordingly.

.. sw-req:: CLI Configuration Precedence
    :id: SW_REQ_CONFIGURATION_CLI_PRECEDENCE
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: When a setting is specified both in a configuration file and via a
        command-line flag, the command-line value SHALL take precedence.
    :rationale: Allows one-run overrides without editing the configuration file.

Fit Criterion: For conflicting file and command-line values, Connix uses the
command-line value.

Traffic Visibility
--------------------

.. sw-req:: Traffic Visibility
    :id: SW_REQ_TRAFFIC_OUTPUT
    :desc: The Connix SHALL provide traffic and log output with the default
        destination and redirection behavior defined by the linked children.
    :rationale: Enables observation and persistent recording of traffic and
        diagnostics.

.. sw-req:: Sent Traffic Output
    :id: SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL output sent traffic when a send action completes during
        a connection cycle.
    :rationale: Makes outbound communication observable.

Fit Criterion: Completed sent messages are present in the configured output.

.. sw-req:: Received Traffic Output
    :id: SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL output received traffic when a receive action completes
        during a connection cycle.
    :rationale: Makes inbound communication observable.

Fit Criterion: Completed received messages are present in the configured output.

.. sw-req:: Default Standard Output
    :id: SW_REQ_TRAFFIC_OUTPUT_STDOUT_DEFAULT
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL direct sent traffic, received traffic, and log output
        to stdout by default when no output-file option is configured.
    :rationale: Provides immediate visibility during normal CLI use.

Fit Criterion: Without output-file configuration, traffic and log output appear
on stdout.

.. sw-req:: Log Output
    :id: SW_REQ_TRAFFIC_OUTPUT_LOG
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL output its own log messages when a diagnostic event
        occurs during a run.
    :rationale: Makes diagnostics observable during normal use.

Fit Criterion: Connix diagnostic messages are present in the configured output.
Log format, levels, and verbosity remain implementation-defined.

.. sw-req:: File Output Redirection
    :id: SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL provide the file-redirection behaviors defined by the
        linked child requirements when an output file is configured.
    :rationale: Keeps file destination and stdout suppression independently
        testable.

.. sw-req:: File Output Destination
    :id: SW_REQ_TRAFFIC_OUTPUT_FILE_DESTINATION
    :links: SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
    :desc: Connix SHALL write sent traffic, received traffic, and log output to
        the configured file during the run.
    :rationale: Provides a persistent output record.

Fit Criterion: The configured file contains the redirected traffic and log
output.

.. sw-req:: Redirected Stdout Suppression
    :id: SW_REQ_TRAFFIC_OUTPUT_FILE_STDOUT_SUPPRESSION
    :links: SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
    :desc: Connix SHALL suppress the default stdout copy of sent traffic,
        received traffic, and log output while file redirection is configured.
    :rationale: Prevents redirected output from being duplicated to stdout.

Fit Criterion: With file redirection enabled, stdout contains none of the
redirected traffic or log output.
