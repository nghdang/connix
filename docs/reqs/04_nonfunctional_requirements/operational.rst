Operational
===========

Deployment Mode
-----------------

.. sw-req:: Deployment Mode
    :id: SW_REQ_FOREGROUND_EXECUTION
    :desc: Connix SHALL provide the foreground execution and deployment
        constraints defined by the linked child requirements.
    :rationale: Keeps the operational model simple while allowing users to
        apply their own OS-level process tooling.

.. sw-req:: Foreground Process Execution
    :id: SW_REQ_FOREGROUND_EXECUTION_PROCESS
    :links: SW_REQ_FOREGROUND_EXECUTION
    :desc: Connix SHALL remain attached to its invoking process when Connix is
        started, including for the duration of PERIODIC execution.
    :rationale: Makes process lifetime and ownership explicit to the caller.

Fit Criterion: Starting Connix leaves it attached to the invoking process and
terminal, and PERIODIC execution does not detach it.

.. sw-req:: No Built-in Daemon Service
    :id: SW_REQ_NO_DAEMON_SERVICE
    :links: SW_REQ_FOREGROUND_EXECUTION
    :desc: Connix SHALL NOT provide a built-in daemon or background-service
        mode for the duration of the product's supported operation.
    :rationale: Leaves backgrounding and process supervision to OS-level tooling.

Fit Criterion: Connix exposes no built-in daemon or background-service mode.

.. sw-req:: No Detach Option
    :id: SW_REQ_NO_DETACH_OPTION
    :links: SW_REQ_FOREGROUND_EXECUTION
    :desc: Connix SHALL NOT provide a command-line option or configuration
        setting that detaches Connix from its controlling terminal during a run.
    :rationale: Prevents an implicit change from foreground to background
        execution.

Fit Criterion: No CLI option or configuration setting detaches a running
Connix process.

.. sw-req:: No PID File Management
    :id: SW_REQ_NO_PID_FILE_MANAGEMENT
    :links: SW_REQ_FOREGROUND_EXECUTION
    :desc: Connix SHALL NOT create or manage a PID file during a run.
    :rationale: Leaves process supervision and PID tracking to external tooling.

Fit Criterion: A Connix run creates and manages no PID file.

Note: Console and file output behavior is specified by
``SW_REQ_TRAFFIC_OUTPUT`` in the Functional Requirements section.
