#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Execution lifecycle states of an individual timer.
 */
enum class TimerState : std::uint8_t {
    STOPPED, /**< Timer is disarmed and inactive. */
    RUNNING, /**< Timer is armed and counting down toward deadline. */
    EXPIRED  /**< Single-shot timer has elapsed its configured interval. */
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
