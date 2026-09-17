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
