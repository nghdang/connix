Introduction & Goals
####################

Connix is a tool that can help users simulate a network node. It allows users to send and receive
data over TCP, UDP and Unix Domain Socket. Beside that, It also supports REST API communication.

Connix also provide core library for developers who want to integrate it to their products. It is
totally free under MIT license.


Requirements Overview
=====================

Connix shall be release in three binaries:

* `connix`: The standalone application run on command line interface.
* `connix-core`: The C++ static library for integration.
* `connix-gui`: The Qt/QML application which supports graphic user interface.

Connix is very flexible. Users can run it command by command, or declare all running steps in a rule
file. Additionally, Connix also supports daemon mode for traffic monitoring and UI for users who are
not familiar with terminal.

Quality Goals
=============

.. list-table::
   :header-rows: 1

   * - Priority
     - Quality Goal
     - Scenario
   * - 1
     - Operability
     - Non-tech users can easily use the tool.
   * - 2
     - Compatibility
     - The core library must be easily integrated to all modern C++ product.
   * - 3
     - Maintainability
     - The tool should be extendable for more protocol.

Stakeholder
===========

.. list-table::
   :header-rows: 1

   * - Role
     - Description
     - Expectation
   * - Developers
     - Users who want to integrate `connix-core` to their products.
     - The tool provides useful APIs, and does not depend on any specific C++ version.
   * - Non-tech users
     - Users who want to verify network traffic of their products.
     - The tool provides user friendly interface and does not require deep knowledge of any specific
       technology.

