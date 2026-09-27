#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Operating system socket protocol specification.
 */
enum class SocketProtocol : std::uint8_t {
    DEFAULT, /**< Default protocol for the domain and type (0). */
    TCP,     /**< Transmission Control Protocol. */
    UDP      /**< User Datagram Protocol. */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
