#include "ConnixCore/Infrastructure/Timer/TimerFactory.hpp"

#include <memory>
#include <utility>

#include "ConnixCore/Infrastructure/Timer/Clock.hpp"
#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/Timer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

TimerFactory::TimerFactory(std::shared_ptr<IClock> clock)
    : m_clock(std::move(clock))
{
    if (!m_clock)
    {
        m_clock = std::make_shared<Clock>();
    }
}

std::shared_ptr<ITimer>
TimerFactory::createSingleShotTimer(TimerDuration interval,
                                    TimerCallback callback)
{
    return std::make_shared<Timer>(interval, TimerType::SINGLE_SHOT, m_clock,
                                   std::move(callback));
}

std::shared_ptr<ITimer>
TimerFactory::createPeriodicTimer(TimerDuration interval,
                                  TimerCallback callback)
{
    return std::make_shared<Timer>(interval, TimerType::PERIODIC, m_clock,
                                   std::move(callback));
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
