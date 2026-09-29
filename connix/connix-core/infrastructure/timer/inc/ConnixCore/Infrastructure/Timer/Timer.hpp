#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Concrete timer implementation supporting single-shot and periodic
 * intervals.
 */
class Timer : public ITimer
{
public:
    /**
     * @brief Constructs a new Timer instance.
     * @param interval Duration before expiration.
     * @param type Recurrence behavior (SINGLE_SHOT or PERIODIC).
     * @param clock Clock abstraction for time measurement.
     * @param callback Optional expiration callback.
     * @throws TimerException if interval is zero.
     */
    Timer(TimerDuration interval, TimerType type,
          std::shared_ptr<IClock> clock = nullptr,
          TimerCallback callback = nullptr);

    /**
     * @brief Destructor. Halts and joins background thread cleanly.
     */
    ~Timer() override;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer&&) = delete;

    void start() override;
    void stop() override;
    void reset() override;
    bool isRunning() const override;
    TimerType getType() const override;
    TimerState getState() const override;
    TimerDuration getInterval() const override;
    void setCallback(TimerCallback callback) override;

private:
    void workerLoop();
    void stopInternal();

    TimerDuration m_interval;
    TimerType m_type;
    std::shared_ptr<IClock> m_clock;
    TimerCallback m_callback;
    TimerTimePoint m_deadline{};
    TimerState m_state{ TimerState::STOPPED };

    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_worker;
    bool m_stopRequested{ false };
    bool m_resetRequested{ false };
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
