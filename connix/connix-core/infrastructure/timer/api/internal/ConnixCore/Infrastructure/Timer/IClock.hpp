#pragma once

#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Operating-system clock abstraction interface for monotonic time
 * progression.
 */
class IClock
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~IClock() = default;

    /**
     * @brief Retrieves the current monotonic time point.
     * @return Current TimerTimePoint.
     */
    virtual TimerTimePoint now() const = 0;

    /**
     * @brief Suspends current execution for the specified duration.
     * @param duration Time duration to sleep.
     */
    virtual void sleepFor(TimerDuration duration) = 0;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
