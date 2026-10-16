# Task 39: Create Detailed Design of File System Component

## Objective
Create the comprehensive detailed design documentation and architectural
diagrams for the `filesystem` component under
`connix-core/infrastructure/filesystem/dd/`.

## Scope
- `connix/connix-core/infrastructure/filesystem/dd/use_case.puml`
- `connix/connix-core/infrastructure/filesystem/dd/static_view.puml`
- `connix/connix-core/infrastructure/filesystem/dd/data_structures.puml`
- `connix/connix-core/infrastructure/filesystem/dd/interface_view.puml`
- `connix/connix-core/infrastructure/filesystem/dd/runtime_view.puml`
- `connix/connix-core/infrastructure/filesystem/dd/detailed_design.md`
- `.cspell/custom-dictionary.txt`
- `.ai/tasks/39_create_detailed_design_file_system_component.md`

## Important Decisions & Assumptions
- Component Placement and Namespace:
  - Placed under `connix-core/infrastructure/filesystem/` matching the
    existing clean architecture structure (`configuration`, `logging`, `timer`,
    `transport`).
  - Namespace standardized as `ConnixCore::Infrastructure::Filesystem` matching
    `FilesystemConfig` and project namespace standards.
- Architectural Model & Core Modules:
  - **`FilesystemService`**: Central registry and coordinator for path watchers
    and file I/O operations (`IFilesystemService`), managing active watchers,
    assigning `watchId`, and pumping debounced `FilesystemEvent` objects to
    registered event subscribers (`FilesystemEventHandler`).
  - **`FilesystemWatcherFactory`**: Factory (`IFilesystemWatcherFactory`)
    instantiating concrete `FilesystemWatcher` instances and injecting OS
    abstractions (`IInotify`, `IFileIo`).
  - **`FilesystemWatcher`**: Encapsulates single-path monitoring
    (`IFilesystemWatcher`), kernel descriptor tracking, and debounce timing.
  - **`FileIo`**: Robust file operations (`IFileIo`) implementing text/binary
    reading (`readText`, `readBinary`), file writing/appending (`writeText`,
    `writeBinary`, `appendText`), existence checks, and parent directory
    validation.
  - **`Inotify`**: POSIX Linux `inotify(7)` abstraction barrier (`IInotify`)
    decoupling OS kernel syscalls and allowing 100% deterministic mock
    testing (`MockIInotify`).
- Requirements Traceability:
  - `SW_REQ_EVENT_HANDLING` & `SW_REQ_EVENT_HANDLING_EVENT_KINDS`: Filesystem
    modification event generation for `filesystems` config with `onModified`
    triggers.
  - `SW_REQ_RULE_BASED`: Rule matching and action triggering on filesystem
    change events.
  - `SW_REQ_TRAFFIC_VISIBILITY_FILE_REDIRECTION`,
    `SW_REQ_TRAFFIC_VISIBILITY_FILE_DESTINATION`,
    `SW_REQ_TRAFFIC_VISIBILITY_FILE_STDOUT_SUPPRESSION`: File output
    redirection with fail-fast parent directory and permission checks
    (`TC_OUT_05`, `TC_OUT_06`).
  - `SW_REQ_CONFIGURATION_FILE`, `SW_REQ_CONFIGURATION_JSON_FILE`,
    `SW_REQ_CONFIGURATION_YAML_FILE`, `SW_REQ_CONFIGURATION_UNSUPPORTED_FILE`,
    `SW_REQ_CONFIGURATION_MALFORMED_FILE`: Synchronous file loading.
  - `SW_REQ_NO_PID_FILE_MANAGEMENT`: Filesystem cleanliness verification.
- Event Debouncing:
  - Monotonic clock calculations aggregate rapid kernel events (`IN_MODIFY`,
    `IN_CLOSE_WRITE`) across configured `debounce` interval (e.g. 100 ms),
    emitting a single consolidated `FilesystemEvent` once the file is quiet.

## Changes Made
- Created `connix/connix-core/infrastructure/filesystem/dd/use_case.puml`
  defining 7 core use cases (Watch Path, Unwatch Path, Dispatch Filesystem
  Event, Read File Content, Write File Content, Manage File Properties, Stop All
  Watchers).
- Created `connix/connix-core/infrastructure/filesystem/dd/static_view.puml`
  specifying module boundaries and OS API interactions.
- Created `connix/connix-core/infrastructure/filesystem/dd/data_structures.puml`
  defining enums (`FilesystemEventType`, `FilesystemWatcherState`,
  `FilesystemErrorCode`), exception class (`FilesystemException`), and event
  class (`FilesystemEvent`).
- Created `connix/connix-core/infrastructure/filesystem/dd/interface_view.puml`
  detailing public and internal interface contracts and realizations.
- Created `connix/connix-core/infrastructure/filesystem/dd/runtime_view.puml`
  illustrating sequence workflows across 5 runtime scenarios.
- Created `connix/connix-core/infrastructure/filesystem/dd/detailed_design.md`
  documenting comprehensive specifications, requirements traceability, and
  design decisions.
- Updated `.cspell/custom-dictionary.txt` with technical terms (`inotify`,
  `INOTIFY`, `Inotify`, `CLOEXEC`, `unwatch`, `Unwatch`).
- Created work tracking record
  `.ai/tasks/39_create_detailed_design_file_system_component.md`.

## Validation Performed
- PlantUML syntax check: `plantuml -syntaxcheck` across all `.puml` files
  (Clean, 0 errors).
- Spelling check: `./scripts/check_spelling.sh` across all 273 files (Clean, 0
  issues).
- Line length check: `./scripts/check_line_length.sh` across all created design
  files (Clean, 0 violations).
- Verified `git status` to ensure clean working tree with only expected files.
