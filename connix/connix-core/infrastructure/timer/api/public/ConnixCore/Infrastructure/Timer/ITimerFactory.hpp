#pragma once

#include <memory>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Factory interface for instantiating concrete timer instances.
 */
class ITimerFactory
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~ITimerFactory() = default;

    /**
     * @brief Creates a single-shot timer.
     * @param interval Duration before the timer expires.
     * @param callback Optional expiration callback.
     * @return Shared pointer to the created ITimer instance.
     * @throws TimerException if interval is zero.
     */
    virtual std::shared_ptr<ITimer>
    createSingleShotTimer(TimerDuration interval,
                          TimerCallback callback = nullptr) = 0;

    /**
     * @brief Creates a periodic recurring timer.
     * @param interval Recurrence interval duration.
     * @param callback Optional expiration callback.
     * @return Shared pointer to the created ITimer instance.
     * @throws TimerException if interval is zero.
     */
    virtual std::shared_ptr<ITimer>
    createPeriodicTimer(TimerDuration interval,
                        TimerCallback callback = nullptr) = 0;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
