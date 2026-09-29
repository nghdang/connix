#include "ConnixCore/Infrastructure/Timer/TimerService.hpp"

#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerErrorCode.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerException.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

TimerService::TimerService() = default;

TimerService::~TimerService()
{
    stopAllInternal();
}

std::string TimerService::registerTimer(std::shared_ptr<ITimer> timer)
{
    if (!timer)
    {
        throw TimerException(TimerErrorCode::INVALID_DURATION,
                             "Cannot register null timer");
    }

    std::string timerId;
    {
        const std::lock_guard<std::mutex> lock(m_mutex);
        timerId = "timer_" + std::to_string(m_nextTimerId++);
        m_timers.emplace(timerId, timer);
    }

    try
    {
        const TimerType type = timer->getType();
        timer->setCallback([this, timerId, type]() {
            TimerEventHandler handler;
            {
                const std::lock_guard<std::mutex> lock(m_mutex);
                handler = m_eventHandler;
            }
            if (handler)
            {
                const auto now = std::chrono::steady_clock::now();
                const TimerEvent event(timerId, type, now);
                handler(event);
            }
        });
    } catch (...)
    {
        const std::lock_guard<std::mutex> lock(m_mutex);
        m_timers.erase(timerId);
        throw;
    }

    return timerId;
}

void TimerService::unregisterTimer(const std::string& timerId)
{
    std::shared_ptr<ITimer> timerToStop;
    {
        const std::lock_guard<std::mutex> lock(m_mutex);
        const auto it = m_timers.find(timerId);
        if (it == m_timers.end())
        {
            throw TimerException(TimerErrorCode::TIMER_NOT_FOUND,
                                 "Timer not found: " + timerId);
        }
        timerToStop = it->second;
        m_timers.erase(it);
    }

    if (timerToStop)
    {
        timerToStop->stop();
    }
}

std::shared_ptr<ITimer>
TimerService::getTimer(const std::string& timerId) const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    const auto it = m_timers.find(timerId);
    if (it == m_timers.end())
    {
        throw TimerException(TimerErrorCode::TIMER_NOT_FOUND,
                             "Timer not found: " + timerId);
    }
    return it->second;
}

bool TimerService::hasTimer(const std::string& timerId) const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_timers.find(timerId) != m_timers.end();
}

void TimerService::stopAll()
{
    stopAllInternal();
}

void TimerService::stopAllInternal()
{
    std::vector<std::shared_ptr<ITimer>> timersToStop;
    {
        const std::lock_guard<std::mutex> lock(m_mutex);
        timersToStop.reserve(m_timers.size());
        for (const auto& pair : m_timers)
        {
            timersToStop.push_back(pair.second);
        }
    }

    for (const auto& timer : timersToStop)
    {
        if (timer)
        {
            timer->stop();
        }
    }
}

void TimerService::setEventHandler(TimerEventHandler handler)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_eventHandler = std::move(handler);
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
