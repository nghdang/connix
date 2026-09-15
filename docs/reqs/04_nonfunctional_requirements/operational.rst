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

Output Destination
--------------------

.. sw-req:: Output Destination
    :id: SW_REQ_CONSOLE_OUTPUT_DEFAULT
    :desc: The Connix SHALL print traffic and log output to the console
        (stdout) by default. The Connix SHALL allow the user to
        redirect output to a file.
    :rationale: Console output supports interactive and scripted use
        out of the box, while file output remains available for users
        who need a persistent record.

Fit Criterion: Running Connix without any output configuration prints
traffic to the console. A configuration option or command-line flag
redirects that same output to a file instead.
