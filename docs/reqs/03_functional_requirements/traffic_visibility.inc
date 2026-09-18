Traffic Visibility
--------------------

.. sw-req:: Traffic Visibility
    :id: [$SW_REQ TRAFFIC_VISIBILITY]
    :desc: Connix SHALL provide visibility of sent traffic, received traffic,
           and log messages.

.. sw-req:: Traffic Visibility Sent Traffic
    :id: [$SW_REQ TRAFFIC_VISIBILITY_SENT_TRAFFIC]
    :desc: Connix SHALL output sent traffic when a send action completes during
           a connection cycle.

.. sw-req:: Traffic Visibility Received Traffic
    :id: [$SW_REQ TRAFFIC_VISIBILITY_RECEIVED_TRAFFIC]
    :desc: Connix SHALL output received traffic when a receive action completes
           during a connection cycle.

.. sw-req:: Traffic Visibility Stdout Default
    :id: [$SW_REQ TRAFFIC_VISIBILITY_STDOUT_DEFAULT]
    :desc: Connix SHALL output sent traffic, received traffic, and log messages
           to stdout by default.

.. sw-req:: Traffic Visibility Log
    :id: [$SW_REQ TRAFFIC_VISIBILITY_LOG]
    :desc: Connix SHALL output log messages when diagnostic events occur during
           a run.

.. sw-req:: Traffic Visibility File Redirection
    :id: [$SW_REQ TRAFFIC_VISIBILITY_FILE_REDIRECTION]
    :desc: Connix SHALL redirect sent traffic, received traffic, and log messages
           to a configured output file.

.. sw-req:: Traffic Visibility File Destination
    :id: [$SW_REQ TRAFFIC_VISIBILITY_FILE_DESTINATION]
    :desc: Connix SHALL write sent traffic, received traffic, and log messages to
           the configured output file during a run.

.. sw-req:: Traffic Visibility File Stdout Suppression
    :id: [$SW_REQ TRAFFIC_VISIBILITY_FILE_STDOUT_SUPPRESSION]
    :desc: Connix SHALL suppress sent traffic, received traffic, and log messages
           on stdout when file redirection is configured.
