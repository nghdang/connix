Operational
===========

Deployment Mode
-----------------

.. sw-req:: Deployment Mode
    :id: SW_REQ_FOREGROUND_EXECUTION
    :desc: The Connix SHALL run as a foreground process, including in
        periodic execution mode. Connix SHALL NOT provide a built-in
        daemon or background-service mode.
    :rationale: Keeps the operational model simple; users who need
        backgrounding can rely on their own OS-level tooling to do so.

Fit Criterion: Connix has no command-line option or configuration
setting that detaches the process from its controlling terminal or
manages a PID file.

Note: Console and file output behavior is specified by
``SW_REQ_TRAFFIC_OUTPUT`` in the Functional Requirements section.
