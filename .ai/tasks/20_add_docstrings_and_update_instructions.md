# Task 20: Add Docstrings for Transport Component and Update LLM Instructions

## Objective
Add comprehensive Doxygen-style docstrings across all data structures, public interfaces, internal interfaces, and GMock mock classes in the Transport component under `connix-core/infrastructure/transport/`. Update repository instruction files (`GEMINI.md` and `CLAUDE.md`) to establish a permanent mandate requiring LLMs to always write comprehensive Doxygen docstrings when creating or modifying code.

## Scope
- `GEMINI.md`
- `CLAUDE.md`
- `.cspell/custom-dictionary.txt`
- `connix/connix-core/infrastructure/transport/api/public/`
  - `TransportProtocol.hpp`
  - `TransportState.hpp`
  - `TransportErrorCode.hpp`
  - `TransportException.hpp`
  - `TransportEndpoint.hpp`
  - `ITransport.hpp`
  - `ITransportFactory.hpp`
- `connix/connix-core/infrastructure/transport/api/internal/`
  - `SocketDomain.hpp`
  - `SocketType.hpp`
  - `SocketProtocol.hpp`
  - `SocketOption.hpp`
  - `ISocket.hpp`
- `connix/connix-core/infrastructure/transport/gmock/`
  - `MockITransport.hpp`
  - `MockITransportFactory.hpp`
  - `MockISocket.hpp`
- `.ai/tasks/20_add_docstrings_and_update_instructions.md`

## Important Decisions & Assumptions
- **Docstring Format**: Standardized on Doxygen Javadoc-style `/** ... */` comments with `@brief`, `@param`, `@return`, and `@throws` tags for all interfaces, classes, methods, constructors, destructors, and inline member comments (`/**< ... */`) for enumeration values.
- **LLM Instruction Updates**: Added explicit rules to both `GEMINI.md` (under *Code Style & Formatting*) and `CLAUDE.md` (under *Coding Standards & Docstrings*) mandating that LLM assistants must always include Doxygen docstrings whenever writing or modifying code.
- **Dictionary Updates**: Added `docstring` and `docstrings` to `.cspell/custom-dictionary.txt` under *Sphinx & Documentation*.
- **Style and Formatting**: Preserved full line length compliance ($\le 80$ characters) across all docstrings and re-verified via `clang-format`.

## Changes Made
- Added Doxygen comments to all public transport headers: `TransportProtocol.hpp`, `TransportState.hpp`, `TransportErrorCode.hpp`, `TransportException.hpp`, `TransportEndpoint.hpp`, `ITransport.hpp`, and `ITransportFactory.hpp`.
- Added Doxygen comments to all internal transport headers: `SocketDomain.hpp`, `SocketType.hpp`, `SocketProtocol.hpp`, `SocketOption.hpp`, and `ISocket.hpp`.
- Added Doxygen comments to mock class headers: `MockITransport.hpp`, `MockITransportFactory.hpp`, and `MockISocket.hpp`.
- Updated `GEMINI.md` to add the **Documentation & Docstrings** mandate under *Development & Contribution Conventions*.
- Updated `CLAUDE.md` to add the **Coding Standards & Docstrings** section.
- Added `docstring` and `docstrings` to `.cspell/custom-dictionary.txt`.
- Created task tracking log `.ai/tasks/20_add_docstrings_and_update_instructions.md`.

## Validation Performed
- Built native debug target with tests: `./scripts/build_application_native.sh` (clean).
- Ran all unit tests: `./scripts/run_unit_tests.sh` (100% tests passed, 9/9 test suites).
- Verified line coverage: `./scripts/check_coverage.sh` (100.0% line coverage).
- Ran static analysis: `./scripts/run_clang_tidy.sh` (0 warnings/errors).
- Checked formatting: `./scripts/run_clang_format.sh` (clean).
- Checked spelling: `./scripts/check_spelling.sh` (201 files checked, 0 errors) and verified `GEMINI.md` and `CLAUDE.md`.
- Built release target: `./scripts/build_application_release.sh` (clean, packaged and deployed).
