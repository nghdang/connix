# Connix Qualification Test Specification

## Purpose and Scope

This directory contains the qualification test specification for every requirement defined in
`docs/reqs/03_functional_requirements/` and `docs/reqs/04_nonfunctional_requirements/`. Each
requirement (`SW_REQ_*`) gets its own test spec file with test cases covering:

- **Positive / happy path** — the requirement's primary intended behavior.
- **Boundary / edge cases** — limits, transitions, and unusual-but-valid input.
- **Negative / error handling** — invalid input, failure conditions, and how the system is
  expected to respond.

Two nonfunctional sections — **Security** (`security.rst`) and **Look and Feel**
(`look_and_feel.rst`) — explicitly state "no requirements defined at this time." No test spec is
produced for these; they are out of scope until a future elicitation session defines requirements.

### Implementation status caveat

Per `CLAUDE.md`, only the `common/logging` module exists in the source tree today. The CLI, GUI,
transport/connection/rule-engine, and configuration-parsing behavior described by these
requirements are not yet implemented. Test cases in this specification are written against the
**specified/target behavior** (each requirement's `:desc:` and Fit Criterion) so that they are
ready to execute as each feature is implemented — they are not yet runnable against current code.

## Test Design Techniques (ISTQB)

Applied per requirement, as appropriate to its structure:

- **Equivalence Partitioning (EP)** — for inputs with distinct valid/invalid classes (e.g.
  protocol choice, config file extension, execution mode).
- **Boundary Value Analysis (BVA)** — for numeric/time thresholds (e.g. CPU %, RSS growth, timer
  intervals).
- **Decision Table Testing** — for combinatorial behavior (e.g. role × execution mode × event
  kind in the connection/rule requirements).
- **State Transition Testing** — for the connection lifecycle (connect/accept → send/receive →
  close) and role-driven cycles.
- **Error Guessing** — for failure-handling paths not fully enumerated by the requirement text
  (e.g. malformed config, unreachable endpoint).

Each test case is tagged with an ID `TC_<REQ_ID_SUFFIX>_NN`, a **Type**
(Positive / Boundary / Negative / Error), and a **Priority** (High / Medium / Low), per common
ISTQB test-case documentation practice (ISTQB CTFL syllabus, test case specification).

## Traceability Matrix

| Req ID | Section | Test Spec File | # Test Cases | Status |
|---|---|---|---|---|
| SW_REQ_USER_INTERFACES | Functional | [SW_REQ_USER_INTERFACES_test_spec.md](SW_REQ_USER_INTERFACES_test_spec.md) | 7 | Parent; mapped to child requirements |
| SW_REQ_USER_INTERFACES_CLI | Functional | [SW_REQ_USER_INTERFACES_test_spec.md](SW_REQ_USER_INTERFACES_test_spec.md) | 1 | Mapped: CLI cases |
| SW_REQ_USER_INTERFACES_GUI | Functional | [SW_REQ_USER_INTERFACES_test_spec.md](SW_REQ_USER_INTERFACES_test_spec.md) | 3 | Parent; mapped to GUI children |
| SW_REQ_USER_INTERFACES_GUI_PROVISION | Functional | [SW_REQ_USER_INTERFACES_test_spec.md](SW_REQ_USER_INTERFACES_test_spec.md) | 1 | Mapped: GUI provision |
| SW_REQ_USER_INTERFACES_GUI_OPTIONAL_BUILD | Functional | [SW_REQ_USER_INTERFACES_test_spec.md](SW_REQ_USER_INTERFACES_test_spec.md) | 1 | Mapped: optional build |
| SW_REQ_USER_INTERFACES_GUI_OPTIONAL_DISTRIBUTION | Functional | [SW_REQ_USER_INTERFACES_test_spec.md](SW_REQ_USER_INTERFACES_test_spec.md) | 1 | Mapped: optional distribution |
| SW_REQ_MULTIPLE_PROTOCOLS | Functional | [SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md](SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md) | 8 | Parent; mapped to protocol children |
| SW_REQ_PROTOCOL_TCP | Functional | [SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md](SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md) | 3 | Mapped: TCP cases |
| SW_REQ_PROTOCOL_UDP | Functional | [SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md](SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md) | 3 | Mapped: UDP cases |
| SW_REQ_PROTOCOL_UDS | Functional | [SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md](SW_REQ_MULTIPLE_PROTOCOLS_test_spec.md) | 2 | Mapped: UDS cases |
| SW_REQ_WORKING_ROLES | Functional | [SW_REQ_WORKING_ROLES_test_spec.md](SW_REQ_WORKING_ROLES_test_spec.md) | 7 | Parent; mapped to role children |
| SW_REQ_ROLE_CLIENT | Functional | [SW_REQ_WORKING_ROLES_test_spec.md](SW_REQ_WORKING_ROLES_test_spec.md) | 2 | Mapped: CLIENT cases |
| SW_REQ_ROLE_SERVER | Functional | [SW_REQ_WORKING_ROLES_test_spec.md](SW_REQ_WORKING_ROLES_test_spec.md) | 2 | Mapped: SERVER cases |
| SW_REQ_ROLE_DUAL | Functional | [SW_REQ_WORKING_ROLES_test_spec.md](SW_REQ_WORKING_ROLES_test_spec.md) | 3 | Mapped: DUAL cases |
| SW_REQ_SIMPLE_CONNECTION_FLOW | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 9 | Parent; mapped to child requirements |
| SW_REQ_CONNECTION_FLOW_CLIENT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 5 | Parent; mapped to CLIENT children |
| SW_REQ_CONNECTION_FLOW_CLIENT_LIFECYCLE | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 2 | Mapped: CLIENT lifecycle cases |
| SW_REQ_CONNECTION_FLOW_CLIENT_CONNECT_TIMEOUT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: CLIENT connect timeout |
| SW_REQ_CONNECTION_FLOW_CLIENT_SEND_TIMEOUT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: CLIENT send timeout |
| SW_REQ_CONNECTION_FLOW_CLIENT_RECEIVE_TIMEOUT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: CLIENT receive timeout |
| SW_REQ_CONNECTION_FLOW_CLIENT_TIMEOUT_RESULT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: CLIENT timeout result |
| SW_REQ_CONNECTION_FLOW_SERVER | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 4 | Parent; mapped to SERVER children |
| SW_REQ_CONNECTION_FLOW_SERVER_LIFECYCLE | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: SERVER lifecycle |
| SW_REQ_CONNECTION_FLOW_SERVER_ACCEPT_TIMEOUT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: SERVER accept timeout |
| SW_REQ_CONNECTION_FLOW_SERVER_RECEIVE_TIMEOUT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: SERVER receive timeout |
| SW_REQ_CONNECTION_FLOW_SERVER_SEND_TIMEOUT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: SERVER send timeout |
| SW_REQ_CONNECTION_FLOW_SERVER_TIMEOUT_RESULT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: SERVER timeout result |
| SW_REQ_CONNECTION_FLOW_TIMEOUTS | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 4 | Parent; mapped to timeout children |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_CONNECT_ACCEPT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 2 | Mapped: connect/accept bounds |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_RECEIVE | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: receive bound |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_SEND | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: send bound |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_SOCKET_CLOSURE | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: timeout socket closure |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 3 | Parent; mapped to timeout-result children |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_REPORT | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: timeout error reporting |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_ONETIME | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: ONETIME timeout outcome |
| SW_REQ_CONNECTION_FLOW_TIMEOUT_RESULT_PERIODIC | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: PERIODIC timeout outcome |
| SW_REQ_CONNECTION_FLOW_DUAL | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 2 | Parent; mapped to DUAL child requirements |
| SW_REQ_CONNECTION_FLOW_DUAL_LEGS | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: TC_FLOW_04 |
| SW_REQ_CONNECTION_FLOW_DUAL_CONCURRENCY | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: TC_FLOW_04 |
| SW_REQ_CONNECTION_FLOW_DUAL_SOCKET_ISOLATION | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: TC_FLOW_04 |
| SW_REQ_CONNECTION_FLOW_DUAL_FAILURE_ISOLATION | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: TC_FLOW_05 |
| SW_REQ_CONNECTION_FLOW_DUAL_INDEPENDENT_CLOSURE | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: TC_FLOW_04, TC_FLOW_05 |
| SW_REQ_CONNECTION_FLOW_ONETIME | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: TC_FLOW_06 |
| SW_REQ_CONNECTION_FLOW_PER_TRIGGER | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 4 | Parent; mapped to per-trigger children |
| SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_START | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 2 | Mapped: trigger cycle start |
| SW_REQ_CONNECTION_FLOW_PER_TRIGGER_CYCLE_CLOSURE | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: trigger cycle closure |
| SW_REQ_CONNECTION_FLOW_PER_TRIGGER_PERIODIC_NO_OVERLAP | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: periodic overlap |
| SW_REQ_CONNECTION_FLOW_SINGLE_SEND | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 2 | Parent; mapped to single-send children |
| SW_REQ_CONNECTION_FLOW_SINGLE_SEND_REJECTION | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: config rejection |
| SW_REQ_CONNECTION_FLOW_SINGLE_SEND_ERROR_DETAIL | Functional | [SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md](SW_REQ_SIMPLE_CONNECTION_FLOW_test_spec.md) | 1 | Mapped: error detail |
| SW_REQ_EVENT_DRIVEN | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 8 | Parent; mapped to event children |
| SW_REQ_EVENT_DRIVEN_KINDS | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 8 | Parent; mapped to event children |
| SW_REQ_EVENT_MESSAGE_RECEIPT | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 2 | Mapped: message-receipt cases |
| SW_REQ_EVENT_CONNECT | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 1 | Mapped: connect cases |
| SW_REQ_EVENT_ACCEPT | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 1 | Mapped: accept cases |
| SW_REQ_EVENT_CLOSE | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 1 | Mapped: close cases |
| SW_REQ_EVENT_TIMER | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 2 | Mapped: timer cases |
| SW_REQ_EVENT_DRIVEN_TIMER_SOURCE | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 2 | Mapped: timer-source cases |
| SW_REQ_EVENT_DRIVEN_CYCLE_SCOPE | Functional | [SW_REQ_EVENT_DRIVEN_test_spec.md](SW_REQ_EVENT_DRIVEN_test_spec.md) | 2 | Mapped: cycle-scope cases |
| SW_REQ_RULE_BASED | Functional | [SW_REQ_RULE_BASED_test_spec.md](SW_REQ_RULE_BASED_test_spec.md) | 7 | Parent; mapped to child requirements |
| SW_REQ_RULE_BASED_DEFINITION | Functional | [SW_REQ_RULE_BASED_test_spec.md](SW_REQ_RULE_BASED_test_spec.md) | 4 | Mapped: rule-definition cases |
| SW_REQ_RULE_BASED_CYCLE_SCOPE | Functional | [SW_REQ_RULE_BASED_test_spec.md](SW_REQ_RULE_BASED_test_spec.md) | 3 | Mapped: cycle-scope cases |
| SW_REQ_RUNNING_MODES | Functional | [SW_REQ_RUNNING_MODES_test_spec.md](SW_REQ_RUNNING_MODES_test_spec.md) | 6 | Parent; mapped to child requirements |
| SW_REQ_RUNNING_MODES_ONETIME | Functional | [SW_REQ_RUNNING_MODES_test_spec.md](SW_REQ_RUNNING_MODES_test_spec.md) | 2 | Mapped: ONETIME cases |
| SW_REQ_RUNNING_MODES_PERIODIC | Functional | [SW_REQ_RUNNING_MODES_test_spec.md](SW_REQ_RUNNING_MODES_test_spec.md) | 2 | Mapped: PERIODIC cases |
| SW_REQ_RUNNING_MODES_BUILT_IN | Functional | [SW_REQ_RUNNING_MODES_test_spec.md](SW_REQ_RUNNING_MODES_test_spec.md) | 2 | Mapped: built-in scheduling cases |
| SW_REQ_CONFIGURATION_FILE | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 11 | Parent; mapped to configuration children |
| SW_REQ_CONFIGURATION_FORMATS | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 11 | Parent; mapped to format children |
| SW_REQ_CONFIGURATION_JSON | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 1 | Mapped: JSON cases |
| SW_REQ_CONFIGURATION_YAML | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 1 | Mapped: YAML cases |
| SW_REQ_CONFIGURATION_PARSER_SELECTION | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 3 | Parent; mapped to parser-selection children |
| SW_REQ_CONFIGURATION_PARSER_SELECTION_EXTENSION_MAP | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 2 | Mapped: extension mapping |
| SW_REQ_CONFIGURATION_PARSER_SELECTION_UNSUPPORTED_EXTENSION | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 1 | Mapped: unsupported extension rejection |
| SW_REQ_CONFIGURATION_PARSE_ERROR | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 3 | Parent; mapped to parse-error children |
| SW_REQ_CONFIGURATION_PARSE_ERROR_REPORT | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 1 | Mapped: error reporting |
| SW_REQ_CONFIGURATION_PARSE_ERROR_ABORT | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 1 | Mapped: abort behavior |
| SW_REQ_CONFIGURATION_PARSE_ERROR_NO_FALLBACK | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 1 | Mapped: no fallback |
| SW_REQ_CONFIGURATION_EXPLICIT_FORMAT | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 2 | Mapped: extensionless-input cases |
| SW_REQ_CONFIGURATION_CLI_PRECEDENCE | Functional | [SW_REQ_CONFIGURATION_FILE_test_spec.md](SW_REQ_CONFIGURATION_FILE_test_spec.md) | 2 | Mapped: precedence cases |
| SW_REQ_TRAFFIC_OUTPUT | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 7 | Parent; mapped to output children |
| SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 1 | Mapped: sent-traffic cases |
| SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 1 | Mapped: received-traffic cases |
| SW_REQ_TRAFFIC_OUTPUT_STDOUT_DEFAULT | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 2 | Mapped: default-destination cases |
| SW_REQ_TRAFFIC_OUTPUT_LOG | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 2 | Mapped: log-output cases |
| SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 3 | Parent; mapped to redirection children |
| SW_REQ_TRAFFIC_OUTPUT_FILE_DESTINATION | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 2 | Mapped: file destination |
| SW_REQ_TRAFFIC_OUTPUT_FILE_STDOUT_SUPPRESSION | Functional | [SW_REQ_TRAFFIC_OUTPUT_test_spec.md](SW_REQ_TRAFFIC_OUTPUT_test_spec.md) | 1 | Mapped: stdout suppression |
| SW_REQ_LOW_RESOURCE_USAGE | Performance | [SW_REQ_LOW_RESOURCE_USAGE_test_spec.md](SW_REQ_LOW_RESOURCE_USAGE_test_spec.md) | 8 | Parent; mapped to child requirements |
| SW_REQ_RESOURCE_CPU_IDLE | Performance | [SW_REQ_LOW_RESOURCE_USAGE_test_spec.md](SW_REQ_LOW_RESOURCE_USAGE_test_spec.md) | 4 | Mapped: CPU cases |
| SW_REQ_RESOURCE_MEMORY_GROWTH | Performance | [SW_REQ_LOW_RESOURCE_USAGE_test_spec.md](SW_REQ_LOW_RESOURCE_USAGE_test_spec.md) | 4 | Mapped: RSS cases |
| SW_REQ_CLI_GUI_CONSISTENCY | Usability | [SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md](SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md) | 5 | Parent; mapped to parity children |
| SW_REQ_CLI_GUI_CONSISTENCY_ACTIONS | Usability | [SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md](SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md) | 2 | Blocked: action parity artifact |
| SW_REQ_CLI_GUI_CONSISTENCY_RESULTS | Usability | [SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md](SW_REQ_CLI_GUI_CONSISTENCY_test_spec.md) | 3 | Blocked: GUI capability artifact |
| SW_REQ_PROTOCOL_EXTENSIBILITY | Maintainability | [SW_REQ_PROTOCOL_EXTENSIBILITY_test_spec.md](SW_REQ_PROTOCOL_EXTENSIBILITY_test_spec.md) | 5 | Blocked (target interface named, not yet implemented) |
| SW_REQ_FOREGROUND_EXECUTION | Operational | [SW_REQ_FOREGROUND_EXECUTION_test_spec.md](SW_REQ_FOREGROUND_EXECUTION_test_spec.md) | 5 | Parent; mapped to child requirements |
| SW_REQ_FOREGROUND_EXECUTION_PROCESS | Operational | [SW_REQ_FOREGROUND_EXECUTION_test_spec.md](SW_REQ_FOREGROUND_EXECUTION_test_spec.md) | 3 | Mapped: foreground-process cases |
| SW_REQ_NO_DAEMON_SERVICE | Operational | [SW_REQ_FOREGROUND_EXECUTION_test_spec.md](SW_REQ_FOREGROUND_EXECUTION_test_spec.md) | 1 | Mapped: daemon-service case |
| SW_REQ_NO_DETACH_OPTION | Operational | [SW_REQ_FOREGROUND_EXECUTION_test_spec.md](SW_REQ_FOREGROUND_EXECUTION_test_spec.md) | 1 | Mapped: detach case |
| SW_REQ_NO_PID_FILE_MANAGEMENT | Operational | [SW_REQ_FOREGROUND_EXECUTION_test_spec.md](SW_REQ_FOREGROUND_EXECUTION_test_spec.md) | 1 | Mapped: PID-file case |
| — | Security | *(no requirements defined — out of scope)* | — | N/A |
| — | Look and Feel | *(no requirements defined — out of scope)* | — | N/A |

**Status legend:**
- **Ready** — test cases are fully designed and deterministic, and (once implemented) executable
  as written.
- **Blocked (...)** — the requirement's testability is fundamentally limited by a missing design
  artifact (e.g. an interface or capability list that doesn't exist yet); test cases are drafted
  at the highest level of confidence currently possible, but full coverage cannot be guaranteed
  until the artifact exists.

All 7 open points raised during initial test-spec design have received PO answers — see
[OPEN_POINTS.md](OPEN_POINTS.md). Five were fully resolved, letting the affected test cases become
deterministic (reflected above as **Ready**). The remaining two (`SW_REQ_CLI_GUI_CONSISTENCY`,
`SW_REQ_PROTOCOL_EXTENSIBILITY`) got PO confirmation of the plan/target but still depend on a
design artifact (a GUI capability list, and the `ITransport` interface landing in source) that
doesn't exist yet, so they remain **Blocked** pending that artifact.
