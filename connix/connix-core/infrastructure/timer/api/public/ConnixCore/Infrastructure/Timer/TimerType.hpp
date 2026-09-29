#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Classification of timer recurrence behavior.
 */
enum class TimerType : std::uint8_t {
    SINGLE_SHOT, /**< One-time timer that fires once upon elapsed interval. */
    PERIODIC /**< Recurring timer that fires repeatedly at interval boundaries.
              */
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
