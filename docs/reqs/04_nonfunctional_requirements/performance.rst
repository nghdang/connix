Performance
===========

Resource Usage
---------------

.. sw-req:: Resource Usage
    :id: SW_REQ_LOW_RESOURCE_USAGE
    :desc: The Connix SHALL satisfy the CPU and memory resource limits
        defined by the linked child requirements.
    :rationale: Keeps resource usage objectively pass/failable through
        measurable thresholds rather than reviewer judgment.

.. sw-req:: Idle CPU Usage
    :id: SW_REQ_RESOURCE_CPU_IDLE
    :links: SW_REQ_LOW_RESOURCE_USAGE
    :desc: Connix SHALL maintain average CPU usage below 1% while idling
        between periodic triggers on a reference 4-core x86_64 machine built
        with the ``conan_profiles/release/gcc13_x86_64`` profile, measured over
        a 60-second idle window during PERIODIC execution.
    :rationale: Prevents an idle periodic process from becoming a resource
        burden on the host machine.

Fit Criterion: Sampling CPU usage every second during a 60-second idle
window with no triggers firing shows an average below 1%.

.. sw-req:: Resident Memory Growth
    :id: SW_REQ_RESOURCE_MEMORY_GROWTH
    :links: SW_REQ_LOW_RESOURCE_USAGE
    :desc: Connix SHALL limit resident-memory growth to 5 MB or less during
        PERIODIC execution on a 1-second trigger interval, measured from
        steady-state RSS after the first 10 minutes to RSS after 24 hours.
    :rationale: Prevents unbounded memory growth during long-running
        periodic execution.

Fit Criterion: Sampling RSS at the 10-minute and 24-hour marks of a
1-second-interval PERIODIC run shows growth no greater than 5 MB. The
full 24-hour run is authoritative and is exercised in a scheduled (e.g.
nightly) test lane rather than gating every commit.
