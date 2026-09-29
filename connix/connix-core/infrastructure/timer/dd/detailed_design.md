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
   - Implements the timer service coordinator interface (`ITimerService`) as the central entry point and lifecycle manager for all scheduled timers within `connix-core`.
   - Manages the collection of active timers, tracking their running state and scheduled deadlines.
   - Coordinates with `TimerFactory` to instantiate concrete timer instances (`SingleShotTimer`, `PeriodicTimer`) based on configuration (`TimerConfig`).
   - Dispatches timer expiration events to registered subscribers and enforces built-in internal scheduling (`SW_REQ_EXECUTION_MODES_BUILT_IN_SCHEDULING`).
   - Evaluates cycle active states to enforce periodic no-overlap rules (`SW_REQ_CONNECTION_MANAGEMENT_PERIODIC_NO_OVERLAP`), skipping triggers while an active connection cycle is executing.

3. **`TimerFactory`**:
   - Implements the Factory pattern (`ITimerFactory`) to instantiate concrete timer implementations (`SingleShotTimer`, `PeriodicTimer`).
   - Decouples timer construction from callers and orchestrators, injecting the clock abstraction (`IClock`) into created timer instances.
   - Enables mock injection during testing, allowing test doubles to provide deterministic time without real-world delays.

4. **`SingleShotTimer`**:
   - Implements single-execution timer semantics (`SW_REQ_EVENT_HANDLING_TIMER`, `SW_REQ_EXECUTION_MODES_ONETIME`).
   - Implements `ITimer`.
   - Arms for a configured time interval and fires its registered expiration callback once upon expiry, subsequently transitioning to an inactive/expired state.
   - Uses `Clock` to determine elapsed duration and target expiration timestamps.

5. **`PeriodicTimer`**:
   - Implements recurring interval timer semantics (`SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`, `SW_REQ_EXECUTION_MODES_PERIODIC`).
   - Implements `ITimer`.
   - Fires its registered expiration callback repeatedly at configured interval boundaries, computing the next monotonic deadline to prevent cumulative timing drift.
   - Uses `Clock` to evaluate time progression.

6. **`Clock`**:
   - Encapsulates operating-system clock primitives and monotonic time queries (`clock_gettime(CLOCK_MONOTONIC)`, `nanosleep`, `std::chrono::steady_clock`).
   - Implements `IClock` to establish an abstraction barrier (Dependency Inversion Principle) between timer logic and OS system time APIs.
   - Enables deterministic unit testing and simulated time progression via Google Mock (`MockIClock` / `FakeClock`) without real-time sleep (satisfying `TC_EVT_04` and `TC_EVT_05`).

7. **`Operating System (POSIX Clock / Timer APIs)`**:
   - The underlying Linux platform timing facilities and standard POSIX clock interfaces (`<chrono>`, `<ctime>`, `clock_gettime`, `nanosleep`).

### 2.2 Design Principles & Architectural Alignment

- **Clean Architecture:** `Timer` belongs to the Infrastructure / Frameworks & Drivers layer (`connix-core/infrastructure/timer/`). It realizes inward-facing interfaces (`ITimerService`, `ITimerFactory`, `ITimer`), isolating low-level OS time retrieval from Domain Entities and Application Use Cases.
- **Single Responsibility Principle (SRP):** `TimerService` coordinates timer collections and event dispatching; `TimerFactory` isolates object instantiation and dependency wiring; `SingleShotTimer` and `PeriodicTimer` specialize in one-off vs recurring interval deadlines; `Clock` isolates operating-system time queries.
- **Open-Closed Principle (OCP):** Additional timer types (such as cron expressions or exponential backoff timers) can be added by implementing `ITimer` and extending `TimerFactory` without altering existing timers or execution scheduling logic.
- **Dependency Inversion Principle (DIP):** Callers depend exclusively on abstract interfaces (`ITimerService`, `ITimerFactory`, `ITimer`). Timers depend on the abstract `IClock` interface, enabling deterministic mock testing with simulated time advancement (`MockIClock`).
- **Zero `std::optional` Mandate:** Conforms strictly to the project convention avoiding `std::optional` through explicit states (`TimerState::STOPPED`, `RUNNING`, `EXPIRED`), concrete types, and well-defined default parameter values.
