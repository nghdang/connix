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
     - Designs, builds, and maintains all three Connix components (``connix``, ``connix-core``,
       ``connix-gui``).
     - Codebase stays clean and maintainable; the tool sees active adoption; contributions are easy
       to accept.
     - Codebase becomes hard to maintain; no external adoption; contributor friction is high.

   * - **CLI user** (network or systems developer)
     - Uses ``connix`` daily as a scriptable replacement for ``curl``, ``netcat``, and ``socat``.
     - Single tool covers all three protocol families with a consistent interface; fast enough for
       production scripting.
     - Still requires multiple tools for different protocols; ``connix`` is slower or harder to
       use than the tools it replaces.

   * - **Library consumer** (application integrator)
     - Embeds ``connix-core`` as a static library in their own C++ application to gain programmatic
       access to REST, TCP/UDP, and Unix Domain Socket communication.
     - Stable, well-documented public API; straightforward CMake and Conan v2 integration; no
       unexpected breaking changes between releases.
     - Frequent breaking API changes; poor documentation; difficult to link or package.

   * - **GUI user**
     - Uses ``connix-gui`` (Qt/QML) for visual, interactive exploration of networked services
       without writing command-line invocations.
     - GUI exposes the full capability of ``connix-core``; behavior is consistent with the CLI;
       runs reliably on Windows and Linux.
     - GUI lags behind CLI features; crashes or freezes during use; missing support for one or
       more protocol families.

   * - **Open-source community**
     - May reuse ``connix-core`` in other projects or contribute bug fixes and features upstream.
     - MIT license allows reuse without restriction; project documentation is sufficient to
       onboard contributors; maintainer is responsive to pull requests.
     - Project is abandoned or unresponsive; license or contribution process is unclear; code is
       not structured for external contribution.
