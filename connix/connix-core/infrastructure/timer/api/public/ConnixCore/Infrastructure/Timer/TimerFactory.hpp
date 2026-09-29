#pragma once

#include <memory>

#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimerFactory.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Factory for creating concrete Timer instances.
 */
class TimerFactory : public ITimerFactory
{
public:
    /**
     * @brief Constructs a new TimerFactory.
     * @param clock Optional shared clock abstraction; defaults to standard
     * Clock.
     */
    explicit TimerFactory(std::shared_ptr<IClock> clock = nullptr);

    /**
     * @brief Default virtual destructor.
     */
    ~TimerFactory() override = default;

    std::shared_ptr<ITimer>
    createSingleShotTimer(TimerDuration interval,
                          TimerCallback callback = nullptr) override;

    std::shared_ptr<ITimer>
    createPeriodicTimer(TimerDuration interval,
                        TimerCallback callback = nullptr) override;

private:
    std::shared_ptr<IClock> m_clock;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
