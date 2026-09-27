#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class TransportProtocol : std::uint8_t {
    TCP,
    UDP,
    UDS_STREAM,
    UDS_DATAGRAM
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
