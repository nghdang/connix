Configuration
--------------

.. sw-req:: Configuration
    :id: SW_REQ_CONFIGURATION_FILE
    :desc: The Connix SHALL provide deterministic configuration-file and
        command-line configuration behavior through the linked child
        requirements below.
    :rationale: Supports reproducible workflows while making configuration
        interpretation deterministic.

.. sw-req:: Configuration Formats
    :id: SW_REQ_CONFIGURATION_FORMATS
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Connix SHALL accept the configuration formats defined by the linked
        child requirements.
    :rationale: Supports common configuration formats as separate behaviors.

.. sw-req:: JSON Configuration
    :id: SW_REQ_CONFIGURATION_JSON
    :links: SW_REQ_CONFIGURATION_FORMATS
    :desc: Connix SHALL accept a JSON configuration when the input is selected
        as JSON, for the duration of configuration loading.
    :rationale: Supports JSON-based configuration.

Fit Criterion: Connix accepts a valid JSON configuration and applies it.

.. sw-req:: YAML Configuration
    :id: SW_REQ_CONFIGURATION_YAML
    :links: SW_REQ_CONFIGURATION_FORMATS
    :desc: Connix SHALL accept a YAML configuration when the input is selected
        as YAML, for the duration of configuration loading.
    :rationale: Supports YAML-based configuration.

Fit Criterion: Connix accepts a valid YAML configuration and applies it.

.. sw-req:: Extension-Based Parser Selection
    :id: SW_REQ_CONFIGURATION_PARSER_SELECTION
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Connix SHALL provide the parser-selection behaviors defined by the
        linked child requirements.
    :rationale: Makes parser selection independently testable.

.. sw-req:: Extension Parser Mapping
    :id: SW_REQ_CONFIGURATION_PARSER_SELECTION_EXTENSION_MAP
    :links: SW_REQ_CONFIGURATION_PARSER_SELECTION
    :desc: Connix SHALL select JSON for ``.json`` files and YAML for ``.yaml``
        or ``.yml`` files when loading configuration.
    :rationale: Makes supported extension mapping deterministic.

Fit Criterion: Each supported extension selects its specified parser without
content sniffing.

.. sw-req:: Unsupported Extension Rejection
    :id: SW_REQ_CONFIGURATION_PARSER_SELECTION_UNSUPPORTED_EXTENSION
    :links: SW_REQ_CONFIGURATION_PARSER_SELECTION
    :desc: Connix SHALL reject a configuration file with an unsupported or
        missing extension unless an explicit format is supplied.
    :rationale: Prevents ambiguous parser selection.

Fit Criterion: An unsupported or extensionless file is rejected unless an
explicit format is supplied.

.. sw-req:: Configuration Parse Errors
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Connix SHALL provide the parse-error behaviors defined by the linked
        child requirements when the selected parser fails.
    :rationale: Keeps diagnostics, abort behavior, and parser fallback separate.

.. sw-req:: Parse Error Reporting
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR_REPORT
    :links: SW_REQ_CONFIGURATION_PARSE_ERROR
    :desc: Connix SHALL report a parse error identifying the configuration file
        and the reason when the selected parser fails.
    :rationale: Makes configuration failures diagnosable.

Fit Criterion: The parse error identifies the file and reason.

.. sw-req:: Parse Error Abort
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR_ABORT
    :links: SW_REQ_CONFIGURATION_PARSE_ERROR
    :desc: Connix SHALL exit non-zero without performing configured actions when
        the selected parser fails.
    :rationale: Prevents invalid configuration from starting execution.

Fit Criterion: Invalid content causes a non-zero exit and no configured action.

.. sw-req:: Parse Error No Fallback
    :id: SW_REQ_CONFIGURATION_PARSE_ERROR_NO_FALLBACK
    :links: SW_REQ_CONFIGURATION_PARSE_ERROR
    :desc: Connix SHALL NOT attempt another parser after the selected parser
        fails.
    :rationale: Preserves extension-based parser selection.

Fit Criterion: An invalid JSON file does not trigger a YAML parsing attempt,
and vice versa.

.. sw-req:: Explicit Format for Extensionless Configuration
    :id: SW_REQ_CONFIGURATION_EXPLICIT_FORMAT
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: Configuration supplied through stdin or another extensionless
        source SHALL require an explicit ``--format`` flag naming ``json`` or
        ``yaml``.
    :rationale: Ensures extensionless input is still interpreted deterministically.

Fit Criterion: Extensionless configuration without ``--format`` is rejected;
with ``--format json`` or ``--format yaml`` it is parsed accordingly.

.. sw-req:: CLI Configuration Precedence
    :id: SW_REQ_CONFIGURATION_CLI_PRECEDENCE
    :links: SW_REQ_CONFIGURATION_FILE
    :desc: When a setting is specified both in a configuration file and via a
        command-line flag, the command-line value SHALL take precedence.
    :rationale: Allows one-run overrides without editing the configuration file.

Fit Criterion: For conflicting file and command-line values, Connix uses the
command-line value.
