# Test Specification: SW_REQ_LOW_RESOURCE_USAGE

**Source:** `docs/reqs/04_nonfunctional_requirements/performance.rst`

## Requirement Summary

This test specification covers the child requirements under the parent
`SW_REQ_LOW_RESOURCE_USAGE`: `SW_REQ_RESOURCE_CPU_IDLE` and
`SW_REQ_RESOURCE_MEMORY_GROWTH`.

The CPU child requires average idle CPU below 1% on the reference machine built with the
`conan_profiles/release/gcc13_x86_64` profile over a 60-second window. The memory child requires
RSS growth of no more than 5 MB over a 24-hour PERIODIC run at a 1-second interval. The full run
is authoritative in a scheduled (e.g. nightly) lane; per-commit CI uses the documented 15-minute
smoke proxy as an early-warning gate, not a substitute for the nightly check.

**Fit Criterion:** Sampling CPU usage every second during a 60-second idle window on the reference
profile shows an average below 1%. Sampling RSS at the 10-minute and 24-hour marks of a
1-second-interval PERIODIC run in the nightly lane shows growth no greater than 5 MB. Sampling RSS
at the 10-minute and 15-minute marks of the same run in the per-commit lane shows growth no greater
than 1 MB.

## Test Design Techniques

- Boundary Value Analysis: CPU % threshold (1%), RSS growth thresholds (5 MB nightly, 1 MB
  per-commit smoke proxy) — test at and near the boundary.
- Long-running/soak testing (ISTQB non-functional testing practice), split across a nightly
  authoritative lane and a per-commit smoke-proxy lane.

## Test Cases

### TC_PERF_01 — Idle CPU usage below 1% over 60s window
*Type: Positive · Priority: High*

Title: Idle CPU usage below 1% over 60s window.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_CPU_IDLE
Objective: Happy — Verify that idle CPU usage below 1% over 60s window.
Steps:
   - 1. Establish the precondition: Build made with the `conan_profiles/release/gcc13_x86_64` profile; PERIODIC mode running, no triggers firing during the window.
   - 2. Start Connix in PERIODIC mode with a long interval (so no trigger fires during the sampling window).
   - 3. Sample CPU usage every second for 60 seconds while idle.
   - 4. Compute the average.

Expectations:
   - 1. Average CPU usage over the 60s window is below 1%.

TestData:
   - mode=PERIODIC, sampling interval=1s, window=60s.
   - Fixture setup required by the precondition: Build made with the `conan_profiles/release/gcc13_x86_64` profile; PERIODIC mode running, no triggers firing during the window.

### TC_PERF_02 — RSS growth over 24h PERIODIC run at 1s interval within 5 MB (nightly lane)
*Type: Positive · Priority: High*

Title: RSS growth over 24h PERIODIC run at 1s interval within 5 MB (nightly lane).
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_MEMORY_GROWTH
Objective: Happy — Verify that rSS growth over 24h PERIODIC run at 1s interval within 5 MB (nightly lane).
Steps:
   - 1. Establish the precondition: Build made with the `conan_profiles/release/gcc13_x86_64` profile; PERIODIC mode, interval=1s; runs in the scheduled nightly test lane.
   - 2. Start Connix in PERIODIC mode, interval=1s.
   - 3. Record RSS at the 10-minute mark (steady state).
   - 4. Let the run continue for 24h.
   - 5. Record RSS at the 24h mark.
   - 6. Compute the difference.

Expectations:
   - 1. RSS growth (24h RSS − 10min RSS) is ≤ 5 MB. This is the sole authoritative check for this requirement.

TestData:
   - mode=PERIODIC, interval=1s, duration=24h.
   - Fixture setup required by the precondition: Build made with the `conan_profiles/release/gcc13_x86_64` profile; PERIODIC mode, interval=1s; runs in the scheduled nightly test lane.

### TC_PERF_03 — CPU usage boundary just above 1% flagged as failing
*Type: Boundary · Priority: Medium*

Title: CPU usage boundary just above 1% flagged as failing.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_CPU_IDLE
Objective: Edge — Verify the CPU measurement harness rejects an average just above the 1% limit.
Steps:
   - 1. Build the same PERIODIC test binary with the `conan_profiles/release/gcc13_x86_64` profile and start it with a long interval so no trigger occurs during sampling; run a controlled CPU-load fixture that targets an average of `1.05%` for the Connix process.
   - 2. Sample Connix CPU usage once per second for `60s` using the approved measurement harness and calculate the arithmetic mean.
   - 3. Submit the samples to the conformance verdict calculation and record the verdict, mean, and maximum sample.

Expectations:
   - 1. The reference-profile binary and controlled load fixture start successfully, with no network trigger during the idle window.
   - 2. The harness produces exactly 60 samples and calculates a mean above `1.00%` (target `1.05%`).
   - 3. The verdict is FAIL for the CPU child because the mean exceeds the strict `<1%` requirement; the result includes the measured mean and sample count.

TestData:
   - Build profile `conan_profiles/release/gcc13_x86_64`; PERIODIC interval greater than `60s`; sampling interval `1s`; window `60s`.
   - Controlled CPU-load fixture targeting Connix mean `1.05%`; CPU sample file; threshold `<1.00%`; capture verdict and measurement metadata.

### TC_PERF_04 — RSS growth boundary just above 5 MB flagged as failing
*Type: Boundary · Priority: Medium*

Title: RSS growth boundary just above 5 MB flagged as failing.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_MEMORY_GROWTH
Objective: Edge — Verify the RSS measurement harness rejects growth just above the 5 MB limit.
Steps:
   - 1. Build the PERIODIC test binary with the `conan_profiles/release/gcc13_x86_64` profile and enable a test-only deterministic allocation fixture that retains `5.1 MB` after the 10-minute baseline.
   - 2. Run the memory harness, record RSS at the 10-minute mark and at the 24-hour test endpoint (or the approved accelerated harness endpoint), and calculate the difference.
   - 3. Submit the two RSS samples to the conformance verdict calculation and record the verdict and measured growth.

Expectations:
   - 1. The reference-profile binary and allocation fixture start successfully and the baseline RSS sample is recorded.
   - 2. Measured growth is greater than `5.00 MB` and approximately `5.10 MB` above the 10-minute baseline.
   - 3. The verdict is FAIL for the memory child because growth exceeds the strict `≤5 MB` requirement; the result contains both RSS samples and the calculated difference.

TestData:
   - Build profile `conan_profiles/release/gcc13_x86_64`; PERIODIC interval `1s`; baseline at `10min`; endpoint `24h` or approved harness equivalent.
   - Test-only retained allocation target `5.1 MB`; RSS sample file; threshold `≤5 MB`; capture verdict and measurement metadata.

### TC_PERF_05 — CPU usage during active PERIODIC triggers (not idle) is out of scope for this requirement
*Type: Boundary · Priority: Low*

Title: CPU usage during active PERIODIC triggers (not idle) is out of scope for this requirement.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_CPU_IDLE
Objective: Edge — Verify that cPU usage during active PERIODIC triggers (not idle) is out of scope for this requirement.
Steps:
   - 1. Establish the precondition: PERIODIC mode with short interval causing frequent triggers.
   - 2. Run PERIODIC mode with interval=1s (near-continuous triggering).
   - 3. Attempt to isolate "idle between triggers" CPU usage.

Expectations:
   - 1. Confirms whether/how idle-only CPU measurement is even meaningful at very short intervals — informs whether interval choice for TC_PERF_01 needs to be long enough to have a measurable idle window.

TestData:
   - mode=PERIODIC, interval=1s.
   - Fixture setup required by the precondition: PERIODIC mode with short interval causing frequent triggers.

### TC_PERF_06 — Resource usage measured on a non-reference profile is documented as non-conclusive
*Type: Negative · Priority: Low*

Title: Resource usage measured on a non-reference profile is documented as non-conclusive.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_CPU_IDLE
   - SW_REQ_RESOURCE_MEMORY_GROWTH
Objective: Error — Verify that resource usage measured on a non-reference profile is documented as non-conclusive.
Steps:
   - 1. Establish the precondition: A build not made with the `conan_profiles/release/gcc13_x86_64` profile (e.g. Debug build, developer laptop, different architecture).
   - 2. Run TC_PERF_01/02 methodology on a non-reference build/machine.
   - 3. Compare results against reference-profile results.

Expectations:
   - 1. Results are recorded as indicative only; pass/fail verdicts for this requirement are only authoritative on the `conan_profiles/release/gcc13_x86_64` profile.

TestData:
   - non-reference build.
   - Fixture setup required by the precondition: A build not made with the `conan_profiles/release/gcc13_x86_64` profile (e.g. Debug build, developer laptop, different architecture).

### TC_PERF_07 — Per-commit smoke proxy: RSS growth from 10-min to 15-min mark within 1 MB
*Type: Positive · Priority: High*

Title: Per-commit smoke proxy: RSS growth from 10-min to 15-min mark within 1 MB.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_MEMORY_GROWTH
Objective: Happy — Verify that per-commit smoke proxy: RSS growth from 10-min to 15-min mark within 1 MB.
Steps:
   - 1. Establish the precondition: Build made with the `conan_profiles/release/gcc13_x86_64` profile; PERIODIC mode, interval=1s; runs in per-commit CI.
   - 2. Start Connix in PERIODIC mode, interval=1s.
   - 3. Record RSS at the 10-minute mark.
   - 4. Let the run continue to the 15-minute mark.
   - 5. Record RSS at the 15-minute mark.
   - 6. Compute the difference.

Expectations:
   - 1. RSS growth (15min RSS − 10min RSS) is ≤ 1 MB. This is an early-warning gate on every commit, not a substitute for the nightly TC_PERF_02 check.

TestData:
   - mode=PERIODIC, interval=1s, duration=15min.
   - Fixture setup required by the precondition: Build made with the `conan_profiles/release/gcc13_x86_64` profile; PERIODIC mode, interval=1s; runs in per-commit CI.

### TC_PERF_08 — Smoke-proxy failure does not itself fail the nightly authoritative check
*Type: Boundary · Priority: Low*

Title: Smoke-proxy failure does not itself fail the nightly authoritative check.
Satisfies:
   - SW_REQ_LOW_RESOURCE_USAGE
   - SW_REQ_RESOURCE_MEMORY_GROWTH
Objective: Edge — Verify that smoke-proxy failure does not itself fail the nightly authoritative check.
Steps:
   - 1. Establish the precondition: Per-commit smoke proxy (TC_PERF_07) has flagged growth between 1 MB and 5 MB (i.e. failing the proxy but not necessarily failing the nightly 5 MB threshold).
   - 2. Trigger a TC_PERF_07 failure (growth > 1 MB but plausibly < 5 MB over 24h).
   - 3. Confirm CI treats this as an early warning requiring investigation, not an automatic requirement failure, and that the nightly TC_PERF_02 run remains the authoritative pass/fail source.

Expectations:
   - 1. The per-commit gate surfaces the regression for investigation but does not itself constitute a `SW_REQ_LOW_RESOURCE_USAGE` conformance failure; only TC_PERF_02 (nightly) does.

TestData:
   - smoke-proxy growth between 1 MB and 5 MB.
   - Fixture setup required by the precondition: Per-commit smoke proxy (TC_PERF_07) has flagged growth between 1 MB and 5 MB (i.e. failing the proxy but not necessarily failing the nightly 5 MB threshold).

## Notes

The reference profile, soak-test duration, and CI lane split were previously open questions (OP-02,
see `OPEN_POINTS.md`) about test-environment reproducibility. The PO resolved them: the reference
profile is `conan_profiles/release/gcc13_x86_64` (Release, not Debug, to avoid skewed CPU/memory
numbers from missing optimization/coverage instrumentation), the full 24-hour/5 MB soak test runs
in a scheduled nightly lane (TC_PERF_02), and per-commit CI runs a 15-minute smoke proxy
(TC_PERF_07/08) as an early-warning gate rather than a substitute. See
`08_crosscutting_concepts.rst`'s Testing Conventions for the recorded lane split.
