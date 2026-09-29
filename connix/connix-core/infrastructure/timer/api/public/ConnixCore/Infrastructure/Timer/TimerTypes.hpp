#pragma once

#include <chrono>
#include <functional>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Standard duration resolution used for timer intervals.
 */
using TimerDuration = std::chrono::milliseconds;

/**
 * @brief Standard monotonic timestamp representation used for timer deadlines.
 */
using TimerTimePoint = std::chrono::steady_clock::time_point;

/**
 * @brief Low-level expiration callback signature for individual timers.
 */
using TimerCallback = std::function<void()>;

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
