Purpose
=======

The Situation
-------------

Developers working with networked systems frequently switch between several
tools: ``netcat`` or ``socat`` for raw TCP/UDP sockets, and ``socat`` for Unix
Domain Sockets. Each tool carries its own syntax, output format, and mental
model, making cross-protocol scripting and debugging slow and error-prone.
Beyond tool fragmentation, none of these tools provide built-in automation:
triggering actions on events such as message receipt or timers requires
external scripting. There is also no single, lightweight, embeddable C++
library that application developers can use to gain the same protocol
coverage programmatically.

The Goal
--------

Connix addresses this gap by providing a unified networking utility
(``connix``) for TCP, UDP, and Unix Domain Sockets. Its primary goal is to
enable automation: event-driven, rule-based, scriptable network interaction
across these protocols without manual intervention. It is built on a
reusable C++20 static library, ``connix-core``, embeddable in third-party
applications that need the same programmatic access. An optional graphical
client, ``connix-gui`` (Qt/QML), extends the tool for users who prefer a
visual workflow. The product targets Windows and Linux.

Background
----------

Connix is a greenfield project with no predecessor tool or legacy codebase
to migrate from. Because all requirements are defined from scratch, the
Volere Requirements Specification Template was adopted to ensure each
requirement is traceable, testable, and accompanied by a measurable fit
criterion.
