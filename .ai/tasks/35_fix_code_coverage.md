# Task 35: Fix Code Coverage to Reach 100%

## Objective
Achieve 100% line code coverage across the `connix-core` library by eliminating
the 6 uncovered lines in `Timer.cpp` and `TimerService.cpp`, adding transactional
rollback to `TimerService::registerTimer`, and adding unit tests covering
restarting expired timers, exception rollback handling, and polymorphic
interface deletions.

## Scope
- `connix/connix-core/infrastructure/timer/src/TimerService.cpp`
- `connix/connix-core/infrastructure/timer/tst/ut/TimerUnitTest.cpp`
- `connix/connix-core/infrastructure/timer/tst/ut/TimerServiceUnitTest.cpp`
- `connix/connix-core/infrastructure/timer/tst/ut/ClockUnitTest.cpp`
- `connix/connix-core/infrastructure/timer/tst/ut/TimerFactoryUnitTest.cpp`
- `connix/connix-core/infrastructure/timer/tst/ut/TimerDataStructuresUnitTest.cpp`
- `.ai/tasks/35_fix_code_coverage.md`

## Important Decisions & Assumptions
- **Timer Worker Thread Re-Joining**: In `Timer::start()`, the check
  `if (m_worker.joinable())` handles single-shot timers that have reached
  `TimerState::EXPIRED` where the worker thread completed its loop but was not
  yet joined. Added unit test `RestartExpiredTimerRejoinsAndStarts` to
  explicitly test re-joining and restarting the timer.
- **TimerService Transactional Rollback**: In `TimerService::registerTimer()`,
  if configuring `timer->setCallback(...)` throws an exception, `TimerService`
  erases the newly generated `timerId` from `m_timers` within `m_mutex` and
  rethrows, ensuring strong exception safety without orphaned registry
  entries.
- **Polymorphic Destructor Verification**: Added unit tests for deleting
  concrete instances through base interface pointers (`ITimer`,
  `ITimerService`, `IClock`, `ITimerFactory`, and `std::runtime_error` for
  `TimerException`) to exercise the virtual deleting destructor (`D0Ev`).

## Changes Made
- Modified `TimerService.cpp` to introduce a `try ... catch (...)` rollback
  block in `registerTimer()`.
- Updated `TimerUnitTest.cpp` with `RestartExpiredTimerRejoinsAndStarts` and
  `PolymorphicDeletionViaInterfacePointer`.
- Updated `TimerServiceUnitTest.cpp` with
  `RegisterTimerRollsBackAndPropagatesOnCallbackFailure` and
  `PolymorphicDeletionViaInterfacePointer`.
- Updated `ClockUnitTest.cpp` with `PolymorphicDeletionViaInterfacePointer`.
- Updated `TimerFactoryUnitTest.cpp` with
  `PolymorphicDeletionViaInterfacePointer`.
- Updated `TimerDataStructuresUnitTest.cpp` with
  `TimerExceptionPolymorphicDeletion`.
- Created `.ai/tasks/35_fix_code_coverage.md`.

## Validation Performed
- Ran unit tests and generated code coverage (`./scripts/run_unit_tests.sh`).
- Verified 100% line coverage threshold (`./scripts/check_coverage.sh`).
- Ran code formatter (`./scripts/run_clang_format.sh`).
- Ran static analysis (`./scripts/run_clang_tidy.sh`).
- Ran spelling check (`./scripts/check_spelling.sh`).
- Built release application (`./scripts/build_application_release.sh`).
