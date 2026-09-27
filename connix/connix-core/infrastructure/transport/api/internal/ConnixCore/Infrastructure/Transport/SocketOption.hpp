#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class SocketOption : std::uint8_t {
    REUSE_ADDRESS,
    NON_BLOCKING,
    RECEIVE_TIMEOUT,
    SEND_TIMEOUT
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
