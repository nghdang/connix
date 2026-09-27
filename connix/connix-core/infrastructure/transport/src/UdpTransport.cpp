#include "ConnixCore/Infrastructure/Transport/UdpTransport.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

UdpTransport::UdpTransport(std::unique_ptr<ISocket> socket)
    : m_socket(std::move(socket))
    , m_state(TransportState::CLOSED)
    , m_localEndpoint()
    , m_remoteEndpoint()
{
    if (!m_socket)
    {
        throw TransportException(TransportErrorCode::SOCKET_CREATION_FAILED,
                                 "Socket abstraction cannot be null");
    }
}

UdpTransport::UdpTransport(std::unique_ptr<ISocket> socket,
                           TransportState state,
                           const TransportEndpoint& localEndpoint,
                           const TransportEndpoint& remoteEndpoint)
    : m_socket(std::move(socket))
    , m_state(state)
    , m_localEndpoint(localEndpoint)
    , m_remoteEndpoint(remoteEndpoint)
{
    if (!m_socket)
    {
        throw TransportException(TransportErrorCode::SOCKET_CREATION_FAILED,
                                 "Socket abstraction cannot be null");
    }
}

UdpTransport::~UdpTransport()
{
    closeInternal();
}

void UdpTransport::bind(const TransportEndpoint& endpoint)
{
    if (m_state == TransportState::CONNECTED)
    {
        throw TransportException(
            TransportErrorCode::BIND_FAILED,
            "Cannot bind: transport is already connected");
    }

    if (!m_socket->isOpen())
    {
        const auto domain =
            (endpoint.getAddress().find(':') != std::string::npos)
                ? SocketDomain::IPV6
                : SocketDomain::IPV4;
        m_socket->open(domain, SocketType::DATAGRAM, SocketProtocol::UDP);
    }

    m_socket->setOption(SocketOption::REUSE_ADDRESS, true);
    m_socket->bind(endpoint);
    m_localEndpoint = endpoint;
    m_state = TransportState::BOUND;
}

void UdpTransport::listen(std::uint32_t /*backlog*/)
{
    throw TransportException(
        TransportErrorCode::LISTEN_FAILED,
        "Listen operation is not supported for UDP datagram transport");
}

std::unique_ptr<ITransport> UdpTransport::accept(std::uint32_t /*timeoutMs*/)
{
    throw TransportException(
        TransportErrorCode::ACCEPT_FAILED,
        "Accept operation is not supported for UDP datagram transport");
}

void UdpTransport::connect(const TransportEndpoint& endpoint,
                           std::uint32_t timeoutMs)
{
    if (!m_socket->isOpen())
    {
        const auto domain =
            (endpoint.getAddress().find(':') != std::string::npos)
                ? SocketDomain::IPV6
                : SocketDomain::IPV4;
        m_socket->open(domain, SocketType::DATAGRAM, SocketProtocol::UDP);
    }

    m_state = TransportState::CONNECTING;

    try
    {
        m_socket->connect(endpoint, timeoutMs);
        m_remoteEndpoint = endpoint;
        m_localEndpoint = m_socket->getLocalEndpoint();
        m_state = TransportState::CONNECTED;
    } catch (...)
    {
        m_socket->close();
        m_state = TransportState::CLOSED;
        throw;
    }
}

std::size_t UdpTransport::send(const std::vector<std::uint8_t>& data,
                               std::uint32_t timeoutMs)
{
    if (m_state == TransportState::CONNECTED)
    {
        try
        {
            return m_socket->send(data, timeoutMs);
        } catch (const TransportException& ex)
        {
            if (ex.getErrorCode() == TransportErrorCode::OPERATION_TIMEOUT)
            {
                m_socket->close();
                m_state = TransportState::CLOSED;
            }
            throw;
        }
    }

    if (m_state == TransportState::BOUND)
    {
        if (m_remoteEndpoint.getAddress().empty())
        {
            throw TransportException(
                TransportErrorCode::SEND_FAILED,
                "Cannot send: destination endpoint is not set for bound UDP "
                "transport");
        }

        try
        {
            return m_socket->sendTo(data, m_remoteEndpoint, timeoutMs);
        } catch (const TransportException& ex)
        {
            if (ex.getErrorCode() == TransportErrorCode::OPERATION_TIMEOUT)
            {
                m_socket->close();
                m_state = TransportState::CLOSED;
            }
            throw;
        }
    }

    throw TransportException(
        TransportErrorCode::SEND_FAILED,
        "Cannot send: transport is not connected or bound");
}

std::vector<std::uint8_t> UdpTransport::receive(std::size_t maxBytes,
                                                std::uint32_t timeoutMs)
{
    if (m_state == TransportState::CONNECTED)
    {
        try
        {
            return m_socket->receive(maxBytes, timeoutMs);
        } catch (const TransportException& ex)
        {
            if (ex.getErrorCode() == TransportErrorCode::OPERATION_TIMEOUT)
            {
                m_socket->close();
                m_state = TransportState::CLOSED;
            }
            throw;
        }
    }

    if (m_state == TransportState::BOUND)
    {
        try
        {
            return m_socket->receiveFrom(maxBytes, m_remoteEndpoint,
                                         timeoutMs);
        } catch (const TransportException& ex)
        {
            if (ex.getErrorCode() == TransportErrorCode::OPERATION_TIMEOUT)
            {
                m_socket->close();
                m_state = TransportState::CLOSED;
            }
            throw;
        }
    }

    throw TransportException(
        TransportErrorCode::RECEIVE_FAILED,
        "Cannot receive: transport is not connected or bound");
}

void UdpTransport::close()
{
    closeInternal();
}

void UdpTransport::closeInternal() noexcept
{
    if (m_socket)
    {
        m_socket->close();
    }
    m_state = TransportState::CLOSED;
    m_localEndpoint = TransportEndpoint();
    m_remoteEndpoint = TransportEndpoint();
}

bool UdpTransport::isOpen() const
{
    return m_socket && m_socket->isOpen();
}

TransportState UdpTransport::getState() const
{
    return m_state;
}

TransportProtocol UdpTransport::getProtocol() const
{
    return TransportProtocol::UDP;
}

const TransportEndpoint& UdpTransport::getLocalEndpoint() const
{
    return m_localEndpoint;
}

const TransportEndpoint& UdpTransport::getRemoteEndpoint() const
{
    return m_remoteEndpoint;
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
