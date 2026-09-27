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
 * @brief Concrete TCP stream implementation of the ITransport interface.
 *
 * Encapsulates client and server TCP connection lifecycles over IPv4 and IPv6
 * delegating low-level descriptor operations to an injected ISocket instance.
 */
class TcpTransport : public ITransport
{
public:
    /**
     * @brief Constructs a TcpTransport with an injected socket implementation.
     * @param socket Unique pointer to the low-level ISocket abstraction.
     * @throws TransportException If socket is null.
     */
    explicit TcpTransport(std::unique_ptr<ISocket> socket);

    /**
     * @brief Constructs an already connected TcpTransport instance.
     * @param socket Connected ISocket descriptor instance.
     * @param state Initial lifecycle TransportState.
     * @param localEndpoint Bound local endpoint of the accepted peer.
     * @param remoteEndpoint Remote endpoint of the accepted peer.
     * @throws TransportException If socket is null.
     */
    TcpTransport(std::unique_ptr<ISocket> socket, TransportState state,
                 const TransportEndpoint& localEndpoint,
                 const TransportEndpoint& remoteEndpoint);

    /**
     * @brief Deleted copy constructor (move-only resource).
     */
    TcpTransport(const TcpTransport&) = delete;

    /**
     * @brief Deleted copy assignment operator (move-only resource).
     */
    TcpTransport& operator=(const TcpTransport&) = delete;

    /**
     * @brief Default move constructor.
     */
    TcpTransport(TcpTransport&&) noexcept = default;

    /**
     * @brief Default move assignment operator.
     */
    TcpTransport& operator=(TcpTransport&&) noexcept = default;

    /**
     * @brief Destructor closing active socket resources.
     */
    ~TcpTransport() override;

    /**
     * @brief Binds the TCP socket to a local endpoint for server operation.
     * @param endpoint Local IP address and port to bind.
     * @throws TransportException If binding fails or state is invalid.
     */
    void bind(const TransportEndpoint& endpoint) override;

    /**
     * @brief Places the bound TCP transport into listening state.
     * @param backlog Maximum length of queued pending connections.
     * @throws TransportException If listen fails or transport is not bound.
     */
    void listen(std::uint32_t backlog) override;

    /**
     * @brief Accepts an incoming client connection on a listening TCP
     * transport.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Unique pointer to the connected client ITransport.
     * @throws TransportException On accept failure or timeout.
     */
    std::unique_ptr<ITransport> accept(std::uint32_t timeoutMs) override;

    /**
     * @brief Establishes an outbound connection to a remote TCP endpoint.
     * @param endpoint Destination IP address and port.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @throws TransportException On connection failure or timeout.
     */
    void connect(const TransportEndpoint& endpoint,
                 std::uint32_t timeoutMs) override;

    /**
     * @brief Transmits a byte payload over the established TCP stream.
     * @param data Byte vector to send.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Exact number of bytes transmitted.
     * @throws TransportException On send failure or timeout.
     */
    std::size_t send(const std::vector<std::uint8_t>& data,
                     std::uint32_t timeoutMs) override;

    /**
     * @brief Receives incoming bytes from the TCP stream.
     * @param maxBytes Maximum number of bytes to read into buffer.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Byte vector containing received payload.
     * @throws TransportException On receive error, disconnect, or timeout.
     */
    std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                      std::uint32_t timeoutMs) override;

    /**
     * @brief Closes the TCP transport and underlying socket descriptor.
     */
    void close() override;

    /**
     * @brief Checks if the transport currently holds an open socket.
     * @return True if socket is open, false otherwise.
     */
    bool isOpen() const override;

    /**
     * @brief Retrieves the current lifecycle state of the TCP transport.
     * @return Current TransportState enum value.
     */
    TransportState getState() const override;

    /**
     * @brief Retrieves the active transport protocol.
     * @return TransportProtocol::TCP.
     */
    TransportProtocol getProtocol() const override;

    /**
     * @brief Retrieves the local endpoint bound to this transport.
     * @return Const reference to local TransportEndpoint.
     */
    const TransportEndpoint& getLocalEndpoint() const override;

    /**
     * @brief Retrieves the remote endpoint connected to this transport.
     * @return Const reference to remote TransportEndpoint.
     */
    const TransportEndpoint& getRemoteEndpoint() const override;

private:
    void closeInternal() noexcept;

    std::unique_ptr<ISocket> m_socket;
    TransportState m_state;
    TransportEndpoint m_localEndpoint;
    TransportEndpoint m_remoteEndpoint;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
