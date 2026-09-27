#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class SocketType : std::uint8_t {
    STREAM,
    DATAGRAM
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
