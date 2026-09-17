Runtime View
############

This section describes the principal runtime scenarios derived from the refined
requirements. The diagrams show use-case orchestration and external ports, not
implementation classes.

CLIENT connection cycle
=======================

A CLIENT cycle connects, sends one message, receives a response, and closes the
socket. Connect and send operations are bounded by five seconds; receive is
bounded by ten seconds. A timeout is reported and the affected socket is closed.

.. uml:: client_connection_cycle.puml
   :align: center
   :caption: CLIENT Connection Cycle

SERVER connection cycle
=======================

A SERVER cycle waits for an incoming connection, receives one message, sends a
response, and closes the socket. Accept and send operations are bounded by five
seconds; receive is bounded by ten seconds.

.. uml:: server_connection_cycle.puml
   :align: center
   :caption: SERVER Connection Cycle

DUAL connection cycle
=====================

A DUAL trigger starts independent CLIENT and SERVER legs concurrently. Each leg
has its own socket and closes independently. Failure of one leg does not stop
the unaffected leg.

.. uml:: dual_connection_cycle.puml
   :align: center
   :caption: DUAL Concurrent Connection Cycles

PERIODIC and rule-driven execution
==================================

PERIODIC mode emits timer events at the configured interval. An eligible timer
starts one cycle; a timer that fires while a periodic cycle remains active is
skipped. Connection and timer events can select scoped rules, but rule actions
remain within the cycle that triggered them.

.. uml:: periodic_rule_execution.puml
   :align: center
   :caption: PERIODIC Trigger and Rule Execution

Configuration startup
=====================

Configuration is selected by file extension or an explicit format for
extensionless input. Parse errors identify the source and reason, abort before
any action, and do not invoke a fallback parser. Command-line values override
file values.

.. uml:: configuration_startup.puml
   :align: center
   :caption: Configuration Loading and Validation

Runtime invariants
==================

* Every eligible trigger starts at most one complete connection cycle.
* A completed cycle leaves no connection open before the next eligible cycle.
* Timeout handling reports the operation, closes the affected socket, and then
  applies the ONETIME or PERIODIC outcome.
* DUAL legs do not share socket state or failure lifetime.
* Traffic and diagnostics are emitted through the observation port after the
  corresponding operation completes.
