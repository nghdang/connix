Performance
===========

Resource Usage
---------------

.. sw-req:: Resource Usage
    :id: SW_REQ_LOW_RESOURCE_USAGE
    :desc: The Connix SHALL minimize CPU and memory usage, particularly
        during periodic execution mode where the process runs for an
        extended duration.
    :rationale: Connix is intended to run alongside other development
        and debugging tools without becoming a resource burden on the
        host machine.

Fit Criterion: Connix idles at low CPU usage between periodic actions
and does not exhibit unbounded memory growth over a long-running
periodic session.
