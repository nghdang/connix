#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"

#include <string>
#include <utility>

#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

TimerEvent::TimerEvent(std::string timerId, TimerType type,
                       TimerTimePoint timestamp)
    : m_timerId(std::move(timerId))
    , m_type(type)
    , m_timestamp(timestamp)
{
}

const std::string& TimerEvent::getTimerId() const noexcept
{
    return m_timerId;
}

TimerType TimerEvent::getType() const noexcept
{
    return m_type;
}

TimerTimePoint TimerEvent::getTimestamp() const noexcept
{
    return m_timestamp;
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
