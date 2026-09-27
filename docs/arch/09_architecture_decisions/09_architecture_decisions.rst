Architecture Decisions
########################

The following decisions shape the architecture derived from the refined
requirements.

Clean Architecture rings
========================

Connix separates Entities, Use Cases, Interface Adapters, and Frameworks &
Drivers. Dependencies point inward so protocol libraries, parsers, UI toolkits,
and operating-system facilities do not become dependencies of the domain or
application workflows.

Common protocol port
====================

TCP, UDP, Unix Domain Sockets, and future protocols use a common client/server
abstraction. This satisfies protocol extensibility while keeping connection
orchestration independent from transport details.

Shared use cases for CLI and GUI
================================

The CLI and optional Qt/QML GUI are adapters over the same use cases. This is
the architectural basis for the required action and result parity.

Foreground execution
====================

Connix intentionally remains attached to its invoking process. Daemonization,
detachment, and PID-file management are excluded so process supervision remains
an operating-system concern.

Built-in scheduling
===================

ONETIME and PERIODIC scheduling are implemented inside Connix. PERIODIC cycles
are non-overlapping, and DUAL legs execute concurrently with independent
lifetime and failure handling.

Socket polling mechanism
========================

Connix uses POSIX ``poll(2)`` rather than ``select(2)`` or ``epoll(7)`` for
point-in-time single-descriptor socket timeout readiness inside the transport
layer.

*   ``select(2)`` is rejected because it is fundamentally limited by
    ``FD_SETSIZE`` (typically 1024 descriptors), risking buffer overflow and
    undefined behavior if descriptor values exceed this threshold.
*   ``epoll(7)`` is rejected at the transport socket level because the socket
    abstraction performs timeout-bounded checks on a single descriptor (N = 1).
    Allocating a dedicated kernel epoll file descriptor for each socket
    introduces unnecessary descriptor allocation and resource churn with no
    performance advantage over ``poll(2)``.
*   ``poll(2)`` provides standard POSIX compliance, has no fixed descriptor
    count limitation, avoids bitmask reallocation overhead, and executes as an
    efficient O(1) check for single-descriptor polling.

Asynchronous event demultiplexing across multiple concurrent connections
remains an Application Layer responsibility, where an event loop or reactor
may utilize ``epoll(7)`` across native handles.
