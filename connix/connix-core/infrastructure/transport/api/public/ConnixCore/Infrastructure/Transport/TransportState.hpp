#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Represents the lifecycle state of a transport connection.
 */
enum class TransportState : std::uint8_t {
    CLOSED,      /**< Transport is unallocated or explicitly closed. */
    BOUND,       /**< Socket descriptor is bound to a local endpoint. */
    LISTENING,   /**< Transport is listening for incoming connections. */
    CONNECTING,  /**< Outbound connection establishment is in progress. */
    CONNECTED,   /**< Connection is established and ready for I/O. */
    DISCONNECTED /**< Peer connection terminated or closed remotely. */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
