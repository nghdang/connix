#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Common interface for network and IPC communication transports.
 *
 * Provides connection lifecycle management, data transfer operations,
 * and state inspection across supported transport protocols.
 */
class ITransport
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~ITransport() = default;

    /**
     * @brief Binds the transport to a local endpoint.
     * @param endpoint Local address and port, or UDS path.
     */
    virtual void bind(const TransportEndpoint& endpoint) = 0;

    /**
     * @brief Places a stream transport into listening mode.
     * @param backlog Maximum length of queue of pending connections.
     */
    virtual void listen(std::uint32_t backlog) = 0;

    /**
     * @brief Accepts an inbound peer connection on a listening transport.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Unique pointer to the connected client transport instance.
     */
    virtual std::unique_ptr<ITransport> accept(std::uint32_t timeoutMs) = 0;

    /**
     * @brief Establishes an outbound connection to a remote endpoint.
     * @param endpoint Remote destination endpoint address and port or path.
     * @param timeoutMs Maximum time to wait in milliseconds.
     */
    virtual void connect(const TransportEndpoint& endpoint,
                         std::uint32_t timeoutMs) = 0;

    /**
     * @brief Transmits data over the active transport connection.
     * @param data Byte buffer to send.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Number of bytes actually sent.
     */
    virtual std::size_t send(const std::vector<std::uint8_t>& data,
                             std::uint32_t timeoutMs) = 0;

    /**
     * @brief Receives incoming data from the active transport connection.
     * @param maxBytes Maximum number of bytes to receive.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Vector containing received byte payload.
     */
    virtual std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                              std::uint32_t timeoutMs) = 0;

    /**
     * @brief Closes the transport and releases underlying socket resources.
     */
    virtual void close() = 0;

    /**
     * @brief Checks if the transport is currently open.
     * @return True if the transport has an active socket descriptor.
     */
    virtual bool isOpen() const = 0;

    /**
     * @brief Retrieves the current lifecycle state of the transport.
     * @return Current TransportState enum value.
     */
    virtual TransportState getState() const = 0;

    /**
     * @brief Retrieves the protocol handled by this transport instance.
     * @return TransportProtocol enum value.
     */
    virtual TransportProtocol getProtocol() const = 0;

    /**
     * @brief Retrieves the local endpoint bound to this transport.
     * @return Const reference to local TransportEndpoint.
     */
    virtual const TransportEndpoint& getLocalEndpoint() const = 0;

    /**
     * @brief Retrieves the remote endpoint connected to this transport.
     * @return Const reference to remote peer TransportEndpoint.
     */
    virtual const TransportEndpoint& getRemoteEndpoint() const = 0;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
