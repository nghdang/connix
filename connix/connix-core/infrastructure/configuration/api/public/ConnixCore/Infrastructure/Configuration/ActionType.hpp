#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class ActionType : std::uint8_t {
    RESPOND,
    SEND
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
