# Test Specification: SW_REQ_CONFIGURATION_FILE

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the configuration children under the parent
`SW_REQ_CONFIGURATION_FILE`: `SW_REQ_CONFIGURATION_JSON`, `SW_REQ_CONFIGURATION_YAML`,
`SW_REQ_CONFIGURATION_PARSER_SELECTION`, `SW_REQ_CONFIGURATION_PARSE_ERROR`,
`SW_REQ_CONFIGURATION_EXPLICIT_FORMAT`, and `SW_REQ_CONFIGURATION_CLI_PRECEDENCE`.

**Fit Criterion:** Each child behavior is tested independently: JSON and YAML acceptance,
extension-based parser selection, deterministic parse errors without fallback, explicit format for
extensionless input, and command-line precedence over conflicting file values. A `.json` file and an equivalent
`.yaml` file produce the same behavior. An
invalid `.json` file produces a parse error naming the file and exits non-zero, without
attempting YAML parsing. Piping an equivalent config via stdin with `--format yaml` produces the
same behavior as the file-based YAML case. When a configuration file and a command-line flag
specify conflicting values for the same setting, Connix runs using the command-line flag's value.

## Test Design Techniques

- Equivalence Partitioning: extension classes {.json, .yaml, .yml, no/other extension},
  `--format` classes {json, yaml, missing, invalid}.
- Decision Table: extension × `--format` flag presence × content validity; config-value ×
  CLI-flag-value presence/conflict for precedence.
- Error Guessing: extension/content mismatch, ambiguous stdin without `--format`.

## Test Cases

### TC_CFG_01 — Valid `.json` config accepted
*Type: Positive · Priority: High*

Title: Valid `.json` config accepted.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_JSON
Objective: Happy — Verify that a valid JSON configuration is parsed and applied.
Steps:
   - 1. Create `$TEST_TMPDIR/config.json` containing a complete minimal CLIENT configuration with protocol `TCP`, host `127.0.0.1`, an ephemeral peer port, mode `ONETIME`, and payload `PING`.
   - 2. Start an in-process TCP echo peer on the configured port and launch Connix with `--config $TEST_TMPDIR/config.json`.
   - 3. Capture the selected parser, peer bytes, Connix event trace, and exit status.

Expectations:
   - 1. The JSON file is created and the TCP peer becomes ready before Connix starts.
   - 2. The JSON parser is selected; Connix connects, sends `PING`, receives the echo, and applies the CLIENT configuration.
   - 3. The parser trace identifies JSON, the cycle succeeds, and the process exits with success.

TestData:
   - JSON: `{ "protocol": "TCP", "role": "CLIENT", "host": "127.0.0.1", "port": <ephemeral-port>, "mode": "ONETIME", "payload": "PING" }`.
   - Capture parser selection, ordered events, peer bytes, and exit status; close peer and remove the temporary file.

### TC_CFG_02 — Valid `.yaml` config accepted
*Type: Positive · Priority: High*

Title: Valid `.yaml` config accepted.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_YAML
Objective: Happy — Verify that a valid YAML configuration is parsed and applied.
Steps:
   - 1. Create `$TEST_TMPDIR/config.yaml` with protocol `TCP`, role `CLIENT`, host `127.0.0.1`, an ephemeral peer port, mode `ONETIME`, and payload `PING`.
   - 2. Start an in-process TCP echo peer on the configured port and launch Connix with `--config $TEST_TMPDIR/config.yaml`.
   - 3. Capture parser selection, peer bytes, event trace, and process exit status.

Expectations:
   - 1. The YAML file is valid and the peer is ready before launch.
   - 2. The YAML parser is selected; Connix applies the CLIENT configuration and sends `PING` to the peer.
   - 3. The peer returns `PING`, the cycle succeeds, and the process exits successfully.

TestData:
   - YAML: `protocol: TCP`, `role: CLIENT`, `host: 127.0.0.1`, `port: <ephemeral-port>`, `mode: ONETIME`, `payload: PING`.
   - Capture parser selection and lifecycle events; close peer and remove the temporary file.

### TC_CFG_03 — `.yml` extension treated as YAML
*Type: Positive · Priority: Medium*

Title: `.yml` extension treated as YAML.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_YAML
   - SW_REQ_CONFIGURATION_PARSER_SELECTION
Objective: Happy — Verify that the `.yml` extension selects the YAML parser.
Steps:
   - 1. Create `$TEST_TMPDIR/config.yml` containing the minimal YAML CLIENT configuration for a local TCP echo exchange with payload `PING`.
   - 2. Start the echo peer and launch Connix with `--config $TEST_TMPDIR/config.yml`.
   - 3. Capture parser-selection events, the peer payload, and process outcome.

Expectations:
   - 1. The `.yml` file is valid and the local peer is ready.
   - 2. Connix selects `YamlConfigParser`, not `JsonConfigParser`, solely from the `.yml` extension.
   - 3. The configuration is applied, the peer receives `PING`, and Connix exits successfully.

TestData:
   - File `$TEST_TMPDIR/config.yml`; YAML fields `protocol: TCP`, `role: CLIENT`, `host: 127.0.0.1`, `port: <ephemeral-port>`, `mode: ONETIME`, `payload: PING`.
   - Capture parser class/selection event, peer bytes, and exit status; remove file and release port.

### TC_CFG_04 — Equivalent JSON and YAML configs produce identical behavior
*Type: Positive · Priority: High*

Title: Equivalent JSON and YAML configs produce identical behavior.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_JSON
   - SW_REQ_CONFIGURATION_YAML
Objective: Happy — Verify equivalent JSON and YAML models produce the same observable exchange.
Steps:
   - 1. Start one local TCP echo peer on `127.0.0.1:<ephemeral-port>` and create `config.json` and `config.yaml` with identical CLIENT values: mode `ONETIME`, payload `PING`, and the peer address.
   - 2. Launch Connix once with `config.json` and once with `config.yaml`, recording parser selection, sent/received bytes, event trace, and exit status for each run.
   - 3. Compare the normalized results from both runs.

Expectations:
   - 1. Both files parse successfully into equivalent configuration models.
   - 2. Each run sends exactly `PING`, receives exactly `PING`, records the same lifecycle event sequence, and exits successfully.
   - 3. The only expected difference is the selected parser; behavior, payloads, and result status are identical.

TestData:
   - JSON and YAML files containing the same TCP CLIENT configuration; peer `127.0.0.1:<ephemeral-port>`.
   - Payload `PING`, mode `ONETIME`; normalized event/output records; remove both files after the runs.

### TC_CFG_05 — Invalid JSON content in `.json` file produces parse error, no fallback
*Type: Negative · Priority: High*

Title: Invalid JSON content in `.json` file produces parse error, no fallback.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_PARSE_ERROR
Objective: Error — Verify malformed JSON fails in the selected parser without fallback or side effects.
Steps:
   - 1. Create `$TEST_TMPDIR/config.json` containing `{ "role": "CLIENT",, "mode": "ONETIME" }` and prepare JSON/YAML parser spies plus a transport-factory spy.
   - 2. Load the file through the configuration entry point and capture parser calls, diagnostic text, side-effect calls, and exit status.

Expectations:
   - 1. The malformed file is written successfully and the JSON parser is selected from `.json`.
   - 2. The JSON parser reports an error naming `config.json` and the syntax location; the YAML parser and transport factory are never called, no action starts, and the process returns non-zero.

TestData:
   - Path `$TEST_TMPDIR/config.json`; malformed content `{ "role": "CLIENT",, "mode": "ONETIME" }`.
   - JSON/YAML parser spies, transport-factory spy, diagnostic capture, and non-zero exit assertion; remove the file afterward.

### TC_CFG_06 — Invalid YAML content in `.yaml` file produces parse error, no fallback
*Type: Negative · Priority: High*

Title: Invalid YAML content in `.yaml` file produces parse error, no fallback.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_PARSE_ERROR
Objective: Error — Verify malformed YAML fails in the selected parser without fallback or side effects.
Steps:
   - 1. Create `$TEST_TMPDIR/config.yaml` containing `role: CLIENT\n  protocol: TCP` with invalid indentation and prepare YAML/JSON parser spies plus a transport-factory spy.
   - 2. Load the file through the configuration entry point and capture parser calls, diagnostic text, side-effect calls, and exit status.

Expectations:
   - 1. The malformed file is written successfully and the YAML parser is selected from `.yaml`.
   - 2. The YAML parser reports an error naming `config.yaml` and the syntax location; the JSON parser and transport factory are never called, no action starts, and the process returns non-zero.

TestData:
   - Path `$TEST_TMPDIR/config.yaml`; malformed content `role: CLIENT\n  protocol: TCP`.
   - YAML/JSON parser spies, transport-factory spy, diagnostic capture, and non-zero exit assertion; remove the file afterward.

### TC_CFG_07 — Valid JSON content in a `.yaml`-named file parses successfully via the YAML parser
*Type: Positive · Priority: Medium*

Title: Valid JSON content in a `.yaml`-named file parses successfully via the YAML parser.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_PARSER_SELECTION
   - SW_REQ_CONFIGURATION_YAML
Objective: Happy — Verify extension-only selection while accepting JSON syntax as valid YAML.
Steps:
   - 1. Create `$TEST_TMPDIR/config.yaml` containing valid JSON text `{ "role": "CLIENT", "protocol": "TCP", "host": "127.0.0.1", "port": <ephemeral-port>, "mode": "ONETIME", "payload": "PING" }`.
   - 2. Start the local TCP echo peer and launch Connix with `--config $TEST_TMPDIR/config.yaml`, recording parser selection and the exchange.

Expectations:
   - 1. The file is syntactically valid JSON and also valid YAML; the TCP peer is ready before launch.
   - 2. Connix selects the YAML parser solely from `.yaml`, successfully parses the JSON syntax as YAML, completes the `PING` exchange, and does not invoke content-sniffing fallback.

TestData:
   - File `$TEST_TMPDIR/config.yaml` containing the JSON object shown in step 1; peer `127.0.0.1:<ephemeral-port>`.
   - Parser-selection spy, event/output capture, and cleanup removing the file and releasing the port.

### TC_CFG_08 — Stdin config with explicit `--format yaml` matches file-based YAML
*Type: Positive · Priority: High*

Title: Stdin config with explicit `--format yaml` matches file-based YAML.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_YAML
   - SW_REQ_CONFIGURATION_EXPLICIT_FORMAT
Objective: Happy — Verify that an explicit YAML format enables extensionless stdin configuration.
Steps:
   - 1. Start a local TCP echo peer on `127.0.0.1:<ephemeral-port>` and prepare YAML content with role `CLIENT`, mode `ONETIME`, payload `PING`, and that peer address.
   - 2. Pipe the exact YAML content to Connix stdin with `--format yaml`, capturing parser selection, peer bytes, event trace, and exit status.
   - 3. Run the same configuration from `$TEST_TMPDIR/config.yaml` and compare normalized results.

Expectations:
   - 1. The peer is ready and the file/stdin configurations contain identical values.
   - 2. Stdin input is parsed by the YAML parser, sends `PING`, receives the echo, and exits successfully.
   - 3. Stdin and file runs have identical connection behavior, payloads, event sequence, and result status.

TestData:
   - YAML: `protocol: TCP`, `role: CLIENT`, `host: 127.0.0.1`, `port: <ephemeral-port>`, `mode: ONETIME`, `payload: PING`.
   - Command input: stdin plus `--format yaml`; capture parser/events/output; remove temporary file and release port.

### TC_CFG_09 — Stdin config with explicit `--format json` works
*Type: Positive · Priority: Medium*

Title: Stdin config with explicit `--format json` works.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_JSON
   - SW_REQ_CONFIGURATION_EXPLICIT_FORMAT
Objective: Happy — Verify that an explicit JSON format enables extensionless stdin configuration.
Steps:
   - 1. Start a local TCP echo peer on `127.0.0.1:<ephemeral-port>` and prepare JSON content with role `CLIENT`, mode `ONETIME`, payload `PING`, and that peer address.
   - 2. Pipe the exact JSON content to Connix stdin with `--format json`, capturing parser selection, peer bytes, event trace, and exit status.

Expectations:
   - 1. The peer is ready and the JSON input contains all required configuration fields.
   - 2. Stdin input is parsed by the JSON parser, sends `PING`, receives the echo, records the expected cycle, and exits successfully.

TestData:
   - JSON: `{ "protocol": "TCP", "role": "CLIENT", "host": "127.0.0.1", "port": <ephemeral-port>, "mode": "ONETIME", "payload": "PING" }`.
   - Command input: stdin plus `--format json`; capture parser/events/output and release the peer port.

### TC_CFG_10 — Stdin config without `--format` flag rejected
*Type: Error · Priority: High*

Title: Stdin config without `--format` flag rejected.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_EXPLICIT_FORMAT
Objective: Error — Verify extensionless stdin input cannot be parsed without an explicit format.
Steps:
   - 1. Prepare valid JSON stdin content `{ "role": "CLIENT", "protocol": "TCP", "host": "127.0.0.1", "port": 1, "mode": "ONETIME" }` and start Connix with no config filename and no `--format` option.
   - 2. Capture parser selection calls, diagnostic output, transport-factory calls, and exit status.

Expectations:
   - 1. Connix receives the complete stdin stream but has no extension or format hint.
   - 2. Connix reports that `--format json` or `--format yaml` is required, exits non-zero, invokes neither parser nor transport factory, and performs no configured action.

TestData:
   - Stdin bytes: `{ "role": "CLIENT", "protocol": "TCP", "host": "127.0.0.1", "port": 1, "mode": "ONETIME" }`.
   - No filename and no `--format`; parser/transport spies, diagnostic capture, and exit-status assertion.

### TC_CFG_11 — Command-line flag overrides conflicting config-file setting
*Type: Positive · Priority: High*

Title: Command-line flag overrides conflicting config-file setting.
Satisfies:
   - SW_REQ_CONFIGURATION_FILE
   - SW_REQ_CONFIGURATION_CLI_PRECEDENCE
Objective: Happy — Verify CLI precedence for a concrete conflicting role value.
Steps:
   - 1. Create `$TEST_TMPDIR/config.json` with role `CLIENT`, protocol `TCP`, host `127.0.0.1`, an ephemeral peer port, mode `ONETIME`, and payload `PING`; start a TCP peer that can serve both role outcomes.
   - 2. Launch Connix with `--config $TEST_TMPDIR/config.json --role server`, capture the effective configuration and readiness output, and connect an in-process CLIENT to the configured port.
   - 3. Send `PING` to the launched process and capture its role-specific event trace, response bytes, and exit status.

Expectations:
   - 1. The JSON file is valid and records role `CLIENT`; the peer and configuration loader start successfully.
   - 2. The effective role is `SERVER`, because the CLI value overrides the file value; Connix listens rather than initiating the client connection.
   - 3. Connix accepts the in-process CLIENT, receives `PING`, completes the SERVER flow, and reports success; the file's conflicting `CLIENT` value is not used.

TestData:
   - File role `CLIENT`; CLI `--role server`; protocol `TCP`; host `127.0.0.1`; ephemeral port; mode `ONETIME`; payload/response `PING`/`PONG`.
   - Capture effective configuration, lifecycle events, peer bytes, exit status; remove file and release port.

## Notes

TC_CFG_07 was previously flagged as an open point (JSON is valid YAML); the PO confirmed this is
expected, intended behavior — no requirement change was needed, and the case is documented as
positive/expected-result rather than a defect. CLI-over-config precedence (TC_CFG_11) was
likewise an open point, now resolved and codified in the requirement text above. See
`OPEN_POINTS.md` (OP-05, OP-06) for the full PO decisions.
