#include "ConnixCore/Infrastructure/Transport/TcpTransport.hpp"

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

TcpTransport::TcpTransport(std::unique_ptr<ISocket> socket)
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

TcpTransport::TcpTransport(std::unique_ptr<ISocket> socket,
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

TcpTransport::~TcpTransport()
{
    closeInternal();
}

void TcpTransport::bind(const TransportEndpoint& endpoint)
{
    if (m_state == TransportState::LISTENING ||
        m_state == TransportState::CONNECTED)
    {
        throw TransportException(
            TransportErrorCode::BIND_FAILED,
            "Cannot bind: transport is already listening or connected");
    }

    if (!m_socket->isOpen())
    {
        const auto domain =
            (endpoint.getAddress().find(':') != std::string::npos)
                ? SocketDomain::IPV6
                : SocketDomain::IPV4;
        m_socket->open(domain, SocketType::STREAM, SocketProtocol::TCP);
    }

    m_socket->setOption(SocketOption::REUSE_ADDRESS, true);
    m_socket->bind(endpoint);
    m_localEndpoint = endpoint;
    m_state = TransportState::BOUND;
}

void TcpTransport::listen(std::uint32_t backlog)
{
    if (m_state != TransportState::BOUND)
    {
        throw TransportException(TransportErrorCode::LISTEN_FAILED,
                                 "Cannot listen: transport is not bound");
    }

    m_socket->listen(static_cast<std::int32_t>(backlog));
    m_state = TransportState::LISTENING;
}

std::unique_ptr<ITransport> TcpTransport::accept(std::uint32_t timeoutMs)
{
    if (m_state != TransportState::LISTENING)
    {
        throw TransportException(
            TransportErrorCode::ACCEPT_FAILED,
            "Cannot accept: transport is not in LISTENING state");
    }

    auto clientSocket = m_socket->accept(timeoutMs);
    if (!clientSocket)
    {
        throw TransportException(TransportErrorCode::ACCEPT_FAILED,
                                 "Socket accept returned null");
    }

    const auto localEp = clientSocket->getLocalEndpoint();
    const auto remoteEp = clientSocket->getRemoteEndpoint();

    return std::make_unique<TcpTransport>(
        std::move(clientSocket), TransportState::CONNECTED, localEp, remoteEp);
}

void TcpTransport::connect(const TransportEndpoint& endpoint,
                           std::uint32_t timeoutMs)
{
    if (m_state == TransportState::CONNECTED ||
        m_state == TransportState::LISTENING ||
        m_state == TransportState::CONNECTING)
    {
        throw TransportException(
            TransportErrorCode::CONNECT_FAILED,
            "Cannot connect: transport is already connecting, connected, "
            "or listening");
    }

    if (!m_socket->isOpen())
    {
        const auto domain =
            (endpoint.getAddress().find(':') != std::string::npos)
                ? SocketDomain::IPV6
                : SocketDomain::IPV4;
        m_socket->open(domain, SocketType::STREAM, SocketProtocol::TCP);
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

std::size_t TcpTransport::send(const std::vector<std::uint8_t>& data,
                               std::uint32_t timeoutMs)
{
    if (m_state != TransportState::CONNECTED)
    {
        throw TransportException(TransportErrorCode::SEND_FAILED,
                                 "Cannot send: transport is not connected");
    }

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
        else if (ex.getErrorCode() == TransportErrorCode::CONNECTION_CLOSED)
        {
            m_state = TransportState::DISCONNECTED;
        }
        throw;
    }
}

std::vector<std::uint8_t> TcpTransport::receive(std::size_t maxBytes,
                                                std::uint32_t timeoutMs)
{
    if (m_state != TransportState::CONNECTED)
    {
        throw TransportException(TransportErrorCode::RECEIVE_FAILED,
                                 "Cannot receive: transport is not connected");
    }

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
        else if (ex.getErrorCode() == TransportErrorCode::CONNECTION_CLOSED)
        {
            m_state = TransportState::DISCONNECTED;
        }
        throw;
    }
}

void TcpTransport::close()
{
    closeInternal();
}

void TcpTransport::closeInternal() noexcept
{
    if (m_socket)
    {
        m_socket->close();
    }
    m_state = TransportState::CLOSED;
    m_localEndpoint = TransportEndpoint();
    m_remoteEndpoint = TransportEndpoint();
}

bool TcpTransport::isOpen() const
{
    return m_socket && m_socket->isOpen();
}

TransportState TcpTransport::getState() const
{
    return m_state;
}

TransportProtocol TcpTransport::getProtocol() const
{
    return TransportProtocol::TCP;
}

const TransportEndpoint& TcpTransport::getLocalEndpoint() const
{
    return m_localEndpoint;
}

const TransportEndpoint& TcpTransport::getRemoteEndpoint() const
{
    return m_remoteEndpoint;
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
