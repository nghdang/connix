Purpose
=======

The Situation
-------------

Developers working with networked systems frequently switch between several tools: ``curl`` for
REST APIs, ``netcat`` or ``socat`` for raw TCP/UDP sockets, and ``socat`` for Unix Domain Sockets.
Each tool carries its own syntax, output format, and mental model, making cross-protocol scripting
and debugging slow and error-prone. There is no single, lightweight, composable tool that addresses
all three protocol families under a uniform interface.

The Goal
--------

Connix addresses this gap by providing a unified networking utility (``connix``) for REST APIs,
TCP/UDP, and Unix Domain Sockets. It is built on a reusable C++20 static library, ``connix-core``,
embeddable in third-party applications that need programmatic access to the same protocol set. An
optional graphical client, ``connix-gui`` (Qt/QML), extends the tool for users who prefer a visual
workflow. The product targets Windows and Linux.

Background
----------

Connix is a greenfield project with no predecessor tool or legacy codebase to migrate from. Because
all requirements are defined from scratch, the Volere Requirements Specification Template was
adopted to ensure each requirement is traceable, testable, and accompanied by a measurable fit
criterion.
