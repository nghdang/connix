#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class SocketProtocol : std::uint8_t {
    DEFAULT,
    TCP,
    UDP
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
