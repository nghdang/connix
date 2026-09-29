# Task 31: Add Use Cases of Timer Component

## Objective
Create the detailed design use cases and diagram (`use_case.puml`) for the `timer` component under `connix-core/infrastructure/timer/dd/`, and document comprehensive use case specifications in `detailed_design.md`.

## Scope
- `connix/connix-core/infrastructure/timer/dd/use_case.puml`
- `connix/connix-core/infrastructure/timer/dd/detailed_design.md`
- `.ai/tasks/31_add_timer_use_cases.md`

## Important Decisions & Assumptions
- Modeled the six primary use cases supported by the Timer component:
  - **`Create Timer (UC1)`**: Factory creation via `ITimerFactory::createSingleShotTimer()` and `ITimerFactory::createPeriodicTimer()`, configuring duration, type (`SINGLE_SHOT` or `PERIODIC`), injecting the monotonic `IClock` abstraction, and validating durations (`SW_REQ_EVENT_HANDLING_TIMER`, `SW_REQ_EVENT_HANDLING_PERIODIC_TIMER_SOURCE`, `SW_REQ_EXECUTION_MODES_ONETIME`, `SW_REQ_EXECUTION_MODES_PERIODIC`).
  - **`Register Timer (UC2)`**: Registration of active timers into `ITimerService::registerTimer(timer)`, which assigns unique `timerId`s, manages registry collection, and wires timer timeout to emit `TimerEvent`s (`SW_REQ_EXECUTION_MODES_BUILT_IN_SCHEDULING`).
  - **`Unregister Timer (UC3)`**: Deregistration and cancellation of timers via `ITimerService::unregisterTimer(timerId)`, stopping running instances and removing them from the service registry (`SW_REQ_EXECUTION_MODES_BUILT_IN_SCHEDULING`).
  - **`Control Timer Lifecycle (UC4)`**: Direct caller control of `ITimer` instances (`start`, `stop`, `reset`, `setCallback`) and runtime state queries (`isRunning`, `getType`, `getState`, `getInterval`) without intermediate service wrappers.
  - **`Dispatch Timer Event (UC5)`**: Autonomous expiration and event pumping: on deadline arrival, `TimerService` constructs a strongly typed `TimerEvent` and invokes `TimerEventHandler`, feeding into the engine's central FIFO event queue alongside socket events (`SW_REQ_EVENT_HANDLING_INCOMING_EVENT_ORDER`).
  - **`Stop All Timers (UC6)`**: Coordinated bulk teardown via `ITimerService::stopAll()` during application shutdown, execution mode changes, or connection cycle completion.
- Maintained exact structural alignment with `connix-core/infrastructure/transport/dd/detailed_design.md` and `connix-core/infrastructure/configuration/dd/detailed_design.md` (`Section 1: Introduction`, `Section 2: Use Cases`, `Section 3: Static View`, `Section 4: Interface View`, `Section 5: Data Structures`, `Section 6: Design Decisions`).

## Changes Made
- Created `connix/connix-core/infrastructure/timer/dd/use_case.puml` modeling the 6 use cases associated with the `Client` actor.
- Updated `connix/connix-core/infrastructure/timer/dd/detailed_design.md`:
  - Added Section 2 (Use Cases) embedding `use_case.puml` and specifying UC1 to UC6.
  - Renumbered subsequent sections to Section 3 (Static View), Section 4 (Interface View), Section 5 (Data Structures), and Section 6 (Design Decisions).
- Created task tracking log `.ai/tasks/31_add_timer_use_cases.md`.

## Validation Performed
- Validated PlantUML syntax across all timer diagrams (`static_view.puml`, `data_structures.puml`, `interface_view.puml`, `use_case.puml`) using `plantuml -syntaxcheck` (clean, 0 errors).
- Validated spelling with `cspell` on modified documentation and task records (`./scripts/check_spelling.sh`) (clean, 0 errors).
- Checked `git status` to verify clean tracking of all modified and newly created files.
