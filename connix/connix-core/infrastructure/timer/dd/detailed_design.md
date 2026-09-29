# Detailed Design: Component Timer

This document specifies the detailed design for the Timer module inside
`connix-core/infrastructure/timer/`.

## 1. Introduction

The timer module is a foundational infrastructure component responsible for
providing deterministic, event-driven timing mechanisms across single-shot delays
and periodic recurring intervals. It encapsulates operating-system time and clock
primitives, enforces timing accuracy and periodic no-overlap semantics, and
adheres to **SOLID principles**, **Clean Code** standards, and **Clean Architecture**.

The component satisfies:
- **`SW_REQ_EVENT_HANDLING`**: Trigger configured actions from message-receipt, connect, accept, close, and timer events.
- **`SW_REQ_EVENT_HANDLING_EVENT_KINDS`**: Recognize timer events when the corresponding interval occurs.
- **`SW_REQ_EVENT_HANDLING_TIMER`**: Recognize a timer event when a configured elapsed-time interval expires.
- **`SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`**: Use the event-driven timer mechanism for intervals configured by PERIODIC mode.
- **`SW_REQ_EXECUTION_MODES_PERIODIC`**: Perform configured actions at each configured interval when operating in PERIODIC mode.
- **`SW_REQ_EXECUTION_MODES_BUILT_IN_SCHEDULING`**: Schedule ONETIME and PERIODIC actions internally without requiring an external scheduler.
- **`SW_REQ_CONNECTION_MANAGEMENT_PER_TRIGGER_CONNECTION_EXECUTION`**: Start one complete connection cycle for each eligible PERIODIC, event, or rule trigger, and skip a PERIODIC trigger while another PERIODIC cycle is active.
- **`SW_REQ_CONNECTION_MANAGEMENT_PERIODIC_NO_OVERLAP`**: Skip a PERIODIC trigger when another PERIODIC cycle is still active.
- **`SW_REQ_CONNECTION_MANAGEMENT_PERIODIC_TIMEOUT_OUTCOME`**: Continue future eligible PERIODIC triggers after a timed-out cycle has closed.

---

## 2. Static View

The static relationship of modules and interfaces within the Timer
component is detailed in

```plantuml
!include static_view.puml
```

### 2.1 Module Responsibilities

1. **`Client`**:
   - External consumer of the timer component (such as the execution scheduler, connection-cycle use case, rule engine, or application orchestration layer in Clean Architecture).
   - Manages and registers scheduled triggers via `ITimerService` or instantiates standalone timer objects via `ITimerFactory`.
   - Binds timeout actions and callbacks to timer events (`SW_REQ_EVENT_HANDLING_TIMER`, `SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`).

2. **`TimerService`**:
   - Implements the timer service coordinator interface (`ITimerService`) as the central registry and event pump for all active timers within `connix-core`.
   - Manages the collection of registered `ITimer` instances, assigning and managing unique timer identifiers.
   - Pushes strongly typed `TimerEvent` objects to registered listeners (`ExecutionScheduler`, rule engine) upon timer timeout.
   - Provides lifecycle management operations such as `stopAll()` during cycle teardown or application shutdown.

3. **`TimerFactory`**:
   - Implements the Factory pattern (`ITimerFactory`) to instantiate concrete timer instances (`Timer`).
   - Decouples timer construction from callers and orchestrators, injecting the clock abstraction (`IClock`) into created timer instances.
   - Enables mock injection during testing, allowing test doubles to provide deterministic time without real-world delays.

4. **`Timer`**:
   - Implements `ITimer` providing deterministic interval timing for both single-shot delays (`SW_REQ_EVENT_HANDLING_TIMER`, `SW_REQ_EXECUTION_MODES_ONETIME`) and recurring intervals (`SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`, `SW_REQ_EXECUTION_MODES_PERIODIC`).
   - Armed for a configured time interval. When elapsed, it dispatches its registered callback automatically.
   - For single-shot timers, execution count is 1, after which it transitions to an inactive/expired state.
   - For periodic timers, it maintains continuous repetition, computing the next monotonic deadline (`deadline += interval`) to eliminate cumulative timing drift.
   - Uses `Clock` to evaluate time progression.

5. **`Clock`**:
   - Encapsulates operating-system clock primitives and monotonic time queries (`clock_gettime(CLOCK_MONOTONIC)`, `nanosleep`, `std::chrono::steady_clock`).
   - Implements `IClock` to establish an abstraction barrier (Dependency Inversion Principle) between timer logic and OS system time APIs.
   - Enables deterministic unit testing and simulated time progression via Google Mock (`MockIClock` / `FakeClock`) without real-time sleep (satisfying `TC_EVT_04` and `TC_EVT_05`).

6. **`Operating System (POSIX Clock / Timer APIs)`**:
   - The underlying Linux platform timing facilities and standard POSIX clock interfaces (`<chrono>`, `<ctime>`, `clock_gettime`, `nanosleep`).

### 2.2 Design Principles & Architectural Alignment

- **Clean Architecture:** `Timer` belongs to the Infrastructure / Frameworks & Drivers layer (`connix-core/infrastructure/timer/`). It realizes inward-facing interfaces (`ITimerService`, `ITimerFactory`, `ITimer`), isolating low-level OS time retrieval from Domain Entities and Application Use Cases.
- **Single Responsibility Principle (SRP):** `TimerService` manages timer registration, lookup, and lifecycle teardown; `TimerFactory` isolates object instantiation and dependency wiring; `Timer` encapsulates interval deadline tracking and automatic callback invocation; `Clock` isolates operating-system time queries.
- **Open-Closed Principle (OCP):** Additional timer types (such as cron expressions or exponential backoff timers) can be added by implementing `ITimer` and extending `TimerFactory` without altering existing timers or registry logic.
- **Dependency Inversion Principle (DIP):** Callers depend exclusively on abstract interfaces (`ITimerService`, `ITimerFactory`, `ITimer`). Timers depend on the abstract `IClock` interface, enabling deterministic mock testing with simulated time advancement (`MockIClock`).
- **Zero `std::optional` Mandate:** Conforms strictly to the project convention avoiding `std::optional` through explicit states (`TimerState::STOPPED`, `RUNNING`, `EXPIRED`), concrete types, and well-defined default parameter values.

---

## 3. Interface View

Details of core object behaviors, public/internal interfaces, concrete implementations,
and dependency relationships are modeled in

```plantuml
!include interface_view.puml
```

### 3.1 Interface Specifications

1. **`ITimer` (Public Interface)**:
   - Represents an individual timer entity supporting arming, disarming, reset, and automatic expiration notification.
   - **Lifecycle Management**:
     - `start()`: Transitions the timer to `TimerState::RUNNING` and computes its deadline from the current time. If already running, throws `TimerException` with `TimerErrorCode::TIMER_ALREADY_RUNNING`.
     - `stop()`: Halts the timer and transitions state to `TimerState::STOPPED`. Idempotent if already stopped.
     - `reset()`: Re-computes the target deadline starting from the current clock time and sets state to `TimerState::RUNNING`.
     - `setCallback(TimerCallback callback)`: Updates or registers the expiration callback invoked automatically upon timeout.
   - **State Inspection**:
     - `isRunning()`: Returns `true` if the timer is in `TimerState::RUNNING`.
     - `getType()`: Returns `TimerType::SINGLE_SHOT` or `TimerType::PERIODIC`.
     - `getState()`: Returns current `TimerState` (`STOPPED`, `RUNNING`, `EXPIRED`).
     - `getInterval()`: Returns configured duration in milliseconds.

2. **`ITimerFactory` & `TimerFactory` (Public Interface & Implementation)**:
   - Provides factory construction for concrete timer instances, hiding allocation and dependency wiring from consumers.
   - `createSingleShotTimer(milliseconds interval, TimerCallback callback)`: Creates and returns a `std::shared_ptr<ITimer>` wrapping a `Timer` configured for single-shot execution.
   - `createPeriodicTimer(milliseconds interval, TimerCallback callback)`: Creates and returns a `std::shared_ptr<ITimer>` wrapping a `Timer` configured for periodic recurring execution.
   - Holds a `std::shared_ptr<IClock>` which is injected into all created timer instances, facilitating deterministic testing and time progression simulation.

3. **`ITimerService` & `TimerService` (Public Interface & Implementation)**:
   - Central registry and event source for active timer instances within `connix-core`.
   - **Registry & Lifecycle Management**:
     - `registerTimer(shared_ptr<ITimer> timer)`: Registers an existing timer instance into the service registry, assigns and returns a unique `timerId` (string). Internally binds the timer's expiration to emit a `TimerEvent`.
     - `unregisterTimer(string timerId)`: Stops and removes the named timer from the service collection. Throws `TimerException` with `TimerErrorCode::TIMER_NOT_FOUND` if `timerId` does not exist.
     - `getTimer(string timerId)`: Retrieves the registered `shared_ptr<ITimer>`. Throws `TimerException` with `TimerErrorCode::TIMER_NOT_FOUND` if `timerId` does not exist.
     - `hasTimer(string timerId)`: Returns `true` if a timer with the given ID exists.
     - `stopAll()`: Halts all registered timers simultaneously during cycle teardown or application shutdown.
     - `setEventHandler(TimerEventHandler handler)`: Registers the subscriber callback to receive `TimerEvent` instances emitted by expiring timers.

4. **`IClock` & `Clock` (Internal Interface & Implementation)**:
   - Establishes an OS-agnostic clock boundary (Dependency Inversion Principle).
   - `now()`: Returns current monotonic `time_point`. Implemented via `std::chrono::steady_clock::now()` or POSIX `clock_gettime(CLOCK_MONOTONIC)`.
   - `sleepFor(milliseconds duration)`: Pauses execution for the specified duration using `std::this_thread::sleep_for` or `nanosleep`.
   - Enables mock injection (`MockIClock` / `FakeClock`) in unit and integration tests (`TC_EVT_04`, `TC_EVT_05`) to advance virtual time deterministically with zero real-time delay.

5. **`Timer` (Internal Class)**:
   - Realizes `ITimer` for both single-shot delay expiration (`SW_REQ_EVENT_HANDLING_TIMER`, `SW_REQ_EXECUTION_MODES_ONETIME`) and recurring interval triggers (`SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`, `SW_REQ_EXECUTION_MODES_PERIODIC`).
   - For single-shot mode: runs once, automatically invokes the registered callback upon expiration, and transitions to `TimerState::EXPIRED`.
   - For periodic mode: automatically invokes the callback at each interval boundary and computes `deadline += interval` upon each expiration, eliminating cumulative drift and maintaining exact harmonic intervals.

---

## 4. Data Structures

The data structures, enums, exception types, and their relationships are modeled in

```plantuml
!include data_structures.puml
```

### 4.1 Data Structure Specifications

1. **`TimerType` (Public Enum)**:
   - Strongly typed enumeration classifying the operational recurrence of a timer:
     - `SINGLE_SHOT`: Fires once upon elapsed interval and transitions to `EXPIRED`.
     - `PERIODIC`: Fires repeatedly at each configured interval boundary.

2. **`TimerState` (Public Enum)**:
   - Represents the current execution lifecycle state of a timer:
     - `STOPPED`: Timer is disarmed and inactive.
     - `RUNNING`: Timer is armed and counting down toward its deadline.
     - `EXPIRED`: Single-shot timer has completed its delay and fired its callback.

3. **`TimerErrorCode` & `TimerException` (Public Types)**:
   - `TimerErrorCode`: Strongly typed enumeration of granular timer error states:
     - `TIMER_NOT_FOUND`: Specified timer identifier does not exist in the registry.
     - `INVALID_DURATION`: Configured interval duration is zero or invalid.
     - `TIMER_ALREADY_RUNNING`: Attempted to start an already running timer.
     - `TIMER_NOT_RUNNING`: Attempted to stop or reset an inactive timer.
     - `SYSTEM_CLOCK_ERROR`: Underlying OS clock query failed.
   - `TimerException`: Standard C++ exception (`std::runtime_error`) holding a concrete `TimerErrorCode` and diagnostic message.

4. **`TimerEvent` (Public Class)**:
   - Represents a strongly typed timer expiration event emitted by `TimerService` to the engine:
     - `m_timerId`: `std::string` identifying the timer source.
     - `m_type`: `TimerType` (`SINGLE_SHOT` or `PERIODIC`).
     - `m_timestamp`: `std::chrono::steady_clock::time_point` when the timeout occurred.
   - Methods: `getTimerId()`, `getType()`, `getTimestamp()`.

5. **Type Aliases**:
   - `TimerCallback`: `std::function<void()>` — signature of low-level timer expiration handlers.
   - `TimerEventHandler`: `std::function<void(const TimerEvent&)>` — signature of central timer event subscribers.
   - `TimerDuration`: `std::chrono::milliseconds` — standard time resolution for configuration and interval specifications.
   - `TimerTimePoint`: `std::chrono::steady_clock::time_point` — standard monotonic timestamp representation.

---

## 5. Design Decisions

### 5.1 Autonomous Timer Expiration vs POSIX Asynchronous Signal Timers (`timer_create` / `SIGALRM`)

Timers operate with internal scheduling and automatic callback invocation utilizing monotonic clock calculations rather than OS signal-based timers (`timer_create(2)`, `setitimer(2)`, `SIGALRM`).

1. **Why POSIX Signals Were Rejected**:
   - **Signal Safety Hazards**: POSIX signal handlers may only call async-signal-safe functions. Dispatching high-level connection-cycle use cases, memory allocations, or logging from a signal handler triggers undefined behavior or deadlocks.
   - **Thread Targeting Ambiguity**: In multi-threaded applications, signals generated by `SIGALRM` or `timer_create` are delivered to an arbitrary eligible thread unless complex signal masking and dedicated `sigwaitinfo` threads are maintained.
   - **State Corruption & Reentrancy**: Handling timer expirations inside signal interruptions creates race conditions and requires atomic locks across all engine components.

2. **Why Monotonic Deadline Calculation Was Selected**:
   - Timers evaluate deadlines using `IClock::now()`.
   - Expiration callbacks execute synchronously or via managed dispatch without signal handler restrictions.

### 5.2 Drift Prevention in Periodic Timers

When managing recurring triggers (`SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`, `SW_REQ_EXECUTION_MODES_PERIODIC`),
`Timer` (in periodic mode) computes the next deadline via:

$$\text{next\_deadline} = \text{previous\_deadline} + \text{interval}$$

rather than:

$$\text{next\_deadline} = \text{now}() + \text{interval}$$

1. **Elimination of Cumulative Drift**:
   - If a periodic timer with a 1000 ms interval takes 5 ms to dispatch its callback, re-arming with
     `now() + interval` causes the timer to fire every 1005 ms. Over 1000 iterations, the timer would drift
     by 5 seconds.
   - In contrast, accumulating monotonic intervals (`previous_deadline + interval`) preserves exact harmonic
     cadence regardless of execution latency or thread scheduling jitter.

### 5.3 Testability through Clock Decoupling (`IClock` Injection)

Requirements `TC_EVT_04` and `TC_EVT_05` specify that timer rules and PERIODIC mode intervals must be verified
deterministically without real-time sleep:

1. **Mock Clock Injection**:
   - `TimerFactory` accepts `std::shared_ptr<IClock>`. In production, this defaults to `Clock`
     backed by `std::chrono::steady_clock`.
   - In test suites, tests inject `MockIClock` or `FakeClock`. Test cases advance the virtual clock timestamp
     by arbitrary intervals (e.g., advancing 2 seconds three times in microseconds), verifying rule triggers and
     action dispatches with zero CPU delay or flaky timing dependencies.

### 5.4 Centralized Event Pumping vs Direct Callback Execution

Connix is an event-driven network interaction engine (`SW_REQ_EVENT_HANDLING_TIMER`,
`SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`, `SW_REQ_EVENT_HANDLING_INCOMING_EVENT_ORDER`).

1. **Centralized Event Dispatching**:
   - Rather than having individual timers invoke arbitrary domain actions or rule evaluations directly,
     `TimerService` collects timer timeouts and pumps unified `TimerEvent` objects via `TimerEventHandler`.
   - This aligns with the system runtime architecture (`periodic_rule_execution.puml`), where the timer
     runtime feeds timer events to the `ExecutionScheduler` and `EventRuleUseCase`.
2. **Preservation of Event Ordering**:
   - Dispatched `TimerEvent` instances can be queued alongside incoming socket transport events in the
     application layer's FIFO event pipeline, guaranteeing predictable evaluation order without thread
     concurrency hazards.
