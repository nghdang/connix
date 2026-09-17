Traffic Visibility
--------------------

.. sw-req:: Traffic Visibility
    :id: SW_REQ_TRAFFIC_OUTPUT
    :desc: The Connix SHALL provide traffic and log output with the default
        destination and redirection behavior defined by the linked children.
    :rationale: Enables observation and persistent recording of traffic and
        diagnostics.

.. sw-req:: Sent Traffic Output
    :id: SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL output sent traffic when a send action completes during
        a connection cycle.
    :rationale: Makes outbound communication observable.

Fit Criterion: Completed sent messages are present in the configured output.

.. sw-req:: Received Traffic Output
    :id: SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL output received traffic when a receive action completes
        during a connection cycle.
    :rationale: Makes inbound communication observable.

Fit Criterion: Completed received messages are present in the configured output.

.. sw-req:: Default Standard Output
    :id: SW_REQ_TRAFFIC_OUTPUT_STDOUT_DEFAULT
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL direct sent traffic, received traffic, and log output
        to stdout by default when no output-file option is configured.
    :rationale: Provides immediate visibility during normal CLI use.

Fit Criterion: Without output-file configuration, traffic and log output appear
on stdout.

.. sw-req:: Log Output
    :id: SW_REQ_TRAFFIC_OUTPUT_LOG
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL output its own log messages when a diagnostic event
        occurs during a run.
    :rationale: Makes diagnostics observable during normal use.

Fit Criterion: Connix diagnostic messages are present in the configured output.
Log format, levels, and verbosity remain implementation-defined.

.. sw-req:: File Output Redirection
    :id: SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
    :links: SW_REQ_TRAFFIC_OUTPUT
    :desc: Connix SHALL provide the file-redirection behaviors defined by the
        linked child requirements when an output file is configured.
    :rationale: Keeps file destination and stdout suppression independently
        testable.

.. sw-req:: File Output Destination
    :id: SW_REQ_TRAFFIC_OUTPUT_FILE_DESTINATION
    :links: SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
    :desc: Connix SHALL write sent traffic, received traffic, and log output to
        the configured file during the run.
    :rationale: Provides a persistent output record.

Fit Criterion: The configured file contains the redirected traffic and log
output.

.. sw-req:: Redirected Stdout Suppression
    :id: SW_REQ_TRAFFIC_OUTPUT_FILE_STDOUT_SUPPRESSION
    :links: SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
    :desc: Connix SHALL suppress the default stdout copy of sent traffic,
        received traffic, and log output while file redirection is configured.
    :rationale: Prevents redirected output from being duplicated to stdout.

Fit Criterion: With file redirection enabled, stdout contains none of the
redirected traffic or log output.
