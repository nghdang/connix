# Task 30: Add Data Structures and Interfaces View of Timer Component

## Objective
Create the detailed design documentation, data structures diagram (`data_structures.puml`), and interface view diagram (`interface_view.puml`) for the `timer` component under `connix-core/infrastructure/timer/dd/`.

## Scope
- `connix/connix-core/infrastructure/timer/dd/static_view.puml`
- `connix/connix-core/infrastructure/timer/dd/data_structures.puml`
- `connix/connix-core/infrastructure/timer/dd/interface_view.puml`
- `connix/connix-core/infrastructure/timer/dd/detailed_design.md`
- `.ai/tasks/30_add_timer_data_structures_and_interfaces_view.md`

## Important Decisions & Assumptions
- **Interface Structure (`interface_view.puml`)**:
  - Organized under namespace `ConnixCore::Infrastructure::Timer` into `Public` and `Internal` packages, matching Clean Architecture conventions.
  - Defined public `ITimer` interface representing individual timers with lifecycle operations (`start`, `stop`, `reset`, `setCallback`) and query methods (`isRunning`, `getType`, `getState`, `getInterval`).
  - Removed unneeded interfaces (`trigger`, `isExpired`, `getRemainingTime`, `getDeadline`): timers dispatch registered callbacks automatically upon timeout, avoiding external trigger calls and clock type leakage.
  - Consolidated `SingleShotTimer` and `PeriodicTimer` into a unified `Timer` class implementing `ITimer` and depending on `IClock`. Internal repeat count / `TimerType` governs one-time vs periodic repetition.
  - Defined public `ITimerFactory` and `TimerFactory` with `createSingleShotTimer` and `createPeriodicTimer` returning `std::shared_ptr<ITimer>` and injecting `IClock` (omitted generic `createTimer` since only two concrete types exist).
  - Streamlined `ITimerService` and `TimerService`: eliminated redundant pass-through methods and out-of-scope cycle overlap / polling methods (`notifyCycleStarted`, `notifyCycleCompleted`, `isCycleActive`, `processTimers`, `getNextTimeout`).
  - Implemented centralized event pumping via `setEventHandler(TimerEventHandler handler)` on `ITimerService`. When timers expire, `TimerService` constructs and emits a strongly typed `TimerEvent` to central application schedulers/listeners.
  - `ITimerService` operations:
    - `registerTimer(timer: shared_ptr<ITimer>): string`: assigns and returns the unique timer ID, binding expiration to emit `TimerEvent`.
    - `unregisterTimer(timerId: string): void`: unregisters and stops the named timer.
    - `getTimer(timerId: string): shared_ptr<ITimer> const`: retrieves the timer instance.
    - `hasTimer(timerId: string): bool const`: checks if a timer exists.
    - `stopAll(): void`: halts all registered timers.
    - `setEventHandler(handler: TimerEventHandler): void`: registers subscriber for timer expiration events.
  - Defined internal `IClock` and `Clock` encapsulating POSIX/steady_clock primitives, decoupling timer logic from OS time APIs and enabling mock injection (`MockIClock` / `FakeClock`) for zero-delay deterministic testing (`TC_EVT_04`, `TC_EVT_05`).
- **Data Structures (`data_structures.puml`)**:
  - Defined public enums: `TimerType` (`SINGLE_SHOT`, `PERIODIC`), `TimerState` (`STOPPED`, `RUNNING`, `EXPIRED`), and `TimerErrorCode` (`TIMER_NOT_FOUND`, `INVALID_DURATION`, `TIMER_ALREADY_RUNNING`, `TIMER_NOT_RUNNING`, `SYSTEM_CLOCK_ERROR`).
  - Defined public `TimerException` carrying a strongly typed `TimerErrorCode`.
  - Defined public `TimerEvent` class encapsulating `timerId`, `type`, and `timestamp`.
  - Defined type aliases: `TimerCallback`, `TimerEventHandler`, `TimerDuration`, `TimerTimePoint`.
  - **Zero `std::optional` Mandate**: Conforms strictly to workspace conventions by using concrete types and default states, avoiding `std::optional`.
- **Design Decisions Documented**:
  - Autonomous timer expiration vs POSIX asynchronous signal timers (`timer_create` / `SIGALRM`).
  - Cumulative drift prevention in periodic timers via monotonic interval accumulation (`previous_deadline + interval`).
  - Testability through clock decoupling (`IClock` injection).
  - Centralized event pumping vs direct callback execution: aligns with `periodic_rule_execution.puml`, guaranteeing FIFO event ordering with transport socket events and clean architecture boundaries.

## Changes Made
- Updated `connix/connix-core/infrastructure/timer/dd/static_view.puml` reflecting unified `Timer` component and aliasing `rectangle "Timer" as timer_module` to prevent PlantUML name conflicts.
- Created `connix/connix-core/infrastructure/timer/dd/interface_view.puml` with PlantUML diagram of streamlined public and internal interfaces, concrete classes, event pumping, and dependencies.
- Created `connix/connix-core/infrastructure/timer/dd/data_structures.puml` with PlantUML diagram of public timer data structures, `TimerEvent`, enums, exceptions, and relationships.
- Updated `connix/connix-core/infrastructure/timer/dd/detailed_design.md` adding Section 3 (Interface View), Section 4 (Data Structures), and Section 5 (Design Decisions) with detailed specifications.
- Documented task progress in `.ai/tasks/30_add_timer_data_structures_and_interfaces_view.md`.

## Validation Performed
- Validated PlantUML syntax of `interface_view.puml`, `data_structures.puml`, and `static_view.puml` using `plantuml -syntaxcheck` (clean, 0 errors).
- Ran spelling check (`./scripts/check_spelling.sh`) to verify zero spelling issues across all project files.
- Checked git status to verify only expected files were created or modified.
