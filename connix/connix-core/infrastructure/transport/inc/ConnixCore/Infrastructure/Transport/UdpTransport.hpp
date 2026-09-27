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
 * @brief Concrete UDP datagram implementation of the ITransport interface.
 *
 * Encapsulates connectionless, datagram-oriented communication over IPv4
 * and IPv6, delegating low-level socket descriptor operations to an injected
 * ISocket instance.
 */
class UdpTransport : public ITransport
{
public:
    /**
     * @brief Constructs a UdpTransport with an injected socket implementation.
     * @param socket Unique pointer to the low-level ISocket abstraction.
     * @throws TransportException If socket is null.
     */
    explicit UdpTransport(std::unique_ptr<ISocket> socket);

    /**
     * @brief Constructs an initialized UdpTransport instance with state.
     * @param socket Initialized ISocket descriptor instance.
     * @param state Initial lifecycle TransportState.
     * @param localEndpoint Bound local endpoint.
     * @param remoteEndpoint Remote peer endpoint.
     * @throws TransportException If socket is null.
     */
    UdpTransport(std::unique_ptr<ISocket> socket, TransportState state,
                 const TransportEndpoint& localEndpoint,
                 const TransportEndpoint& remoteEndpoint);

    /**
     * @brief Deleted copy constructor (move-only resource).
     */
    UdpTransport(const UdpTransport&) = delete;

    /**
     * @brief Deleted copy assignment operator (move-only resource).
     */
    UdpTransport& operator=(const UdpTransport&) = delete;

    /**
     * @brief Default move constructor.
     */
    UdpTransport(UdpTransport&&) noexcept = default;

    /**
     * @brief Default move assignment operator.
     */
    UdpTransport& operator=(UdpTransport&&) noexcept = default;

    /**
     * @brief Destructor closing active socket resources.
     */
    ~UdpTransport() override;

    /**
     * @brief Binds the UDP socket to a local endpoint for receiving datagrams.
     * @param endpoint Local IP address and port to bind.
     * @throws TransportException If binding fails or state is invalid.
     */
    void bind(const TransportEndpoint& endpoint) override;

    /**
     * @brief Places transport into listening state (unsupported for UDP).
     * @param backlog Maximum queue length.
     * @throws TransportException Always, as UDP is connectionless.
     */
    void listen(std::uint32_t backlog) override;

    /**
     * @brief Accepts an incoming connection (unsupported for UDP).
     * @param timeoutMs Maximum wait time in milliseconds.
     * @return Never returns normally.
     * @throws TransportException Always, as UDP is connectionless.
     */
    std::unique_ptr<ITransport> accept(std::uint32_t timeoutMs) override;

    /**
     * @brief Associates a default remote destination endpoint with the socket.
     * @param endpoint Destination IP address and port.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @throws TransportException On connection failure or timeout.
     */
    void connect(const TransportEndpoint& endpoint,
                 std::uint32_t timeoutMs) override;

    /**
     * @brief Transmits a datagram payload over UDP.
     * @param data Byte vector to send.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Exact number of bytes transmitted.
     * @throws TransportException On send failure, missing destination,
     *         or timeout.
     */
    std::size_t send(const std::vector<std::uint8_t>& data,
                     std::uint32_t timeoutMs) override;

    /**
     * @brief Receives an incoming datagram from the UDP socket.
     * @param maxBytes Maximum number of bytes to read into buffer.
     * @param timeoutMs Maximum time to wait in milliseconds.
     * @return Byte vector containing received payload.
     * @throws TransportException On receive error, un-bound state, or timeout.
     */
    std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                      std::uint32_t timeoutMs) override;

    /**
     * @brief Closes the UDP transport and underlying socket descriptor.
     */
    void close() override;

    /**
     * @brief Checks if the transport currently holds an open socket.
     * @return True if socket is open, false otherwise.
     */
    bool isOpen() const override;

    /**
     * @brief Retrieves the current lifecycle state of the UDP transport.
     * @return Current TransportState enum value.
     */
    TransportState getState() const override;

    /**
     * @brief Retrieves the active transport protocol.
     * @return TransportProtocol::UDP.
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
    TransportState m_state;
    TransportEndpoint m_localEndpoint;
    TransportEndpoint m_remoteEndpoint;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
