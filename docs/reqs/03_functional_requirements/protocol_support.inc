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
