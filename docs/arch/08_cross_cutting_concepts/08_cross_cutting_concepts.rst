Cross-cutting Concepts
#######################

Error handling
==============

Errors are represented as operation-specific outcomes. Configuration errors
identify the source and reason and prevent execution. Connection timeouts
identify the operation, close the affected socket, and then apply the execution
mode's result policy.

Concurrency and lifecycle
=========================

A connection cycle owns its socket lifetime from connect or accept through
close. PERIODIC execution does not overlap cycles. DUAL execution uses
independent concurrent legs, sockets, and failure outcomes.

Configuration
=============

JSON and YAML are supported. File extensions select the parser; extensionless
input requires an explicit format. Command-line values override file values.
Invalid configuration is rejected before any configured action occurs.

Observability
=============

Completed sent traffic, received traffic, and diagnostic messages are emitted
through the observation boundary. Output goes to stdout by default or to a
configured file, with redirected content suppressed from stdout.

Portability
===========

Entities and Use Cases are platform-independent. Platform-specific behavior is
isolated in protocol drivers, Qt/QML, file I/O, timers, and the composition root.
Unix Domain Sockets are available where the target platform supports them.

Testing
=======

Google Test verifies entities and use cases in isolation, while integration and
scenario tests exercise protocol drivers, adapters, and runtime behavior.
