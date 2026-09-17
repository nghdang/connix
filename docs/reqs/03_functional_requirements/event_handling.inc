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
