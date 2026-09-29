#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimerService.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Central timer registry and event pumping service.
 */
class TimerService : public ITimerService
{
public:
    /**
     * @brief Constructs a new TimerService.
     */
    TimerService();

    /**
     * @brief Destructor. Halts all registered timers.
     */
    ~TimerService() override;

    TimerService(const TimerService&) = delete;
    TimerService& operator=(const TimerService&) = delete;
    TimerService(TimerService&&) = delete;
    TimerService& operator=(TimerService&&) = delete;

    std::string registerTimer(std::shared_ptr<ITimer> timer) override;
    void unregisterTimer(const std::string& timerId) override;
    std::shared_ptr<ITimer>
    getTimer(const std::string& timerId) const override;
    bool hasTimer(const std::string& timerId) const override;
    void stopAll() override;
    void setEventHandler(TimerEventHandler handler) override;

private:
    void stopAllInternal();

    mutable std::mutex m_mutex;
    std::unordered_map<std::string, std::shared_ptr<ITimer>> m_timers;
    std::uint64_t m_nextTimerId{ 1 };
    TimerEventHandler m_eventHandler;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
