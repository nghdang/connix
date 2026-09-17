Operating Roles
-----------------

.. sw-req:: Operating Roles
    :id: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL provide the CLIENT, SERVER, and DUAL role behaviors
        defined by the linked child requirements.
    :rationale: Enables flexible deployment without additional tooling.

.. sw-req:: CLIENT Role
    :id: SW_REQ_ROLE_CLIENT
    :links: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL operate as a CLIENT when the CLIENT role is selected
        by configuration or command-line option, for the configured run.
    :rationale: Enables outbound connection initiation.

Fit Criterion: Selecting CLIENT starts CLIENT behavior without a separate build.

.. sw-req:: SERVER Role
    :id: SW_REQ_ROLE_SERVER
    :links: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL operate as a SERVER when the SERVER role is selected
        by configuration or command-line option, for the configured run.
    :rationale: Enables inbound connection acceptance.

Fit Criterion: Selecting SERVER starts SERVER behavior without a separate build.

.. sw-req:: DUAL Role
    :id: SW_REQ_ROLE_DUAL
    :links: SW_REQ_WORKING_ROLES
    :desc: Connix SHALL operate as DUAL when the DUAL role is selected by
        configuration or command-line option, for the configured run.
    :rationale: Enables simultaneous client and server operation.

Fit Criterion: Selecting DUAL starts both DUAL legs without a separate build.
