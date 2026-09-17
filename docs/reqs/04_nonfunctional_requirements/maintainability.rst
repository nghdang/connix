Maintainability
===============

Protocol Extensibility
------------------------

.. sw-req:: Protocol Extensibility
    :id: SW_REQ_PROTOCOL_EXTENSIBILITY
    :desc: connix-core SHALL expose a common client/server interface that a
        new protocol implementation can satisfy without modifying the core
        orchestration engine, when the new protocol is integrated and for the
        duration of its operation.
    :rationale: New protocols beyond TCP, UDP, and Unix Domain Sockets must be
        addable without reworking the core.

Fit Criterion: A new protocol implementation can be added by
implementing the existing client/server interface, with no changes
required to the core orchestration engine.
