#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Operating system socket address family / domain.
 */
enum class SocketDomain : std::uint8_t {
    IPV4, /**< IPv4 Internet domain (AF_INET). */
    IPV6, /**< IPv6 Internet domain (AF_INET6). */
    UNIX  /**< Unix domain local socket (AF_UNIX). */
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
