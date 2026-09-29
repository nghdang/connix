#pragma once

#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Interface representing an individual countdown timer.
 */
class ITimer
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~ITimer() = default;

    /**
     * @brief Arms and starts the timer countdown from current clock time.
     * @throws TimerException if the timer is already running.
     */
    virtual void start() = 0;

    /**
     * @brief Halts the countdown and transitions state to STOPPED.
     */
    virtual void stop() = 0;

    /**
     * @brief Recomputes the deadline from current clock time and arms the
     * timer.
     */
    virtual void reset() = 0;

    /**
     * @brief Checks if the timer is currently actively counting down.
     * @return True if in RUNNING state.
     */
    virtual bool isRunning() const = 0;

    /**
     * @brief Retrieves the recurrence classification of the timer.
     * @return TimerType enum value.
     */
    virtual TimerType getType() const = 0;

    /**
     * @brief Retrieves the current lifecycle state of the timer.
     * @return TimerState enum value.
     */
    virtual TimerState getState() const = 0;

    /**
     * @brief Retrieves the configured duration interval of the timer.
     * @return Configured interval in milliseconds.
     */
    virtual TimerDuration getInterval() const = 0;

    /**
     * @brief Registers or updates the callback invoked upon timeout
     * expiration.
     * @param callback Function to invoke when the timer expires.
     */
    virtual void setCallback(TimerCallback callback) = 0;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
