#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Strongly typed error codes for transport and socket operations.
 */
enum class TransportErrorCode : std::uint8_t {
    SOCKET_CREATION_FAILED, /**< Failed to create OS socket. */
    SOCKET_OPTION_FAILED,   /**< Failed to configure socket option. */
    BIND_FAILED,            /**< Failed to bind socket to local endpoint. */
    LISTEN_FAILED,          /**< Failed to set stream socket to listen. */
    ACCEPT_FAILED,          /**< Failed to accept incoming connection. */
    CONNECT_FAILED,         /**< Failed to connect to remote endpoint. */
    SEND_FAILED,            /**< Failed to transmit data over socket. */
    RECEIVE_FAILED,         /**< Failed to receive data from socket. */
    OPERATION_TIMEOUT,      /**< Operation exceeded timeout bound. */
    CONNECTION_CLOSED,      /**< Connection closed unexpectedly or by peer. */
    INVALID_ADDRESS,        /**< Provided endpoint address is invalid. */
    UNSUPPORTED_PROTOCOL    /**< Requested protocol is not registered. */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
