#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Operating system socket communication type semantics.
 */
enum class SocketType : std::uint8_t {
    STREAM,  /**< Sequenced, reliable, bidirectional byte stream. */
    DATAGRAM /**< Connectionless, unreliable datagram buffer. */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
