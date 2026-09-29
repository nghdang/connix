# Task 32: Add Runtime View of Timer Component

## Objective
Create the detailed design runtime view diagram (`runtime_view.puml`) for the `timer` component under `connix-core/infrastructure/timer/dd/`, and document comprehensive runtime sequence specifications in `detailed_design.md`.

## Scope
- `connix/connix-core/infrastructure/timer/dd/runtime_view.puml`
- `connix/connix-core/infrastructure/timer/dd/detailed_design.md`
- `.ai/tasks/32_add_timer_runtime_view.md`

## Important Decisions & Assumptions
- Modeled the four core dynamic execution scenarios for the Timer component:
  - **Scenario 1: Timer Creation and Registration**:
    - `Client` requests `TimerFactory` to instantiate concrete `Timer` (`SINGLE_SHOT` or `PERIODIC`), injecting the monotonic `IClock` abstraction.
    - `Client` registers the resulting `std::shared_ptr<ITimer>` with `TimerService`.
    - `TimerService` binds an internal timeout hook to the timer, assigns a unique `timerId`, stores it in the active registry, and returns `timerId` to the caller.
  - **Scenario 2: Timer Arming and Lifecycle Control**:
    - `Client` calls `timer->start()`, prompting `Timer` to query `Clock::now()` (POSIX `clock_gettime(CLOCK_MONOTONIC)`), compute `deadline = current_time + interval`, and transition state to `TimerState::RUNNING`.
    - `Client` inspects status via `timer->isRunning()` or `timer->getState()` without service wrapper overhead.
  - **Scenario 3: Autonomous Expiration and Event Pumping**:
    - As monotonic time advances and reaches `deadline`, `Timer` executes its timeout handling.
    - For single-shot timers: transitions state to `TimerState::EXPIRED`.
    - For periodic timers: advances deadline harmonically (`deadline += interval`) to eliminate cumulative drift, maintaining `TimerState::RUNNING`.
    - Calls `TimerService`'s bound hook, which constructs a `TimerEvent(timerId, type, timestamp)` and pushes it to the registered `TimerEventHandler`.
    - Pushes event into the engine's central FIFO event queue alongside socket transport events (`SW_REQ_EVENT_HANDLING_INCOMING_EVENT_ORDER`).
  - **Scenario 4: Timer Deregistration and Bulk Teardown**:
    - Targeted deregistration: `TimerService::unregisterTimer(timerId)` stops the timer and removes it from the registry.
    - Bulk teardown: `TimerService::stopAll()` halts all registered countdowns simultaneously, transitioning instances to `TimerState::STOPPED` with zero lingering callbacks.
- Document structure alignment:
  - Added Section 6: Runtime View with `!include runtime_view.puml` and Section 6.1 Runtime Sequence Specifications.
  - Renumbered Design Decisions to Section 7.

## Changes Made
- Created `connix/connix-core/infrastructure/timer/dd/runtime_view.puml` containing the sequence diagram for Scenarios 1 to 4.
- Updated `connix/connix-core/infrastructure/timer/dd/detailed_design.md`:
  - Added Section 6 (Runtime View) embedding `runtime_view.puml`.
  - Added detailed sequence specifications for Scenarios 1-4.
  - Renumbered Section 6 (Design Decisions) to Section 7.
- Created task summary file `.ai/tasks/32_add_timer_runtime_view.md`.

## Validation Performed
- Validated PlantUML syntax across all timer diagrams (`static_view.puml`, `data_structures.puml`, `interface_view.puml`, `use_case.puml`, `runtime_view.puml`) using `plantuml -syntaxcheck` (clean, 0 errors).
- Validated spelling with `cspell` on modified documentation and task records (`./scripts/check_spelling.sh`) (clean, 0 errors).
- Checked `git status` to verify clean tracking of all modified and newly created files.
