Stakeholders
============

The following parties have an interest in the Connix product.

.. list-table::
   :header-rows: 1
   :widths: 22 30 24 24

   * - Stakeholder
     - Interest
     - Win Condition
     - Loss Condition

   * - **Project author / maintainer**
     - Designs, builds, and maintains all three Connix components
       (``connix``, ``connix-core``, ``connix-gui``).
     - Codebase stays clean and maintainable; the tool sees active adoption.
     - Codebase becomes hard to maintain; no external adoption.

   * - **CLI / GUI user**
     - Uses ``connix`` (via the command line or the graphical client) as a
       scriptable, automatable tool for TCP, UDP, and Unix Domain Socket
       interaction.
     - Single tool covers all supported protocol families with a
       consistent interface between CLI and GUI.
     - Still requires multiple tools for different protocols; CLI and GUI
       behave inconsistently.

   * - **Library integrator**
     - Embeds ``connix-core`` as a static library in their own C++
       application to gain programmatic access to TCP, UDP, and Unix
       Domain Socket communication.
     - Stable, well-documented public API; straightforward CMake and
       Conan v2 integration.
     - Frequent breaking API changes; difficult to link or package.
