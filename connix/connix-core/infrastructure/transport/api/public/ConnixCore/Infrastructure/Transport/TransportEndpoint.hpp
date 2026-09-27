#pragma once

#include <cstdint>
#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Value object representing a network or IPC communication endpoint.
 *
 * Encapsulates IPv4/IPv6 addresses and port numbers, or Unix Domain Socket
 * filesystem paths.
 */
class TransportEndpoint
{
public:
    /**
     * @brief Constructs an empty default endpoint.
     */
    TransportEndpoint();

    /**
     * @brief Constructs an IP-based endpoint.
     * @param address IP address string (IPv4 dotted-decimal or IPv6).
     * @param port 16-bit port number.
     */
    TransportEndpoint(const std::string& address, std::uint16_t port);

    /**
     * @brief Constructs a Unix Domain Socket endpoint.
     * @param path Filesystem path to the domain socket.
     */
    explicit TransportEndpoint(const std::string& path);

    /**
     * @brief Default destructor.
     */
    ~TransportEndpoint() = default;

    /**
     * @brief Retrieves the endpoint address or filesystem path.
     * @return Reference to the address or path string.
     */
    const std::string& getAddress() const;

    /**
     * @brief Retrieves the 16-bit port number.
     * @return Port number, or 0 for Unix Domain Sockets.
     */
    std::uint16_t getPort() const;

    /**
     * @brief Checks if this endpoint represents a Unix Domain Socket.
     * @return True if the endpoint is a Unix Domain Socket, false otherwise.
     */
    bool isUnixDomain() const;

    /**
     * @brief Formats the endpoint as a readable string.
     * @return Formatted string ("address:port", "unix:path", or "").
     */
    std::string toString() const;

    /**
     * @brief Equality comparison operator.
     * @param other Endpoint to compare with.
     * @return True if addresses and ports match, false otherwise.
     */
    bool operator==(const TransportEndpoint& other) const;

    /**
     * @brief Inequality comparison operator.
     * @param other Endpoint to compare with.
     * @return True if addresses or ports differ, false otherwise.
     */
    bool operator!=(const TransportEndpoint& other) const;

private:
    std::string m_address;
    std::uint16_t m_port;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
