#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Identifies the communication protocol used by a transport instance.
 */
enum class TransportProtocol : std::uint8_t {
    TCP,         /**< Transmission Control Protocol (stream-oriented). */
    UDP,         /**< User Datagram Protocol (datagram-oriented). */
    UDS_STREAM,  /**< Unix Domain Socket in stream mode. */
    UDS_DATAGRAM /**< Unix Domain Socket in datagram mode. */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
