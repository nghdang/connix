Usability
=========

Consistency Between Interfaces
--------------------------------

.. sw-req:: Consistency Between Interfaces
    :id: SW_REQ_CLI_GUI_CONSISTENCY
    :desc: The Connix GUI SHALL produce the same underlying actions and
        results as the equivalent CLI invocation, for every capability
        the GUI exposes.
    :rationale: Users move between the CLI and GUI and must be able to
        rely on identical behavior regardless of which interface they
        use.

Fit Criterion: For any action available in both interfaces, the same
input configuration (protocol, role, rule, or execution mode) produces
the same connection behavior and output content in both the CLI and the
GUI.
