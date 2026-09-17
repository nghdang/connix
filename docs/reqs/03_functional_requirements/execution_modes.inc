Execution Modes
----------------

.. sw-req:: Execution Modes
    :id: SW_REQ_RUNNING_MODES
    :desc: The Connix SHALL support built-in ONETIME and PERIODIC execution
        modes as defined by the linked child requirements below.
    :rationale: Supports both single-run and recurring operation without
        requiring separate tools.

.. sw-req:: ONETIME Mode
    :id: SW_REQ_RUNNING_MODES_ONETIME
    :links: SW_REQ_RUNNING_MODES
    :desc: Connix SHALL perform the configured action once under ONETIME mode
        when the invocation starts, then exit after that action completes.
    :rationale: Supports deterministic single-run operation.

Fit Criterion: A Connix run in ONETIME mode performs its configured action
once and exits.

.. sw-req:: PERIODIC Mode
    :id: SW_REQ_RUNNING_MODES_PERIODIC
    :links: SW_REQ_RUNNING_MODES
    :desc: Connix SHALL perform the configured action at each configured
        PERIODIC interval for the duration of the PERIODIC run, subject to the
        no-overlap rule in ``SW_REQ_CONNECTION_FLOW_PER_TRIGGER``.
    :rationale: Supports recurring operation with bounded trigger behavior.

Fit Criterion: A Connix run in PERIODIC mode repeats its configured action at
the configured interval.

.. sw-req:: Built-in Execution Scheduling
    :id: SW_REQ_RUNNING_MODES_BUILT_IN
    :links: SW_REQ_RUNNING_MODES
    :desc: Connix SHALL provide ONETIME and PERIODIC scheduling internally when
        either mode is selected, without requiring an external scheduler for
        the duration of the run.
    :rationale: Keeps execution behavior portable and self-contained.

Fit Criterion: Both modes run with Connix alone and do not require a separate
scheduler process. The PERIODIC timer-event relationship is specified by
``SW_REQ_EVENT_DRIVEN_TIMER_SOURCE``.
