.. sw-req:: Connection Management
    :id: [$SW_REQ CONNECTION_MANAGEMENT]
    :desc: For each trigger, Connix SHALL run a bounded connection cycle appropriate
           to the configured role and SHALL close every socket before the cycle
           ends.

.. sw-req:: Connection Management Client
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CLIENT]
    :desc: In CLIENT role, Connix SHALL perform one connect, send, receive, and close
           sequence per trigger.

.. sw-req:: Connection Management Client Lifecycle
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CLIENT_LIFECYCLE]
    :desc: Connix SHALL complete one connect, send one message, receive, and
           close sequence under CLIENT role when a configured trigger occurs.

.. sw-req:: Connection Management Client Connect Timeout
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CLIENT_CONNECT_TIMEOUT]
    :desc: Connix SHALL complete or time out the CLIENT connect operation within
           5 seconds after a CLIENT cycle starts.

.. sw-req:: Connection Management Client Send Timeout
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CLIENT_SEND_TIMEOUT]
    :desc: Connix SHALL complete or time out the CLIENT send operation within
           5 seconds after the CLIENT connection is established.

.. sw-req:: Connection Management Client Receive Timeout
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CLIENT_RECEIVE_TIMEOUT]
    :desc: Connix SHALL complete or time out the CLIENT receive operation within
           10 seconds after the CLIENT message is sent.

.. sw-req:: Connection Management Client Timeout Result
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CLIENT_TIMEOUT_RESULT]
    :desc: Connix SHALL report the operation-specific timeout, close the
           affected CLIENT socket, and apply the mode-specific timeout result.

.. sw-req:: Connection Management Server
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SERVER]
    :desc: In SERVER role, Connix SHALL perform one accept, receive, send, and close
           sequence per accepted connection.

.. sw-req:: Connection Management Server Lifecycle
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SERVER_LIFECYCLE]
    :desc: Connix SHALL complete one accept, receive, send, and close sequence
           under SERVER role when a client connection is accepted.

.. sw-req:: Connection Management Server Accept Timeout
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SERVER_ACCEPT_TIMEOUT]
    :desc: Connix SHALL complete or time out the SERVER accept operation within
           5 seconds after a SERVER cycle starts waiting for a client.

.. sw-req:: Connection Management Server Receive Timeout
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SERVER_RECEIVE_TIMEOUT]
    :desc: Connix SHALL complete or time out the SERVER receive operation within
           10 seconds after the SERVER connection is accepted.

.. sw-req:: Connection Management Server Send Timeout
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SERVER_SEND_TIMEOUT]
    :desc: Connix SHALL complete or time out the SERVER send operation within
           5 seconds after the SERVER message is received.

.. sw-req:: Connection Management Server Timeout Result
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SERVER_TIMEOUT_RESULT]
    :desc: Connix SHALL report the operation-specific timeout, close the
           affected SERVER socket, and apply the mode-specific timeout result.

.. sw-req:: Connection Management Operation Timeouts
    :id: [$SW_REQ CONNECTION_MANAGEMENT_OPERATION_TIMEOUTS]
    :desc: Connix SHALL bound connect, accept, and send operations by 5 seconds and
           receive operations by 10 seconds, and SHALL close the affected socket
           after a timeout.

.. sw-req:: Connection Management Connect and Accept Timeout Bounds
    :id: [$SW_REQ CONNECTION_MANAGEMENT_CONNECT_AND_ACCEPT_TIMEOUT_BOUNDS]
    :desc: Connix SHALL time out a connect or accept operation after 5 seconds
           when the operation has not completed.

.. sw-req:: Connection Management Receive Timeout Bound
    :id: [$SW_REQ CONNECTION_MANAGEMENT_RECEIVE_TIMEOUT_BOUND]
    :desc: Connix SHALL time out a receive operation after 10 seconds when no
           message has been received.

.. sw-req:: Connection Management Send Timeout Bound
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SEND_TIMEOUT_BOUND]
    :desc: Connix SHALL time out a send operation after 5 seconds when the
           operation has not completed.

.. sw-req:: Connection Management Timeout Socket Closure
    :id: [$SW_REQ CONNECTION_MANAGEMENT_TIMEOUT_SOCKET_CLOSURE]
    :desc: Connix SHALL close the affected socket promptly after any connection
           operation timeout.

.. sw-req:: Connection Management Timeout Failure Handling
    :id: [$SW_REQ CONNECTION_MANAGEMENT_TIMEOUT_FAILURE_HANDLING]
    :desc: After a connection timeout, Connix SHALL report the timed-out operation,
           exit non-zero in ONETIME mode, and remain available for future eligible
           triggers in PERIODIC mode.

.. sw-req:: Connection Management Timeout Error Reporting
    :id: [$SW_REQ CONNECTION_MANAGEMENT_TIMEOUT_ERROR_REPORTING]
    :desc: Connix SHALL report an operation-specific timeout error when a
           connection operation exceeds its bound.

.. sw-req:: Connection Management One-Time Timeout Outcome
    :id: [$SW_REQ CONNECTION_MANAGEMENT_ONE_TIME_TIMEOUT_OUTCOME]
    :desc: Connix SHALL exit non-zero after a timeout during an ONETIME run.

.. sw-req:: Connection Management Periodic Timeout Outcome
    :id: [$SW_REQ CONNECTION_MANAGEMENT_PERIODIC_TIMEOUT_OUTCOME]
    :desc: Connix SHALL continue future eligible PERIODIC triggers after a
           timed-out cycle has closed.

.. sw-req:: Connection Management Hybrid Connection Cycles
    :id: [$SW_REQ CONNECTION_MANAGEMENT_HYBRID_CONNECTION_CYCLES]
    :desc: For each HYBRID trigger, Connix SHALL start one CLIENT-role cycle and one
           SERVER-role cycle on separate sockets.

.. sw-req:: Connection Management Hybrid Leg Presence
    :id: [$SW_REQ CONNECTION_MANAGEMENT_HYBRID_LEG_PRESENCE]
    :desc: For each HYBRID trigger, the Connix SHALL start one CLIENT-role cycle
           and one SERVER-role cycle.

.. sw-req:: Connection Management Hybrid Concurrent Execution
    :id: [$SW_REQ CONNECTION_MANAGEMENT_HYBRID_CONCURRENT_EXECUTION]
    :desc: Connix SHALL execute the CLIENT-role and SERVER-role cycles started
           by a HYBRID trigger concurrently for the duration of both cycles.

.. sw-req:: Connection Management Hybrid Socket Isolation
    :id: [$SW_REQ CONNECTION_MANAGEMENT_HYBRID_SOCKET_ISOLATION]
    :desc: Connix SHALL assign separate sockets to the CLIENT-role and
           SERVER-role cycles started by a HYBRID trigger for the duration of both
           cycles.

.. sw-req:: Connection Management Hybrid Failure Isolation
    :id: [$SW_REQ CONNECTION_MANAGEMENT_HYBRID_FAILURE_ISOLATION]
    :desc: Connix SHALL allow the unaffected HYBRID leg to continue when the
           other HYBRID leg fails, for the duration of the unaffected cycle.

.. sw-req:: Connection Management Hybrid Independent Closure
    :id: [$SW_REQ CONNECTION_MANAGEMENT_HYBRID_INDEPENDENT_CLOSURE]
    :desc: Connix SHALL close each HYBRID leg's socket when that leg's own
           cycle completes, without waiting for the other leg.

.. sw-req:: Connection Management One-Time Connection Execution
    :id: [$SW_REQ CONNECTION_MANAGEMENT_ONE_TIME_CONNECTION_EXECUTION]
    :desc: Connix SHALL create one connection cycle for the action executed by
           ONETIME and SHALL not create an additional
           cycle for that action.

.. sw-req:: Connection Management Per-Trigger Connection Execution
    :id: [$SW_REQ CONNECTION_MANAGEMENT_PER_TRIGGER_CONNECTION_EXECUTION]
    :desc: Connix SHALL start one complete connection cycle for each eligible
           PERIODIC, event, or rule trigger, and SHALL skip a PERIODIC trigger
           while another PERIODIC cycle is active.

.. sw-req:: Connection Management Trigger Starts Cycle
    :id: [$SW_REQ CONNECTION_MANAGEMENT_TRIGGER_STARTS_CYCLE]
    :desc: Connix SHALL start one complete connection cycle when an eligible
           PERIODIC, event, or rule trigger occurs.

.. sw-req:: Connection Management Trigger Cycle Closure
    :id: [$SW_REQ CONNECTION_MANAGEMENT_TRIGGER_CYCLE_CLOSURE]
    :desc: Connix SHALL close a triggered connection cycle before starting the
           next eligible cycle.

.. sw-req:: Connection Management Periodic No Overlap
    :id: [$SW_REQ CONNECTION_MANAGEMENT_PERIODIC_NO_OVERLAP]
    :desc: Connix SHALL skip a PERIODIC trigger when another PERIODIC cycle is
           still active.

.. sw-req:: Connection Management Single Send Per Cycle
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SINGLE_SEND_PER_CYCLE]
    :desc: Connix SHALL permit at most one send action per connection cycle and SHALL
           reject a configuration that would exceed this limit.

.. sw-req:: Connection Management Single Send Configuration Rejection
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SINGLE_SEND_CONFIGURATION_REJECTION]
    :desc: Connix SHALL reject a configuration at configuration-load time when
           it would cause more than one send action within one connection cycle.

.. sw-req:: Connection Management Single Send Error Detail
    :id: [$SW_REQ CONNECTION_MANAGEMENT_SINGLE_SEND_ERROR_DETAIL]
    :desc: Connix SHALL identify the conflicting rules in the error reported
           for a multiple-send configuration.
