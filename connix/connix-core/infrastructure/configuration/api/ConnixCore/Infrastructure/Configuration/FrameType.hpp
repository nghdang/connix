#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class FrameType : std::uint8_t {
    FIXED_SIZE,
    LENGTH_PREFIX
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
