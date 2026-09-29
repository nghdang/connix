#pragma once

#include <functional>
#include <string>

#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Strongly typed event emitted when a registered timer expires.
 */
class TimerEvent
{
public:
    /**
     * @brief Constructs a new TimerEvent.
     * @param timerId Unique identifier of the expiring timer.
     * @param type Recurrence classification of the timer.
     * @param timestamp Monotonic time point when expiration occurred.
     */
    TimerEvent(std::string timerId, TimerType type, TimerTimePoint timestamp);

    /**
     * @brief Retrieves the identifier of the timer that emitted this event.
     * @return Const reference to the timer ID string.
     */
    const std::string& getTimerId() const noexcept;

    /**
     * @brief Retrieves the recurrence type of the timer.
     * @return TimerType enum value.
     */
    TimerType getType() const noexcept;

    /**
     * @brief Retrieves the timestamp when the timer expired.
     * @return Monotonic time point of expiration.
     */
    TimerTimePoint getTimestamp() const noexcept;

private:
    std::string m_timerId;
    TimerType m_type;
    TimerTimePoint m_timestamp;
};

/**
 * @brief Subscriber handler signature for receiving timer expiration events.
 */
using TimerEventHandler = std::function<void(const TimerEvent& event)>;

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
