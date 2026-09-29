#pragma once

#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Default operating system clock using std::chrono::steady_clock.
 */
class Clock : public IClock
{
public:
    /**
     * @brief Default constructor.
     */
    Clock() = default;

    /**
     * @brief Default destructor.
     */
    ~Clock() override = default;

    /**
     * @brief Retrieves current monotonic time point from steady_clock.
     * @return Current steady_clock time point.
     */
    TimerTimePoint now() const override;

    /**
     * @brief Suspends current thread execution for the specified duration.
     * @param duration Time duration to sleep.
     */
    void sleepFor(TimerDuration duration) override;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
