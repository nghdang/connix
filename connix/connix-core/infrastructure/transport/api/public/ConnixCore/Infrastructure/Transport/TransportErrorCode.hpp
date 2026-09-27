#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

enum class TransportErrorCode : std::uint8_t {
    SOCKET_CREATION_FAILED,
    SOCKET_OPTION_FAILED,
    BIND_FAILED,
    LISTEN_FAILED,
    ACCEPT_FAILED,
    CONNECT_FAILED,
    SEND_FAILED,
    RECEIVE_FAILED,
    OPERATION_TIMEOUT,
    CONNECTION_CLOSED,
    INVALID_ADDRESS,
    UNSUPPORTED_PROTOCOL
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
