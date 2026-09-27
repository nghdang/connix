#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class NodeTransport : std::uint8_t {
    TCP,
    UDP,
    UDS_STREAM,
    UDS_DATAGRAM
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
