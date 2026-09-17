User Interfaces
----------------

.. sw-req:: User Interfaces
    :id: SW_REQ_USER_INTERFACES
    :desc: The Connix SHALL provide user interfaces through a CLI and, when
        enabled, a GUI. The atomic interface behaviors are specified by the
        linked child requirements below.
    :rationale: Supports scripting-oriented and visual interaction workflows
        without forcing every build or release to include the GUI.

.. sw-req:: CLI Provision
    :id: SW_REQ_USER_INTERFACES_CLI
    :links: SW_REQ_USER_INTERFACES
    :desc: The Connix SHALL provide a Command-Line Interface (CLI).
    :rationale: Supports scripting-oriented and terminal-based workflows.

Fit Criterion: Connix ships a CLI binary that is invocable from a terminal.

.. sw-req:: GUI Provision
    :id: SW_REQ_USER_INTERFACES_GUI
    :links: SW_REQ_USER_INTERFACES
    :desc: Connix SHALL provide the GUI behaviors defined by the linked child
        requirements when GUI support is selected.
    :rationale: Keeps GUI provision and packaging decisions independently
        testable.

.. sw-req:: GUI Provision When Enabled
    :id: SW_REQ_USER_INTERFACES_GUI_PROVISION
    :links: SW_REQ_USER_INTERFACES_GUI
    :desc: Connix SHALL produce a GUI binary when the GUI build option is
        enabled.
    :rationale: Provides the visual interface when requested at build time.

Fit Criterion: A build with the GUI option enabled produces a GUI binary.

.. sw-req:: GUI Optional Build
    :id: SW_REQ_USER_INTERFACES_GUI_OPTIONAL_BUILD
    :links: SW_REQ_USER_INTERFACES_GUI
    :desc: Connix SHALL allow the GUI build option to be disabled without
        preventing a compliant CLI build.
    :rationale: Supports headless CI/CD builds.

Fit Criterion: A build with the GUI option disabled remains compliant and
produces the CLI without a GUI binary.

.. sw-req:: GUI Optional Distribution
    :id: SW_REQ_USER_INTERFACES_GUI_OPTIONAL_DISTRIBUTION
    :links: SW_REQ_USER_INTERFACES_GUI
    :desc: Connix SHALL allow a release to omit the GUI binary when the release
        is intended to be headless.
    :rationale: Allows release packaging to target headless environments.

Fit Criterion: A headless release omits the GUI binary while remaining
compliant. GUI/CLI capability parity is specified by
``SW_REQ_CLI_GUI_CONSISTENCY``.
