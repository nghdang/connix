#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Configurable socket-level options.
 */
enum class SocketOption : std::uint8_t {
    REUSE_ADDRESS,   /**< Allow local address reuse. */
    NON_BLOCKING,    /**< Set non-blocking I/O mode. */
    RECEIVE_TIMEOUT, /**< Set socket receive timeout bound. */
    SEND_TIMEOUT     /**< Set socket send timeout bound. */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
