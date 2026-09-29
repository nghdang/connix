#include "ConnixCore/Infrastructure/Timer/Clock.hpp"

#include <chrono>
#include <thread>

#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

TimerTimePoint Clock::now() const
{
    return std::chrono::steady_clock::now();
}

void Clock::sleepFor(TimerDuration duration)
{
    std::this_thread::sleep_for(duration);
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
