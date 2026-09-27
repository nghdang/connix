#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class SocketDomain : std::uint8_t {
    IPV4,
    IPV6,
    UNIX
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
