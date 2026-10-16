# Detailed Design: Component Filesystem

This document specifies the detailed design for the Filesystem module inside
`connix-core/infrastructure/filesystem/`.

## 1. Introduction

The filesystem module is a foundational infrastructure component responsible for
providing unified filesystem event monitoring and robust, safe file I/O
operations across Connix. It encapsulates Linux kernel `inotify(7)` notification
facilities and operating-system file operations, provides deterministic event
debouncing to eliminate duplicate burst triggers, and adheres to
**SOLID principles**, **Clean Code** standards, and **Clean Architecture**.

The component satisfies:
- **`SW_REQ_EVENT_HANDLING`**: Trigger configured actions from message-receipt,
  connect, accept, close, timer, and filesystem events.
- **`SW_REQ_EVENT_HANDLING_EVENT_KINDS`**: Recognize filesystem modification
  events when the corresponding file changes occur on monitored paths.
- **`SW_REQ_RULE_BASED`**: Support rules that associate one triggering event
  (including filesystem modification) with one send or receive action.
- **`SW_REQ_TRAFFIC_VISIBILITY_FILE_REDIRECTION`**: Support redirecting traffic
  visibility output to a configured output file.
- **`SW_REQ_TRAFFIC_VISIBILITY_FILE_DESTINATION`**: Append or write traffic
  events to the configured output file during a run.
- **`SW_REQ_TRAFFIC_VISIBILITY_FILE_STDOUT_SUPPRESSION`**: Suppress traffic
  visibility output on stdout when file redirection is configured.
- **`SW_REQ_CONFIGURATION_FILE`**: Load configuration from a file.
- **`SW_REQ_CONFIGURATION_JSON_FILE`**: Accept a configuration file in JSON
  format.
- **`SW_REQ_CONFIGURATION_YAML_FILE`**: Accept a configuration file in YAML
  format.
- **`SW_REQ_CONFIGURATION_UNSUPPORTED_FILE`**: Reject configuration files not
  in JSON or YAML format.
- **`SW_REQ_CONFIGURATION_MALFORMED_FILE`**: Reject malformed configuration
  files.
- **`SW_REQ_NO_PID_FILE_MANAGEMENT`**: Connix SHALL NOT create or manage a PID
  file during a run (ensuring filesystem purity and no background daemon
  footprint).

---

## 2. Use Cases

The primary use cases supported by the Filesystem component are modeled in

```plantuml
!include use_case.puml
```

### 2.1 Use Case Specifications

1. **`Watch Path (UC1)`**:
   - Executed by an external caller (`Client`), such as the application
     engine, execution scheduler, or rule coordinator, during initialization
     from the `filesystems` configuration.
   - Requests path monitoring via `IFilesystemService::watch(path, debounce)`
     or instantiates a watcher via
     `IFilesystemWatcherFactory::createWatcher(path, debounce, callback)`.
   - The factory instantiates a concrete `FilesystemWatcher` targeting the
     specified path with a configured `DebounceDuration`, injecting the
     underlying `IInotify` and `IFileIo` abstractions.
   - Arms the watcher via Linux `inotify(7)` for `IN_MODIFY` and
     `IN_CLOSE_WRITE` masks, transitioning state to
     `FilesystemWatcherState::WATCHING`.
   - If the path does not exist, raises a typed `FilesystemException` with
     `FilesystemErrorCode::PATH_NOT_FOUND`.
   - If inotify registration fails (e.g. system watch limit reached), raises
     `FilesystemException` with `FilesystemErrorCode::WATCH_FAILED`.
   - Returns a managed `watchId` string to the caller for lifecycle tracking.
   - Traceability: `SW_REQ_EVENT_HANDLING`, `SW_REQ_EVENT_HANDLING_EVENT_KINDS`,
     `SW_REQ_RULE_BASED`.

2. **`Unwatch Path (UC2)`**:
   - Executed by `Client` during rule teardown, connection cycle completion,
     or dynamic reconfiguration.
   - Invokes `IFilesystemService::unwatch(watchId)`.
   - If `watchId` is not found, raises `FilesystemException` with
     `FilesystemErrorCode::WATCH_NOT_FOUND`.
   - Stops the watcher, disarms the `inotify` watch descriptor, cancels any
     pending debounce timer, and removes the entry from the active registry.
   - Traceability: `SW_REQ_EVENT_HANDLING`.

3. **`Dispatch Filesystem Event (UC3)`**:
   - Initiated autonomously when the operating system kernel notifies the
     underlying `IInotify` descriptor of a file modification event.
   - `FilesystemWatcher` receives the notification and starts or restarts its
     internal debounce timer (`deadline = now + debounce`).
   - Discards intermediate duplicate kernel write events occurring within the
     debounce window.
   - Upon debounce expiration without further writes, `FilesystemWatcher`
     invokes the registered notification callback.
   - `FilesystemService` constructs a strongly typed `FilesystemEvent`
     (`watchId`, `path`, `eventType`, `timestamp`) and pumps it to the
     registered `FilesystemEventHandler`.
   - Traceability: `SW_REQ_EVENT_HANDLING`, `SW_REQ_EVENT_HANDLING_EVENT_KINDS`,
     `SW_REQ_RULE_BASED`.

4. **`Read File Content (UC4)`**:
   - Executed by `Client` to load configuration files, validation schemas, or
     action payloads with `type: FILE`.
   - Invokes `IFileIo::readText(path)` to retrieve file contents as a
     `std::string`, or `IFileIo::readBinary(path)` to retrieve bytes as a
     `std::vector<uint8_t>`.
   - Validates that the target path exists and points to a regular file.
   - If path does not exist, raises `FilesystemException` with
     `FilesystemErrorCode::FILE_NOT_FOUND`.
   - If path is a directory or special device, raises `FilesystemException`
     with `FilesystemErrorCode::NOT_A_REGULAR_FILE`.
   - If file cannot be opened (e.g. permission denied) or read fails, raises
     `FilesystemException` with `FilesystemErrorCode::PERMISSION_DENIED` or
     `FilesystemErrorCode::FILE_READ_FAILED`.
   - Traceability: `SW_REQ_CONFIGURATION_FILE`,
     `SW_REQ_CONFIGURATION_JSON_FILE`, `SW_REQ_CONFIGURATION_YAML_FILE`,
     `SW_REQ_CONFIGURATION_UNSUPPORTED_FILE`,
     `SW_REQ_CONFIGURATION_MALFORMED_FILE`.

5. **`Write File Content (UC5)`**:
   - Executed by `Client` for traffic visibility output redirection or logging
     to disk (`--output-file`).
   - Invokes `IFileIo::writeText(path, content)`, `writeBinary(path, data)`,
     or `appendText(path, content)`.
   - Validates that the parent directory exists before attempting write/append
     operations.
   - If the parent directory is missing, raises `FilesystemException` with
     `FilesystemErrorCode::PATH_NOT_FOUND`, allowing the engine to abort
     before starting connection cycles (satisfying `TC_OUT_05`).
   - If target path lacks write permissions, raises `FilesystemException` with
     `FilesystemErrorCode::PERMISSION_DENIED` (satisfying `TC_OUT_06`).
   - If disk write fails, raises `FilesystemException` with
     `FilesystemErrorCode::FILE_WRITE_FAILED`.
   - Traceability: `SW_REQ_TRAFFIC_VISIBILITY_FILE_REDIRECTION`,
     `SW_REQ_TRAFFIC_VISIBILITY_FILE_DESTINATION`,
     `SW_REQ_TRAFFIC_VISIBILITY_FILE_STDOUT_SUPPRESSION`.

6. **`Manage File Properties (UC6)`**:
   - Executed by `Client` to safely query filesystem metadata and manage paths
     without throwing unhandled standard library exceptions.
   - Provides non-throwing queries: `exists(path)`, `isRegularFile(path)`,
     `isDirectory(path)`, and `getFileSize(path)`.
   - Provides directory creation (`createDirectories(path)`) and file removal
     (`removeFile(path)`).
   - Traceability: `SW_REQ_NO_PID_FILE_MANAGEMENT`,
     `SW_REQ_CONFIGURATION_FILE`.

7. **`Stop All Watchers (UC7)`**:
   - Executed by `Client` during engine shutdown or execution mode transitions.
   - Invokes `IFilesystemService::stopAll()`.
   - Disarms and halts all active watchers simultaneously, closing inotify
     descriptors and guaranteeing zero lingering callbacks.
   - Traceability: `SW_REQ_EVENT_HANDLING`.

---

## 3. Static View

The static relationship of modules and interfaces within the Filesystem
component is detailed in

```plantuml
!include static_view.puml
```

### 3.1 Module Responsibilities

1. **`Client`**:
   - External consumer of the filesystem component (application engine,
     configuration loader, traffic visibility presenter, rule engine, or
     execution scheduler).
   - Manages and registers file change monitors via `IFilesystemService` or
     performs synchronous file operations via `IFileIo`.

2. **`FilesystemService`**:
   - Implements `IFilesystemService` as the central coordinator and registry
     for active filesystem monitors within `connix-core`.
   - Manages the collection of active `IFilesystemWatcher` instances,
     assigning and tracking unique watch identifiers (`watchId`).
   - Pushes strongly typed `FilesystemEvent` objects to registered listeners
     (`FilesystemEventHandler`) upon confirmed file changes.
   - Exposes access to the unified `IFileIo` interface for synchronous file
     reading and writing.
   - Provides bulk lifecycle teardown (`stopAll()`) during application shutdown.

3. **`FilesystemWatcherFactory`**:
   - Implements the Factory pattern (`IFilesystemWatcherFactory`) to instantiate
     concrete `FilesystemWatcher` instances.
   - Decouples watcher construction from callers, injecting the underlying
     `IInotify` and `IFileIo` abstractions.
   - Enables mock injection during testing, allowing test doubles to simulate
     filesystem events without touching physical disks.

4. **`FilesystemWatcher`**:
   - Implements `IFilesystemWatcher` to monitor an individual file or directory
     path for modification events.
   - Manages the kernel watch descriptor via `IInotify`.
   - Enforces debounce timing: aggregates rapid kernel events (`IN_MODIFY`,
     `IN_CLOSE_WRITE`) and triggers its callback only after the configured
     debounce interval elapses with no subsequent modifications.

5. **`FileIo`**:
   - Implements `IFileIo` providing robust, safe file I/O operations using C++17
     `<filesystem>` and `<fstream>` primitives.
   - Implements strict validation: verifies file existence, regular file types,
     parent directory existence, and access permissions.
   - Translates operating-system and standard library errors into typed
     `FilesystemException` instances.

6. **`Inotify`**:
   - Encapsulates Linux `inotify(7)` system call primitives (`inotify_init1`,
     `inotify_add_watch`, `inotify_rm_watch`, `read`, `close`).
   - Implements `IInotify` to establish an abstraction barrier (Dependency
     Inversion Principle) between watcher logic and OS kernel syscalls.
   - Enables deterministic unit testing via Google Mock (`MockIInotify`)
     without depending on operating-system filesystem events.

7. **`Operating System (Linux inotify / VFS APIs, POSIX File I/O APIs)`**:
   - The underlying Linux platform kernel notification facilities
     (`<sys/inotify.h>`) and standard POSIX file I/O system calls (`open`,
     `read`, `write`, `stat`).

### 3.2 Design Principles & Architectural Alignment

- **Clean Architecture:** `Filesystem` belongs to the Infrastructure /
  Frameworks & Drivers layer (`connix-core/infrastructure/filesystem/`). It
  realizes inward-facing interfaces (`IFilesystemService`,
  `IFilesystemWatcherFactory`, `IFilesystemWatcher`, `IFileIo`), isolating
  low-level Linux `inotify(7)` syscalls and file stream details from Domain
  Entities and Application Use Cases.
- **Single Responsibility Principle (SRP):** `FilesystemService` manages
  watcher registration, lookup, and event dispatch; `FilesystemWatcherFactory`
  isolates object instantiation and dependency wiring; `FilesystemWatcher`
  encapsulates single-path monitoring and event debouncing; `FileIo` isolates
  file reading, writing, and path querying; `Inotify` isolates kernel
  notification syscalls.
- **Open-Closed Principle (OCP):** Additional notification backends (such as
  BSD `kqueue` or Windows `ReadDirectoryChangesW`) can be supported by
  providing alternative implementations of `IInotify` without modifying
  the core watcher logic or application consumers.
- **Dependency Inversion Principle (DIP):** Callers depend exclusively on
  abstract interfaces (`IFilesystemService`, `IFileIo`). Watchers depend on
  the abstract `IInotify` interface, enabling deterministic mock testing with
  Google Mock (`MockIInotify`, `MockIFileIo`) with zero disk access.
- **Zero `std::optional` Mandate:** Conforms strictly to the project convention
  avoiding `std::optional` through explicit states
  (`FilesystemWatcherState::STOPPED`, `WATCHING`, `ERROR`), concrete types, and
  well-defined default return values.

---

## 4. Interface View

Details of core object behaviors, public/internal interfaces, concrete
implementations, and dependency relationships are modeled in

```plantuml
!include interface_view.puml
```

### 4.1 Interface Specifications

1. **`IFilesystemWatcher` (Public Interface)**:
   - Represents an individual path monitor supporting lifecycle control,
     debounce timing, and change notifications.
   - **Lifecycle Management**:
     - `start()`: Arms the kernel watch and transitions state to
       `FilesystemWatcherState::WATCHING`. Throws `FilesystemException` on
       failure.
     - `stop()`: Disarms the kernel watch, halts pending debounce timers, and
       transitions state to `FilesystemWatcherState::STOPPED`. Idempotent.
     - `isWatching()`: Returns `true` if state is `WATCHING`.
     - `setCallback(FilesystemCallback callback)`: Registers the callback
       invoked upon confirmed, debounced file modification.
   - **State Inspection**:
     - `getPath()`: Returns the monitored path (`const std::string&`).
     - `getDebounce()`: Returns configured debounce duration in milliseconds.
     - `getState()`: Returns current `FilesystemWatcherState`.

2. **`IFilesystemWatcherFactory` & `FilesystemWatcherFactory`**
   **(Public Interface & Implementation)**:
   - Factory for constructing concrete `FilesystemWatcher` instances, hiding
     allocation and dependency wiring from callers.
   - `createWatcher(path, debounce, callback)`: Instantiates and returns a
     `std::shared_ptr<IFilesystemWatcher>`. Accepts `const std::string& path`,
     `std::chrono::milliseconds debounce`, and `FilesystemCallback callback`.
   - Injects shared `IInotify` and `IFileIo` instances into created watchers.

3. **`IFileIo` & `FileIo` (Public Interface & Implementation)**:
   - Provides safe, validated file operations and path inspections.
   - **File Reading**:
     - `readText(const std::string& path)`: Reads entire file as `std::string`.
       Throws `FilesystemException` if file is missing, unreadable, or not a
       regular file.
     - `readBinary(const std::string& path)`: Reads entire file as
       `std::vector<uint8_t>`. Throws `FilesystemException` on error.
   - **File Writing & Redirection**:
     - `writeText(const std::string& path, const std::string& content)`: Writes
       string to path, overwriting existing content. Validates that the parent
       directory exists; throws `FilesystemException` if missing or denied.
     - `writeBinary(const std::string& path, const std::vector<uint8_t>& data)`:
       Writes raw bytes to path. Validates parent directory.
     - `appendText(const std::string& path, const std::string& content)`:
       Appends string to target path. Validates parent directory.
   - **Path & Metadata Management**:
     - `exists(const std::string& path)`: Returns `true` if path exists.
     - `isRegularFile(const std::string& path)`: Returns `true` if regular file.
     - `isDirectory(const std::string& path)`: Returns `true` if directory.
     - `getFileSize(const std::string& path)`: Returns size in bytes.
     - `createDirectories(const std::string& path)`: Recursively creates
       directories. Returns `true` on success.
     - `removeFile(const std::string& path)`: Deletes file. Returns `true` on
       success.

4. **`IFilesystemService` & `FilesystemService`**
   **(Public Interface & Implementation)**:
   - Central registry and event source for filesystem events within
     `connix-core`.
   - **Registry & Lifecycle Management**:
     - `watch(const std::string& path, std::chrono::milliseconds debounce)`:
       Creates and arms a watcher for the path, assigns a unique `watchId`
       (string), binds internal event handling, and stores the instance in its
       registry. Returns `watchId`.
     - `unwatch(const std::string& watchId)`: Stops and removes the watcher.
       Throws `FilesystemException` with `WATCH_NOT_FOUND` if ID is unknown.
     - `getWatcher(const std::string& watchId)`: Retrieves registered watcher.
       Throws `FilesystemException` with `WATCH_NOT_FOUND` if unknown.
     - `hasWatcher(const std::string& watchId)`: Returns `true` if registered.
     - `stopAll()`: Halts all registered watchers simultaneously.
     - `setEventHandler(FilesystemEventHandler handler)`: Registers subscriber
       callback for emitted `FilesystemEvent` objects.
     - `getFileIo()`: Returns reference to underlying `IFileIo` utility.

5. **`IInotify` & `Inotify` (Internal Interface & Implementation)**:
   - Establishes an OS-agnostic kernel notification boundary (Dependency
     Inversion Principle).
   - `init()`: Initializes inotify instance via
     `inotify_init1(IN_NONBLOCK | IN_CLOEXEC)`.
   - `addWatch(const std::string& path, uint32_t mask)`: Registers path watch.
     Returns watch descriptor (`wd`).
   - `rmWatch(int watchDescriptor)`: Removes registered watch descriptor.
   - `readEvents(std::vector<uint8_t>& buffer)`: Reads available raw event
     structures into buffer.
   - `close()`: Closes the inotify file descriptor.

6. **`FilesystemWatcher` (Internal Implementation)**:
   - Realizes `IFilesystemWatcher` monitoring a single path.
   - Manages debounce timing using monotonic time calculations.
   - Translates raw inotify events into high-level callbacks.

---

## 5. Data Structures

The data structures, enums, exception types, and their relationships are
modeled in

```plantuml
!include data_structures.puml
```

### 5.1 Data Structure Specifications

1. **`FilesystemEventType` (Public Enum)**:
   - Strongly typed enumeration classifying filesystem event kinds:
     - `MODIFIED`: Content was written, appended, or flushed to the file.
     - `CREATED`: Target file or directory was newly created.
     - `DELETED`: Monitored file or directory was removed.
     - `ATTRIBUTES_CHANGED`: File metadata, permissions, or ownership changed.

2. **`FilesystemWatcherState` (Public Enum)**:
   - Represents the current execution lifecycle state of a watcher:
     - `STOPPED`: Watcher is disarmed and inactive.
     - `WATCHING`: Watcher is actively monitoring path via kernel notifications.
     - `ERROR`: Watcher encountered an unrecoverable kernel or descriptor error.

3. **`FilesystemErrorCode` & `FilesystemException` (Public Types)**:
   - `FilesystemErrorCode`: Strongly typed enumeration of granular filesystem
     error states:
     - `FILE_NOT_FOUND`: Target file does not exist on disk.
     - `PATH_NOT_FOUND`: Specified directory path or parent directory does not
       exist.
     - `NOT_A_REGULAR_FILE`: Path points to a directory, socket, or device node
       when a regular file was required.
     - `NOT_A_DIRECTORY`: Path is not a directory.
     - `PERMISSION_DENIED`: Operation prohibited by OS access permissions.
     - `FILE_READ_FAILED`: Failed to open or read file contents.
     - `FILE_WRITE_FAILED`: Failed to open, write, or flush data to file.
     - `WATCH_FAILED`: Failed to add inotify watch to path.
     - `WATCH_NOT_FOUND`: Specified watch identifier does not exist in registry.
     - `WATCH_ALREADY_EXISTS`: Path is already monitored with identical ID.
     - `INOTIFY_INIT_FAILED`: Failed to allocate OS inotify file descriptor.
     - `DEBOUNCE_TIMER_FAILED`: Debounce scheduling encountered a fatal error.
   - `FilesystemException`: Standard C++ exception (`std::runtime_error`)
     holding a concrete `FilesystemErrorCode` and diagnostic message.

4. **`FilesystemEvent` (Public Class)**:
   - Represents a strongly typed filesystem event emitted by `FilesystemService`
     to the engine:
     - `m_watchId`: `std::string` identifying the watcher source.
     - `m_path`: `std::string` of the changed filesystem path.
     - `m_eventType`: `FilesystemEventType` (`MODIFIED`, etc.).
     - `m_timestamp`: `std::chrono::steady_clock::time_point` event timestamp.
   - Methods: `getWatchId()`, `getPath()`, `getEventType()`, `getTimestamp()`.

5. **Type Aliases**:
   - `FilesystemCallback`: `std::function<void(const FilesystemEvent&)>` —
     signature of low-level watcher notification handlers.
   - `FilesystemEventHandler`: `std::function<void(const FilesystemEvent&)>` —
     signature of central filesystem event subscribers.
   - `DebounceDuration`: `std::chrono::milliseconds` — standard debounce time
     resolution.

---

## 6. Runtime View

The runtime interactions between Filesystem modules, the operating system, and
external clients during watcher registration, debounced event pumping, file I/O,
and teardown are modeled in

```plantuml
!include runtime_view.puml
```

### 6.1 Runtime Sequence Specifications

1. **Scenario 1: Watcher Creation and Registration**:
   - `Client` requests `FilesystemService::watch(path, debounce)`.
   - `FilesystemService` delegates to
     `FilesystemWatcherFactory::createWatcher(path, debounce, callback)`.
   - `FilesystemWatcherFactory` injects shared `IInotify` and `IFileIo`
     abstractions and instantiates a concrete `FilesystemWatcher`.
   - `FilesystemService` calls `watcher->start()`.
   - `FilesystemWatcher` invokes
     `Inotify::addWatch(path, IN_MODIFY | IN_CLOSE_WRITE)`.
   - OS returns kernel watch descriptor; watcher transitions to
     `FilesystemWatcherState::WATCHING`.
   - `FilesystemService` stores watcher in active registry and returns managed
     `watchId` to `Client`.

2. **Scenario 2: Filesystem Modification Event and Debounce Processing**:
   - Monitored file is modified by an external process.
   - Linux kernel delivers `IN_MODIFY` notification via `Inotify`.
   - `FilesystemWatcher` receives event and arms/resets its debounce deadline
     (`deadline = now + debounce`).
   - If subsequent burst writes occur during the debounce window, deadline is
     refreshed without dispatching premature events.
   - When monotonic clock reaches deadline without further write activity,
     `FilesystemWatcher` fires its internal callback.
   - `FilesystemService` creates `FilesystemEvent(watchId, path, MODIFIED, ts)`
     and dispatches it to `FilesystemEventHandler`.
   - `Client` receives event in strict FIFO order alongside socket and timer
     events, triggering `onModified` action rules.

3. **Scenario 3: File Reading (Text & Binary)**:
   - `Client` calls `FileIo::readText(filePath)` or `readBinary(filePath)`.
   - `FileIo` verifies path exists (`exists()`) and is a regular file
     (`isRegularFile()`).
   - If missing, throws `FilesystemException` with `FILE_NOT_FOUND`.
   - If directory, throws `FilesystemException` with `NOT_A_REGULAR_FILE`.
   - Reads file data into memory and returns `std::string` or
     `std::vector<uint8_t>` to `Client`.

4. **Scenario 4: Output File Writing & Redirection**:
   - `Client` calls `FileIo::appendText(outputPath, logData)` for traffic
     visibility logging.
   - `FileIo` checks if parent directory exists.
   - If parent directory does not exist, throws `FilesystemException` with
     `PATH_NOT_FOUND`, preventing execution cycle launch (`TC_OUT_05`).
   - `FileIo` opens file stream; if access denied, throws `FilesystemException`
     with `PERMISSION_DENIED` (`TC_OUT_06`).
   - Appends data and flushes stream to disk.

5. **Scenario 5: Watcher Teardown and Bulk Teardown**:
   - **Individual Teardown**: `Client` calls
     `FilesystemService::unwatch(watchId)`. `FilesystemWatcher::stop()` invokes
     `Inotify::rmWatch(wd)`, disarms the kernel watch, transitions state to
     `STOPPED`, and removes the entry.
   - **Bulk Teardown**: During engine shutdown, `Client` calls
     `FilesystemService::stopAll()`. Service iterates over all registered
     watchers and stops each, releasing all kernel inotify resources
     cleanly.

---

## 7. Design Decisions

### 7.1 Linux `inotify(7)` vs Polling (`std::filesystem::last_write_time`)

Filesystem monitoring operates via kernel notifications using Linux `inotify(7)`
rather than periodic polling loops comparing file modification timestamps.

1. **Why Polling Was Rejected**:
   - **High CPU and I/O Overhead**: Polling multiple files every 10–50 ms incurs
     continuous filesystem metadata queries and context switching, violating
     `SW_REQ_LOW_RESOURCE_USAGE`.
   - **Detection Latency vs CPU Trade-off**: Increasing poll interval increases
     event detection latency, while decreasing poll interval wastes CPU cycles.
   - **Sub-Second Precision Limitations**: Filesystem timestamps on certain
     filesystems (e.g. NFS, FAT) have 1-second granularity, leading to missed
     rapid modifications.

2. **Why Linux `inotify(7)` Was Selected**:
   - **Zero Overhead when Idle**: Kernel notifies process only when VFS write
     events occur; process sleeps in event loop with zero CPU consumption.
   - **Instantaneous Notification**: Sub-millisecond notification latency upon
     write completion.
   - **Native File Descriptor Integration**: Inotify file descriptor integrates
     directly with non-blocking event loops (`epoll`, `poll`), ensuring unified
     event ordering alongside socket I/O
     (`SW_REQ_EVENT_HANDLING_INCOMING_EVENT_ORDER`).

### 7.2 Monotonic Clock Debounce Mechanism vs Raw Event Dispatch

When monitoring file modifications (`filesystems` in `connix-config.json`), file
write operations frequently generate multiple sequential events (e.g. open,
multiple `write` chunks, truncate, `close_write`).

1. **Why Raw Event Dispatch Fails**:
   - Emitting an event on every single `IN_MODIFY` syscall would cause a single
     file save to trigger 5–10 duplicate network actions (`onModified`),
     spamming remote peers and violating intended application behavior.

2. **Why Monotonic Debouncing Was Selected**:
   - `FilesystemWatcher` sets a timer for `debounce` milliseconds (e.g. 100 ms).
   - Any secondary modification events arriving before deadline refresh the
     countdown.
   - Once the file has remained quiet for the full debounce duration, a single
     consolidated `FilesystemEvent` is emitted.
   - Monotonic time calculation prevents clock drift and timing anomalies.

### 7.3 Testability via Inotify and FileIo Abstraction Barriers

Deterministic unit and integration testing requires verifying all error paths
without relying on physical files or kernel watch descriptors:

1. **Kernel Decoupling (`IInotify`)**:
   - Encapsulating `inotify_init1`, `inotify_add_watch`, and `inotify_rm_watch`
     behind `IInotify` allows test suites to inject `MockIInotify`.
   - Tests simulate inotify descriptor exhaustion, permission errors, and
     synthetic inotify event streams deterministically without disk operations.

2. **Filesystem Decoupling (`IFileIo`)**:
   - Encapsulating filesystem queries and stream operations behind `IFileIo`
     enables tests to simulate missing files, read failures, disk-full
     conditions, and permission denials with 100% code coverage.

### 7.4 Unified Service & Centralized Event Pumping vs Direct Action Execution

Connix is an event-driven network interaction engine (`SW_REQ_EVENT_HANDLING`,
`SW_REQ_RULE_BASED`).

1. **Centralized Event Dispatching**:
   - Rather than having filesystem watchers execute action payloads directly,
     `FilesystemService` routes events through `FilesystemEventHandler`.
   - This integrates cleanly with the engine's event loop and rule engine,
     allowing filesystem events to be queued alongside transport and timer
     events in strict FIFO order (`SW_REQ_EVENT_HANDLING_INCOMING_EVENT_ORDER`).

### 7.5 Strict Fail-Fast Validation for Output Redirection

Requirements `SW_REQ_TRAFFIC_VISIBILITY_FILE_REDIRECTION` and test
specifications `TC_OUT_05`, `TC_OUT_06` mandate strict pre-flight validation of
output paths:

1. **Fail-Fast Parent Directory Verification**:
   - Before launching connection cycles, `FileIo` verifies that the target
     output file's parent directory exists.
   - If missing, `FilesystemException(PATH_NOT_FOUND)` is raised immediately,
     ensuring the engine aborts with a descriptive error before allocating
     sockets or initiating network cycles.
2. **Permission Verification**:
   - Proactively verifies write permissions to ensure logging will not fail
     silently during traffic execution.
