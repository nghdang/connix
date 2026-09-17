# Test Specification: SW_REQ_TRAFFIC_OUTPUT

**Source:** `docs/reqs/03_functional_requirements/functional_requirements.rst`

## Requirement Summary

This test specification covers the output children under the parent `SW_REQ_TRAFFIC_OUTPUT`:
`SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC`, `SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC`,
`SW_REQ_TRAFFIC_OUTPUT_STDOUT_DEFAULT`, `SW_REQ_TRAFFIC_OUTPUT_LOG`, and
`SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT`.

**Fit Criterion:** Each child behavior is tested independently: sent and received traffic and log
output are produced at their defined lifecycle points, appear on stdout by default, and are directed
to the selected file when configured.

## Test Design Techniques

- Equivalence Partitioning: output destination classes {default/stdout, file-redirected}.
- Error Guessing: unwritable file destination, disk full.

## Test Cases

### TC_OUT_01 — Traffic printed to stdout by default
*Type: Positive · Priority: High*

Title: Traffic printed to stdout by default.
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_STDOUT_DEFAULT
Objective: Happy — Verify sent and received traffic is emitted to stdout when no output destination is configured.
Steps:
   - 1. Start a localhost TCP echo peer on `127.0.0.1:<ephemeral-port>` and configure a Connix CLIENT in ONETIME mode with payload `PING`, without any output option.
   - 2. Run Connix with stdout and stderr captured separately; wait for the exchange and process exit.
   - 3. Search the captured stdout for the sent and received payloads and verify stderr is not the configured traffic destination.

Expectations:
   - 1. The peer starts and is ready before Connix launches.
   - 2. Connix sends `PING`, receives the echoed `PING`, and exits successfully.
   - 3. Captured stdout contains both sent and received traffic; no output-file configuration is required and the traffic is not silently discarded.

TestData:
   - Protocol `TCP`; host `127.0.0.1`; ephemeral port; role `CLIENT`; mode `ONETIME`; payload `PING`.
   - Separate stdout/stderr capture buffers; local echo peer; cleanup closes peer and releases port.

### TC_OUT_02 — Log output printed to stdout by default
*Type: Positive · Priority: High*

Title: Log output printed to stdout by default.
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_LOG
   - SW_REQ_TRAFFIC_OUTPUT_STDOUT_DEFAULT
Objective: Happy — Verify Connix diagnostic output is directed to stdout by default.
Steps:
   - 1. Start Connix with a valid minimal ONETIME CLIENT configuration and no output destination option; use a localhost TCP peer so startup, connection, and close events occur.
   - 2. Capture stdout and stderr separately from process start through successful cycle completion.
   - 3. Classify non-empty Connix diagnostic records by destination without asserting an implementation-specific format.

Expectations:
   - 1. Connix and the peer start successfully with no output redirection configured.
   - 2. Startup, connection, and close diagnostics are emitted during the run.
   - 3. Diagnostic/log output is present on stdout by default; the test does not require a particular message format or level.

TestData:
   - Minimal ONETIME CLIENT/TCP configuration; peer `127.0.0.1:<ephemeral-port>`; payload `PING`.
   - Separate stdout/stderr capture; presence-only log assertion; cleanup closes peer and releases port.

### TC_OUT_03 — Output redirected to file via config/flag
*Type: Positive · Priority: High*

Title: Output redirected to file via config/flag.
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_LOG
   - SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
Objective: Happy — Verify traffic and logs are redirected to the selected file.
Steps:
   - 1. Start a localhost TCP echo peer on `127.0.0.1:<ephemeral-port>` and choose `$TEST_TMPDIR/out.log` as a nonexistent output path.
   - 2. Launch Connix CLIENT in ONETIME mode with payload `PING` and `--output-file $TEST_TMPDIR/out.log`.
   - 3. After exit, read stdout, stderr, and `out.log` as separate byte streams.

Expectations:
   - 1. The peer starts and the output path is writable and absent before launch.
   - 2. Connix completes the `PING`/echo exchange successfully and creates the output file.
   - 3. `out.log` contains sent traffic, received traffic, and diagnostic output; stdout contains no duplicated configured traffic/log stream, except explicitly documented process-console output.

TestData:
   - TCP `127.0.0.1:<ephemeral-port>`; CLIENT/ONETIME; payload `PING`.
   - Output path `$TEST_TMPDIR/out.log`; separate stdout/stderr/file capture; remove file and release port in teardown.

### TC_OUT_04 — Redirected output file contains same content as default stdout case
*Type: Positive · Priority: Medium*

Title: Redirected output file contains same content as default stdout case.
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_LOG
   - SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
Objective: Happy — Verify redirection changes destination but not the emitted traffic/log records.
Steps:
   - 1. Run a fixed TCP CLIENT/echo-peer scenario with payload `PING` and no output option; save stdout, stderr, event trace, and exit status as the baseline.
   - 2. Repeat the identical scenario with `--output-file $TEST_TMPDIR/out.log`; save stdout, stderr, file bytes, event trace, and exit status.
   - 3. Normalize only destination-specific metadata and compare the baseline stdout records with the redirected file records.

Expectations:
   - 1. The baseline run succeeds and produces the expected sent/received traffic and diagnostics.
   - 2. The redirected run succeeds with the same event sequence, payloads, and result status.
   - 3. The normalized records in `out.log` equal the baseline stdout records; no traffic or log record is lost or duplicated by redirection.

TestData:
   - Identical TCP `127.0.0.1:<ephemeral-port>` peer, CLIENT/ONETIME configuration, payload `PING`.
   - Baseline stdout capture; redirected path `$TEST_TMPDIR/out.log`; normalization rule ignores only destination metadata; cleanup removes file and releases port.

### TC_OUT_05 — Redirect target directory does not exist
*Type: Error · Priority: Medium*

Title: Redirect target directory does not exist.
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
Objective: Error — Verify an output path with a missing parent directory fails before the cycle starts.
Steps:
   - 1. Create a unique nonexistent directory `$TEST_TMPDIR/missing-parent-<test-id>` and choose `$TEST_TMPDIR/missing-parent-<test-id>/out.log` as the output path.
   - 2. Launch Connix with a valid minimal ONETIME configuration and `--output-file` set to that path; capture diagnostics, exit status, peer activity, and file-system state.

Expectations:
   - 1. The parent directory is absent immediately before launch.
   - 2. Connix reports an output-path error naming the missing directory, exits non-zero without crashing, starts no connection cycle, and does not silently redirect elsewhere.

TestData:
   - Output path `$TEST_TMPDIR/missing-parent-<test-id>/out.log`; valid local CLIENT config with payload `PING`.
   - Capture diagnostics, exit status, peer activity, and directory/file existence; remove any created artifact.

### TC_OUT_06 — Redirect target not writable (permission denied)
*Type: Error · Priority: Medium*

Title: Redirect target not writable (permission denied).
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
Objective: Error — Verify a non-writable output target fails before traffic is emitted.
Steps:
   - 1. Create `$TEST_TMPDIR/readonly.log` with known bytes `ORIGINAL`, change its permissions to read-only for the Connix test user, and verify the user cannot append to it.
   - 2. Launch Connix with a valid local ONETIME CLIENT configuration and `--output-file $TEST_TMPDIR/readonly.log`; capture diagnostics, exit status, peer activity, and file bytes.

Expectations:
   - 1. The file exists with its original bytes and a write probe fails with permission denied.
   - 2. Connix reports a permission error, exits non-zero without crashing or starting a cycle, and leaves the file bytes exactly `ORIGINAL` with no partial output.

TestData:
   - Path `$TEST_TMPDIR/readonly.log`; initial bytes `ORIGINAL`; mode read-only for the test user.
   - Valid CLIENT/TCP config with payload `PING`; capture diagnostics, exit status, peer activity, and final file bytes; restore permissions and remove file.

### TC_OUT_07 — No traffic/log output lost when redirecting mid-run (PERIODIC mode)
*Type: Boundary · Priority: Low*

Title: No traffic/log output lost when redirecting mid-run (PERIODIC mode).
Satisfies:
   - SW_REQ_TRAFFIC_OUTPUT
   - SW_REQ_TRAFFIC_OUTPUT_SENT_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_RECEIVED_TRAFFIC
   - SW_REQ_TRAFFIC_OUTPUT_LOG
   - SW_REQ_TRAFFIC_OUTPUT_FILE_REDIRECT
Objective: Edge — Verify every PERIODIC cycle contributes output to the redirected file.
Steps:
   - 1. Start a localhost TCP echo peer and configure Connix CLIENT with mode `PERIODIC`, interval `1s`, payload template `PING-<cycle-number>`, duration `4s`, and output path `$TEST_TMPDIR/periodic.log`.
   - 2. Run Connix for exactly four cycle triggers while recording the expected cycle number, sent/received payloads, and process diagnostics.
   - 3. Stop Connix, read `periodic.log`, and count records for cycles 1 through 4 without asserting an implementation-specific log format.

Expectations:
   - 1. The peer and output file initialize successfully before the first trigger.
   - 2. Four cycles each send and receive the expected cycle-specific payload and produce their diagnostic records.
   - 3. The file contains output for all four cycles in order, with no trigger's traffic/log record missing, overwritten, or duplicated.

TestData:
   - Protocol `TCP`; localhost ephemeral port; role `CLIENT`; mode `PERIODIC`; interval `1s`; four triggers.
   - Payloads `PING-1` through `PING-4`; output `$TEST_TMPDIR/periodic.log`; cycle/output collector; cleanup stops process, closes peer, and removes file.

## Notes

The requirement does not define a log message format, log levels, or verbosity control. The PO
confirmed (OP-01, see `OPEN_POINTS.md`) that format is intentionally implementation-defined and
out of scope for conformance testing — no requirement change needed. Test cases here correctly
verify only *presence* and *destination* of output, not its exact structure/content format.
