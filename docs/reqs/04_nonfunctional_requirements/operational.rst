Operational
===========

.. sw-req:: Operational Deployment Mode
    :id: [$SW_REQ_NF OPERATIONAL_DEPLOYMENT_MODE]
    :desc: Connix SHALL run exclusively as a foreground process attached to its
           invoking process and terminal.

.. sw-req:: Operational Deployment Mode Foreground Process
    :id: [$SW_REQ_NF OPERATIONAL_DEPLOYMENT_MODE_FOREGROUND_PROCESS]
    :desc: Connix SHALL remain attached to its invoking process for the duration
           of a run, including PERIODIC execution.

.. sw-req:: Operational Deployment Mode Daemon Service
    :id: [$SW_REQ_NF OPERATIONAL_DEPLOYMENT_MODE_DAEMON_SERVICE]
    :desc: Connix SHALL NOT provide a built-in daemon or background-service mode.

.. sw-req:: Operational Deployment Mode Detach Option
    :id: [$SW_REQ_NF OPERATIONAL_DEPLOYMENT_MODE_DETACH_OPTION]
    :desc: Connix SHALL NOT provide an option that detaches it from its controlling
           terminal during a run.

.. sw-req:: Operational Deployment Mode PID File Management
    :id: [$SW_REQ_NF OPERATIONAL_DEPLOYMENT_MODE_PID_FILE_MANAGEMENT]
    :desc: Connix SHALL NOT create or manage a PID file during a run.
