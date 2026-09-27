#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Internal abstraction for operating system socket descriptors and I/O.
 */
class ISocket
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~ISocket() = default;

    /**
     * @brief Allocates and initializes the operating system socket descriptor.
     * @param domain Address domain (IPV4, IPV6, or UNIX).
     * @param type Socket communication type (STREAM or DATAGRAM).
     * @param protocol Specific protocol (DEFAULT, TCP, or UDP).
     */
    virtual void open(SocketDomain domain, SocketType type,
                      SocketProtocol protocol) = 0;

    /**
     * @brief Binds the socket descriptor to a local endpoint address.
     * @param endpoint Local endpoint address and port or path.
     */
    virtual void bind(const TransportEndpoint& endpoint) = 0;

    /**
     * @brief Places a stream-oriented socket into listening state.
     * @param backlog Maximum length of queued pending connections.
     */
    virtual void listen(std::int32_t backlog) = 0;

    /**
     * @brief Accepts an incoming connection on a listening socket.
     * @param timeoutMs Maximum wait time in milliseconds.
     * @return New connected socket instance for the accepted peer.
     */
    virtual std::unique_ptr<ISocket> accept(std::uint32_t timeoutMs) = 0;

    /**
     * @brief Connects to a remote peer endpoint within a bounded timeout.
     * @param endpoint Remote destination endpoint address and port or path.
     * @param timeoutMs Maximum wait time in milliseconds.
     */
    virtual void connect(const TransportEndpoint& endpoint,
                         std::uint32_t timeoutMs) = 0;

    /**
     * @brief Sends data over a connected stream socket descriptor.
     * @param data Byte payload to transmit.
     * @param timeoutMs Maximum wait time in milliseconds.
     * @return Number of bytes actually written.
     */
    virtual std::size_t send(const std::vector<std::uint8_t>& data,
                             std::uint32_t timeoutMs) = 0;

    /**
     * @brief Receives data from a connected stream socket descriptor.
     * @param maxBytes Maximum number of bytes to read into buffer.
     * @param timeoutMs Maximum wait time in milliseconds.
     * @return Received byte vector.
     */
    virtual std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                              std::uint32_t timeoutMs) = 0;

    /**
     * @brief Transmits a datagram to a specific remote destination.
     * @param data Byte payload to send.
     * @param destination Target remote endpoint.
     * @param timeoutMs Maximum wait time in milliseconds.
     * @return Number of bytes transmitted.
     */
    virtual std::size_t sendTo(const std::vector<std::uint8_t>& data,
                               const TransportEndpoint& destination,
                               std::uint32_t timeoutMs) = 0;

    /**
     * @brief Receives a datagram and captures the originating source address.
     * @param maxBytes Maximum number of bytes to receive.
     * @param source Output parameter receiving the sender endpoint.
     * @param timeoutMs Maximum wait time in milliseconds.
     * @return Received byte vector.
     */
    virtual std::vector<std::uint8_t> receiveFrom(std::size_t maxBytes,
                                                  TransportEndpoint& source,
                                                  std::uint32_t timeoutMs) = 0;

    /**
     * @brief Sets or toggles a socket option.
     * @param option Socket option enum value.
     * @param enable Flag indicating whether option should be enabled.
     */
    virtual void setOption(SocketOption option, bool enable) = 0;

    /**
     * @brief Closes the socket descriptor and unlinks any associated paths.
     */
    virtual void close() = 0;

    /**
     * @brief Checks whether the socket descriptor is currently valid and open.
     * @return True if socket descriptor is valid, false otherwise.
     */
    virtual bool isOpen() const = 0;

    /**
     * @brief Retrieves the raw operating system file descriptor handle.
     * @return Native socket file descriptor integer.
     */
    virtual std::int32_t getNativeHandle() const = 0;

    /**
     * @brief Retrieves the cached local endpoint bound to this socket.
     * @return Const reference to local TransportEndpoint.
     */
    virtual const TransportEndpoint& getLocalEndpoint() const = 0;

    /**
     * @brief Retrieves the remote peer endpoint associated with this socket.
     * @return Const reference to remote TransportEndpoint.
     */
    virtual const TransportEndpoint& getRemoteEndpoint() const = 0;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
