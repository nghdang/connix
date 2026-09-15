Maintainability
===============

Protocol Extensibility
------------------------

.. sw-req:: Protocol Extensibility
    :id: SW_REQ_PROTOCOL_EXTENSIBILITY
    :desc: The connix-core SHALL expose a common client/server interface
        that a new protocol implementation can satisfy without
        modifying the core orchestration engine.
    :rationale: New protocols beyond TCP, UDP, and Unix Domain Sockets
        must be addable without reworking the core; this was called out
        as an important near-term design concern.

Fit Criterion: A new protocol implementation can be added by
implementing the existing client/server interface, with no changes
required to the core orchestration engine.
