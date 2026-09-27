Event Handling
---------------

.. sw-req:: Event Handling
    :id: [$SW_REQ EVENT_HANDLING]
    :desc: Connix SHALL trigger configured actions from message-receipt, connect,
           accept, close, and timer events.

.. sw-req:: Event Handling Event Kinds
    :id: [$SW_REQ EVENT_HANDLING_EVENT_KINDS]
    :desc: Connix SHALL recognize message-receipt, connect, accept, close, and
           timer events when the corresponding input, connection state, or
           interval occurs.

.. sw-req:: Event Handling Message Receipt
    :id: [$SW_REQ EVENT_HANDLING_MESSAGE_RECEIPT]
    :desc: Connix SHALL recognize a message-receipt event when a configured
           connection receives a message.

.. sw-req:: Event Handling Connect
    :id: [$SW_REQ EVENT_HANDLING_CONNECT]
    :desc: Connix SHALL recognize a connect event when a CLIENT connection is
           established.

.. sw-req:: Event Handling Accept
    :id: [$SW_REQ EVENT_HANDLING_ACCEPT]
    :desc: Connix SHALL recognize an accept event when a SERVER connection is
           accepted.

.. sw-req:: Event Handling Close
    :id: [$SW_REQ EVENT_HANDLING_CLOSE]
    :desc: Connix SHALL recognize a close event when a connection cycle closes.

.. sw-req:: Event Handling Timer
    :id: [$SW_REQ EVENT_HANDLING_TIMER]
    :desc: Connix SHALL recognize a timer event when a configured elapsed-time
           interval expires.

.. sw-req:: Event Handling Periodic Timer Source
    :id: [$SW_REQ EVENT_HANDLING_PERIODIC_TIMER_SOURCE]
    :desc: Connix SHALL use the event-driven timer mechanism for intervals
           configured by PERIODIC mode.

.. sw-req:: Event Handling Connection Event Scope
    :id: [$SW_REQ EVENT_HANDLING_CONNECTION_EVENT_SCOPE]
    :desc: Connix SHALL expose only cycle-start and cycle-close connection events
           during one connection cycle.
