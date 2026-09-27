#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class TransportState : std::uint8_t {
    CLOSED,
    BOUND,
    LISTENING,
    CONNECTING,
    CONNECTED,
    DISCONNECTED
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
