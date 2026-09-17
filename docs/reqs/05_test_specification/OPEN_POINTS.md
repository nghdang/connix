# Open Points for Product Owner Review

These items were identified while designing the qualification test specification. Each represents
either an ambiguity that could lead different testers/implementers to different (equally
"compliant") behavior, or a testability gap where a design artifact the requirement depends on
does not yet exist. None of these are blocking for the overall test spec — each affected
requirement still has a full set of best-effort test cases (see the corresponding
`SW_REQ_*_test_spec.md`) — but the specific points below need a PO decision before those test
cases can be finalized as pass/fail-deterministic.

---

## OP-01: `SW_REQ_TRAFFIC_OUTPUT` — no defined log format, levels, or verbosity

**What's unclear:** The requirement specifies *that* traffic and log output go to stdout (or a
redirected file) but not the message format, structure, log levels (e.g. INFO/WARN/ERROR), or any
verbosity control.

**Why it blocks deterministic testing:** Test cases (`TC_OUT_*`) can only assert that *some*
output appears in the right destination, not that the output has any specific, checkable content
or structure. Two conforming implementations could produce wildly different-looking output.

**Question for PO:** Should a log format/verbosity scheme be specified (e.g. a minimum set of log
levels, a structured format such as JSON lines, or at least a statement that format is
implementation-defined and not subject to conformance testing)?

**PO Answer:** Format is implementation-defined and out of scope for conformance testing. Do not
add a log-level or structured-format requirement at this time. Test cases correctly verify only
*presence* and *destination* of output, not content structure — no change needed to
`SW_REQ_TRAFFIC_OUTPUT` or its test spec.

---

## OP-02: `SW_REQ_LOW_RESOURCE_USAGE` — 24-hour soak test and reference-machine reproducibility

**What's unclear:** The fit criterion requires a 24-hour PERIODIC run at a 1-second interval on a
specific "reference 4-core x86_64 machine (matching a Conan/CI profile)." No such profile is
currently named/pinned in `conan_profiles/`, and a 24-hour test is unusual for a CI pipeline that
gates every commit.

**Why it blocks deterministic testing:** Without a named, provisioned reference machine/profile,
"below 1% CPU" and "≤5MB RSS growth" are not reproducibly verifiable — results will vary by
whatever hardware happens to run the test. A 24-hour gate also isn't practical for per-PR CI.

**Question for PO:** (1) Which specific Conan profile under `conan_profiles/` is the reference
profile? (2) Should the 24-hour soak run in a separate, periodic (e.g. nightly/weekly) test lane
rather than blocking every CI run, with a shorter accelerated proxy (e.g. 1-hour extrapolation)
used for per-commit gating?

**PO Answer:** (1) `conan_profiles/release/gcc13_x86_64` is the reference profile — Release, not
Debug, since Debug's lack of optimization and coverage instrumentation would skew CPU/memory
numbers unrepresentatively. The requirement text now names this profile explicitly. (2) Yes: the
full 24-hour/5MB soak test runs in a scheduled nightly lane, not on every commit. Per-commit CI
instead runs a 15-minute smoke proxy of the same PERIODIC/1-second-interval run, checking that RSS
growth from the 10-minute mark to the 15-minute mark stays within 1 MB, as an early-warning gate —
not a substitute for the nightly authoritative check. See `08_crosscutting_concepts.rst`'s Testing
Conventions for the recorded lane split.

---

## OP-03: `SW_REQ_CLI_GUI_CONSISTENCY` — no GUI exists yet and no capability list is defined

**What's unclear:** The requirement says the GUI must match the CLI "for every capability the GUI
exposes," but no GUI implementation exists yet, and no document enumerates which CLI capabilities
the GUI is planned to expose (all vs. a subset).

**Why it blocks deterministic testing:** Test cases can be written for capabilities we already
know exist (protocol, role, mode, rules), but the requirement can't be fully verified until there
is a definitive GUI capability list to check for gaps against.

**Question for PO:** When GUI design begins, can a capability checklist be produced (e.g. as a
GUI-specific requirement or design doc) that this test spec can be updated against?

**PO Answer:** Confirmed. When GUI design begins, a capability checklist will be produced (as a
GUI-specific requirement or design doc) and this test spec updated against it. No action needed
before then.

---

## OP-04: `SW_REQ_PROTOCOL_EXTENSIBILITY` — the referenced interface doesn't exist yet

**What's unclear:** The requirement assumes connix-core "exposes a common client/server
interface," but per `CLAUDE.md`, no `TransportLayer`/`ConnectionLayer` structure exists in the
source tree on this branch yet — only `common/logging` is built out.

**Why it blocks deterministic testing:** Test cases can describe the *verification method*
(implement a 4th protocol, diff the orchestration engine) but cannot be executed, and can't
confirm the interface will actually be "common" and stable, until the interface is designed.

**Question for PO:** Is there a target milestone/design doc for the client/server interface that
this test spec should be re-validated against once available?

**PO Answer:** Yes — `docs/arch/05_building_block_view/05_building_block_view.rst` already
specifies the target interface (`ITransport` under `communication/transport/`, namespace
`ConnixCore::Communication::Transport`), satisfying both `SW_REQ_MULTIPLE_PROTOCOLS` and
`SW_REQ_PROTOCOL_EXTENSIBILITY`. Per the module build order agreed in
`docs/arch/OPEN_POINTS_FOR_PO.md` (point 3), transport is implemented early (alongside
configuration and logging). Re-validate this test spec against `ITransport` once it lands in the
source tree; no new milestone doc is needed.

---

## OP-05: `SW_REQ_CONFIGURATION_FILE` — JSON-as-valid-YAML edge case

**What's unclear:** Valid JSON is (with rare exceptions) also valid YAML. The requirement says
parser selection is strictly by file extension, not content sniffing. This is *not* contradictory,
but it means a `.yaml`-named file containing JSON content will be successfully parsed by the YAML
parser (since JSON parses as YAML), which could look like unintended fallback behavior to an
implementer or reviewer even though it is directly a result of the specified extension-only rule.

**Why it blocks deterministic testing:** Without an explicit note, a test author might mistake
this for a bug (looks like implicit multi-format support) rather than expected behavior.

**Question for PO:** Confirm this is accepted/expected behavior (no change needed), so it can be
documented as such rather than flagged as a defect during test execution.

**PO Answer:** Confirmed accepted/expected, not a defect. `SW_REQ_CONFIGURATION_FILE` already
specifies extension-only parser selection; a `.yaml` file containing valid JSON parsing
successfully via the YAML parser is a direct, intended consequence of that rule, not implicit
format fallback. No requirement change needed — document `TC_CFG_07` as a positive/expected-result
case rather than an error case.

---

## OP-06: `SW_REQ_WORKING_ROLES` / role source precedence not specified

**What's unclear:** When role (or similarly, other settings) is specified in both a config file
and via a CLI flag with conflicting values, the requirements don't state which source wins.

**Why it blocks deterministic testing:** `TC_ROLE_06` can't have a single expected result without
a documented precedence rule.

**Question for PO:** Should CLI flags always override config file values (common convention), or
is a different precedence intended? Please confirm so this can be documented as a rule
(potentially applying to all config/CLI-overlapping settings, not just role).

**PO Answer:** CLI flags SHALL always override config file values for any setting specified in
both, following the common convention. This applies to all config/CLI-overlapping settings, not
just role. `SW_REQ_CONFIGURATION_FILE` in
`03_functional_requirements/functional_requirements.rst` has been updated with this precedence
rule; `TC_ROLE_06` can now be given a single deterministic expected result.

---

## OP-07: `SW_REQ_SIMPLE_CONNECTION_FLOW` — behavior when a rule/config attempts multiple sends per cycle

**What's unclear:** The requirement states CLIENT role sends "one message" per cycle, but doesn't
specify what happens if a user's configuration attempts to define more than one send action for a
single cycle (e.g. two rules both bound to the same connect event).

**Why it blocks deterministic testing:** `TC_FLOW_09` can't have a single expected result — reject
at config-load time vs. silently sending only the first vs. some other behavior are all plausible
readings.

**Question for PO:** Should Connix reject a configuration that would result in more than one send
within a single cycle, at config-load time?

**PO Answer:** Yes. Connix SHALL reject, at configuration-load time, any configuration that would
cause more than one send action within a single connect/send/receive/close cycle.
`SW_REQ_SIMPLE_CONNECTION_FLOW` in `03_functional_requirements/functional_requirements.rst` has
been updated with this rule; `TC_FLOW_09` can now be given a single deterministic expected result
(config-load-time rejection with a clear error).

---

## Informational (not open points — recorded for completeness)

- **Security** (`04_nonfunctional_requirements/security.rst`) and **Look and Feel**
  (`04_nonfunctional_requirements/look_and_feel.rst`) explicitly state no requirements are defined
  at this time. No test spec is produced for these sections; this is by design, not a gap.
