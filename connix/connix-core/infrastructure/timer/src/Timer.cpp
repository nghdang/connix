#include "ConnixCore/Infrastructure/Timer/Timer.hpp"

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

#include "ConnixCore/Infrastructure/Timer/Clock.hpp"
#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerErrorCode.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerException.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

Timer::Timer(TimerDuration interval, TimerType type,
             std::shared_ptr<IClock> clock, TimerCallback callback)
    : m_interval(interval)
    , m_type(type)
    , m_clock(std::move(clock))
    , m_callback(std::move(callback))
{
    if (m_interval.count() == 0)
    {
        throw TimerException(TimerErrorCode::INVALID_DURATION,
                             "Timer interval must be greater than zero");
    }
    if (!m_clock)
    {
        m_clock = std::make_shared<Clock>();
    }
}

Timer::~Timer()
{
    stopInternal();
}

void Timer::start()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_state == TimerState::RUNNING)
    {
        throw TimerException(TimerErrorCode::TIMER_ALREADY_RUNNING,
                             "Timer is already running");
    }

    if (m_worker.joinable())
    {
        m_stopRequested = true;
        m_cv.notify_all();
        lock.unlock();
        m_worker.join();
        lock.lock();
    }

    m_stopRequested = false;
    m_resetRequested = false;
    m_state = TimerState::RUNNING;
    m_deadline = m_clock->now() + m_interval;

    m_worker = std::thread(&Timer::workerLoop, this);
}

void Timer::stop()
{
    stopInternal();
}

void Timer::stopInternal()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_state == TimerState::STOPPED)
    {
        return;
    }

    m_state = TimerState::STOPPED;
    m_stopRequested = true;
    m_cv.notify_all();

    if (m_worker.joinable())
    {
        lock.unlock();
        m_worker.join();
    }
}

void Timer::reset()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_state == TimerState::RUNNING)
    {
        m_resetRequested = true;
        m_cv.notify_all();
    }
    else
    {
        lock.unlock();
        start();
    }
}

bool Timer::isRunning() const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_state == TimerState::RUNNING;
}

TimerType Timer::getType() const
{
    return m_type;
}

TimerState Timer::getState() const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_state;
}

TimerDuration Timer::getInterval() const
{
    return m_interval;
}

void Timer::setCallback(TimerCallback callback)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_callback = std::move(callback);
}

void Timer::workerLoop()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while (!m_stopRequested && m_state == TimerState::RUNNING)
    {
        const auto now = m_clock->now();
        if (now >= m_deadline)
        {
            if (m_type == TimerType::SINGLE_SHOT)
            {
                m_state = TimerState::EXPIRED;
            }
            else
            {
                m_deadline += m_interval;
            }

            const auto callback = m_callback;
            lock.unlock();
            if (callback)
            {
                callback();
            }
            lock.lock();

            if (m_type == TimerType::SINGLE_SHOT)
            {
                break;
            }
        }
        else
        {
            const auto remaining =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    m_deadline - now);
            m_cv.wait_for(lock, remaining, [this]() {
                return m_stopRequested || m_resetRequested;
            });

            if (m_resetRequested)
            {
                m_resetRequested = false;
                m_deadline = m_clock->now() + m_interval;
            }
        }
    }
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
