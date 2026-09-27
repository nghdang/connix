#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Concrete Unix Domain Socket implementation of the ITransport
 *        interface.
 *
 * Encapsulates stream-oriented (and datagram-oriented) IPC communication over
 * filesystem socket paths via AF_UNIX, delegating low-level socket descriptor
 * operations to an injected ISocket instance.
 */
class UdsTransport : public ITransport
{
public:
    /**
     * @brief Constructs a UdsTransport with an injected socket implementation.
     * @param socket Unique pointer to the low-level ISocket abstraction.
     * @param protocol TransportProtocol (defaults to UDS_STREAM).
     * @throws TransportException If socket is null or protocol is not a UDS
     *         protocol.
     */
    explicit UdsTransport(
        std::unique_ptr<ISocket> socket,
        TransportProtocol protocol = TransportProtocol::UDS_STREAM);

    /**
     * @brief Constructs an initialized UdsTransport instance with state.
     * @param socket Initialized ISocket descriptor instance.
     * @param protocol TransportProtocol (UDS_STREAM or UDS_DATAGRAM).
     * @param state Initial lifecycle TransportState.
     * @param localEndpoint Bound local endpoint.
     * @param remoteEndpoint Remote peer endpoint.
     * @throws TransportException If socket is null or protocol is not a UDS
     *         protocol.
     */
    UdsTransport(std::unique_ptr<ISocket> socket, TransportProtocol protocol,
                 TransportState state, const TransportEndpoint& localEndpoint,
                 const TransportEndpoint& remoteEndpoint);

    /**
     * @brief Deleted copy constructor (move-only resource).
     */
    UdsTransport(const UdsTransport&) = delete;

    /**
     * @brief Deleted copy assignment operator (move-only resource).
     */
    UdsTransport& operator=(const UdsTransport&) = delete;

    /**
     * @brief Default move constructor.
     */
    UdsTransport(UdsTransport&&) noexcept = default;

    /**
     * @brief Default move assignment operator.
     */
    UdsTransport& operator=(UdsTransport&&) noexcept = default;

    /**
     * @brief Destructor closing active socket resources.
     */
    ~UdsTransport() override;

    /**
     * @brief Binds the UDS socket to a local filesystem path endpoint.
     * @param endpoint Local UDS filesystem path endpoint.
     * @throws TransportException If binding fails or state is invalid.
     */
    void bind(const TransportEndpoint& endpoint) override;

    /**
     * @brief Places a stream UDS transport into listening state.
     * @param backlog Maximum length of queued pending connections.
     * @throws TransportException If listen fails or transport is not bound.
     */
    void listen(std::uint32_t backlog) override;

    /**
     * @brief Accepts an incoming client connection on a listening UDS
     * transport.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Unique pointer to the connected client ITransport.
     * @throws TransportException On accept failure, unsupported protocol, or
     *         timeout.
     */
    std::unique_ptr<ITransport> accept(std::uint32_t timeoutMs) override;

    /**
     * @brief Establishes an outbound connection to a target UDS socket path.
     * @param endpoint Destination UDS filesystem path endpoint.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @throws TransportException On connection failure or timeout.
     */
    void connect(const TransportEndpoint& endpoint,
                 std::uint32_t timeoutMs) override;

    /**
     * @brief Transmits a byte payload over the UDS connection.
     * @param data Byte vector to send.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Exact number of bytes transmitted.
     * @throws TransportException On send failure or timeout.
     */
    std::size_t send(const std::vector<std::uint8_t>& data,
                     std::uint32_t timeoutMs) override;

    /**
     * @brief Receives incoming bytes from the UDS connection.
     * @param maxBytes Maximum number of bytes to read into buffer.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Byte vector containing received payload.
     * @throws TransportException On receive error, disconnect, or timeout.
     */
    std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                      std::uint32_t timeoutMs) override;

    /**
     * @brief Closes the UDS transport and underlying socket descriptor.
     */
    void close() override;

    /**
     * @brief Checks if the transport currently holds an open socket.
     * @return True if socket is open, false otherwise.
     */
    bool isOpen() const override;

    /**
     * @brief Retrieves the current lifecycle state of the UDS transport.
     * @return Current TransportState enum value.
     */
    TransportState getState() const override;

    /**
     * @brief Retrieves the active transport protocol.
     * @return TransportProtocol (UDS_STREAM or UDS_DATAGRAM).
     */
    TransportProtocol getProtocol() const override;

    /**
     * @brief Retrieves the local endpoint bound to this transport.
     * @return Const reference to local TransportEndpoint.
     */
    const TransportEndpoint& getLocalEndpoint() const override;

    /**
     * @brief Retrieves the remote endpoint associated with this transport.
     * @return Const reference to remote TransportEndpoint.
     */
    const TransportEndpoint& getRemoteEndpoint() const override;

private:
    void closeInternal() noexcept;

    std::unique_ptr<ISocket> m_socket;
    TransportProtocol m_protocol;
    TransportState m_state;
    TransportEndpoint m_localEndpoint;
    TransportEndpoint m_remoteEndpoint;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
