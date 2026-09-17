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
