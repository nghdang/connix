#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Strongly typed error codes for timer management and execution.
 */
enum class TimerErrorCode : std::uint8_t {
    TIMER_NOT_FOUND,  /**< Specified timer identifier does not exist in
                         registry. */
    INVALID_DURATION, /**< Configured interval duration is zero or invalid. */
    TIMER_ALREADY_RUNNING, /**< Attempted to start an already running timer. */
    TIMER_NOT_RUNNING, /**< Attempted to stop or reset an inactive timer. */
    SYSTEM_CLOCK_ERROR /**< Underlying OS clock query failed. */
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
