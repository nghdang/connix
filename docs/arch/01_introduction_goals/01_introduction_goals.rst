Introduction and Goals
######################

Connix is a unified, event-driven networking utility for developers and QA
engineers who need repeatable interaction with networked systems. It supports
TCP, UDP, and Unix Domain Sockets through one configuration and automation
model, reducing the need to switch between protocol-specific tools.

The product consists of a command-line application (``connix``), an
embeddable C++ static library (``connix-core``), and an optional Qt/QML GUI
(``connix-gui``). The target platforms are Windows 10 64-bit and Linux
distributions providing glibc 2.31 or later.

Requirements Overview
=====================

Connix provides:

* CLIENT, SERVER, and DUAL operating roles.
* ONETIME and PERIODIC execution modes.
* Event-driven, rule-based actions scoped to a connection cycle.
* JSON and YAML configuration with deterministic parser selection.
* Traffic and diagnostic output to stdout or a configured file.
* A common protocol abstraction that permits future protocols without changing
  the core orchestration engine.
* Equivalent CLI and GUI behavior when the optional GUI is enabled.

Connix runs in the foreground. It does not provide a built-in daemon mode,
detach option, or PID-file management. Background supervision is delegated to
operating-system tooling.

Quality Goals
=============

.. list-table:: Quality Goals
   :header-rows: 1
   :widths: 12 28 60

   * - Priority
     - Goal
     - Measurable scenario
   * - 1
     - Deterministic operation
     - Invalid configuration fails before any configured action; ONETIME runs
       once and PERIODIC runs at configured eligible intervals.
   * - 2
     - Protocol extensibility
     - A new protocol can implement the common client/server interface without
       modifying the core orchestration engine.
   * - 3
     - Resource efficiency
     - PERIODIC idle CPU averages below 1%; 24-hour RSS growth is no more than
       5 MB under the specified reference scenario.
   * - 4
     - Interface consistency
     - Equivalent CLI and GUI invocations perform the same action and produce
       the same result.
   * - 5
     - Integration usability
     - A library integrator can consume ``connix-core`` through CMake and Conan
       v2 using its documented public API.

Stakeholders and Users
======================

.. list-table:: Stakeholders and Users
   :header-rows: 1
   :widths: 24 36 40

   * - Stakeholder or user
     - Interest
     - Success condition
   * - Project author / maintainer
     - Keeps the project maintainable as a single-maintainer open-source
       project.
     - Architecture and protocol boundaries permit focused changes.
   * - Network / systems developer
     - Uses CLI workflows for daily protocol debugging.
     - One consistent tool covers TCP, UDP, and UDS interaction.
   * - QA / test engineer
     - Automates repeatable protocol scenarios.
     - Rules and periodic execution produce predictable cycles.
   * - Library integrator
     - Embeds ``connix-core`` in a C++ application.
     - Public API and CMake/Conan v2 integration are stable.
   * - Non-technical / visual user
     - Uses the optional GUI without scripting.
     - GUI capabilities and results match equivalent CLI behavior.

Scope and priorities are derived from ``docs/reqs``. Security hardening and GUI
look-and-feel requirements are explicitly deferred in the current requirements
baseline.
