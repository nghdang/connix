Performance
===========

.. sw-req:: Performance Resource Usage
    :id: [$SW_REQ_NF PERFORMANCE_RESOURCE_USAGE]
    :desc: On the reference configuration, Connix SHALL keep average idle CPU usage
           below 1% and limit resident-memory growth to 5 MB or less over a
           24-hour PERIODIC run.

.. sw-req:: Performance Resource Usage Idle CPU
    :id: [$SW_REQ_NF PERFORMANCE_RESOURCE_USAGE_IDLE_CPU]
    :desc: Connix SHALL maintain average CPU usage below 1% while idling between
           periodic triggers on a reference 4-core x86_64 machine built with the
           ``conan_profiles/release/gcc13_x86_64`` profile during a 60-second
           PERIODIC idle window.

.. sw-req:: Performance Resource Usage Resident Memory Growth
    :id: [$SW_REQ_NF PERFORMANCE_RESOURCE_USAGE_RESIDENT_MEMORY_GROWTH]
    :desc: Connix SHALL limit resident-memory growth to 5 MB or less during
           PERIODIC execution on a 1-second trigger interval, measured from
           steady-state RSS after the first 10 minutes to RSS after 24 hours.
