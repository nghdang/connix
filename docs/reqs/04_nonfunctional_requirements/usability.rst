Usability
=========

Consistency Between Interfaces
--------------------------------

.. sw-req:: Consistency Between Interfaces
    :id: SW_REQ_CLI_GUI_CONSISTENCY
    :desc: Connix SHALL provide the interface-consistency behaviors defined by
        the linked child requirements.
    :rationale: Keeps action parity and result parity independently testable.

.. sw-req:: GUI Action Parity
    :id: SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS
    :links: SW_REQ_CLI_GUI_CONSISTENCY
    :desc: Connix SHALL make its GUI perform the same underlying action as an
        equivalent CLI invocation for each capability exposed by both
        interfaces, when both use the same input configuration.
    :rationale: Ensures interface selection does not change the requested action.

Fit Criterion: Tracing equivalent CLI and GUI invocations shows the same
underlying action for each shared capability.

.. sw-req:: GUI Result Parity
    :id: SW_REQ_CLI_GUI_CONSISTENCY_RESULTS
    :links: SW_REQ_CLI_GUI_CONSISTENCY
    :desc: Connix SHALL make its GUI produce the same result as an equivalent
        CLI invocation for each capability exposed by both interfaces, for the
        duration of the corresponding operation.
    :rationale: Ensures interface selection does not change observable results.

Fit Criterion: Equivalent CLI and GUI invocations produce the same connection
behavior and output content.
