#include "ConnixCore/Infrastructure/Transport/UdsTransport.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
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

UdsTransport::UdsTransport(std::unique_ptr<ISocket> socket,
                           TransportProtocol protocol)
    : m_socket(std::move(socket))
    , m_protocol(protocol)
    , m_state(TransportState::CLOSED)
    , m_localEndpoint()
    , m_remoteEndpoint()
{
    if (!m_socket)
    {
        throw TransportException(TransportErrorCode::SOCKET_CREATION_FAILED,
                                 "Socket abstraction cannot be null");
    }

    if (m_protocol != TransportProtocol::UDS_STREAM &&
        m_protocol != TransportProtocol::UDS_DATAGRAM)
    {
        throw TransportException(
            TransportErrorCode::UNSUPPORTED_PROTOCOL,
            "Unsupported protocol for UdsTransport: must be UDS_STREAM or "
            "UDS_DATAGRAM");
    }
}

UdsTransport::UdsTransport(std::unique_ptr<ISocket> socket,
                           TransportProtocol protocol, TransportState state,
                           const TransportEndpoint& localEndpoint,
                           const TransportEndpoint& remoteEndpoint)
    : m_socket(std::move(socket))
    , m_protocol(protocol)
    , m_state(state)
    , m_localEndpoint(localEndpoint)
    , m_remoteEndpoint(remoteEndpoint)
{
    if (!m_socket)
    {
        throw TransportException(TransportErrorCode::SOCKET_CREATION_FAILED,
                                 "Socket abstraction cannot be null");
    }

    if (m_protocol != TransportProtocol::UDS_STREAM &&
        m_protocol != TransportProtocol::UDS_DATAGRAM)
    {
        throw TransportException(
            TransportErrorCode::UNSUPPORTED_PROTOCOL,
            "Unsupported protocol for UdsTransport: must be UDS_STREAM or "
            "UDS_DATAGRAM");
    }
}

UdsTransport::~UdsTransport()
{
    closeInternal();
}

void UdsTransport::bind(const TransportEndpoint& endpoint)
{
    if (m_state == TransportState::LISTENING ||
        m_state == TransportState::CONNECTED)
    {
        throw TransportException(
            TransportErrorCode::BIND_FAILED,
            "Cannot bind: transport is already listening or connected");
    }

    if (endpoint.getAddress().empty())
    {
        throw TransportException(TransportErrorCode::INVALID_ADDRESS,
                                 "Cannot bind: UDS path cannot be empty");
    }

    if (!m_socket->isOpen())
    {
        const auto type = (m_protocol == TransportProtocol::UDS_DATAGRAM)
                              ? SocketType::DATAGRAM
                              : SocketType::STREAM;
        m_socket->open(SocketDomain::UNIX, type, SocketProtocol::DEFAULT);
    }

    m_socket->bind(endpoint);
    m_localEndpoint = endpoint;
    m_state = TransportState::BOUND;
}

void UdsTransport::listen(std::uint32_t backlog)
{
    if (m_protocol == TransportProtocol::UDS_DATAGRAM)
    {
        throw TransportException(
            TransportErrorCode::LISTEN_FAILED,
            "Listen operation is not supported for UDS datagram transport");
    }

    if (m_state != TransportState::BOUND)
    {
        throw TransportException(TransportErrorCode::LISTEN_FAILED,
                                 "Cannot listen: transport is not bound");
    }

    m_socket->listen(static_cast<std::int32_t>(backlog));
    m_state = TransportState::LISTENING;
}

std::unique_ptr<ITransport> UdsTransport::accept(std::uint32_t timeoutMs)
{
    if (m_protocol == TransportProtocol::UDS_DATAGRAM)
    {
        throw TransportException(
            TransportErrorCode::ACCEPT_FAILED,
            "Accept operation is not supported for UDS datagram transport");
    }

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

    return std::make_unique<UdsTransport>(std::move(clientSocket), m_protocol,
                                          TransportState::CONNECTED, localEp,
                                          remoteEp);
}

void UdsTransport::connect(const TransportEndpoint& endpoint,
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

    if (endpoint.getAddress().empty())
    {
        throw TransportException(
            TransportErrorCode::INVALID_ADDRESS,
            "Cannot connect: UDS destination path cannot be empty");
    }

    if (!m_socket->isOpen())
    {
        const auto type = (m_protocol == TransportProtocol::UDS_DATAGRAM)
                              ? SocketType::DATAGRAM
                              : SocketType::STREAM;
        m_socket->open(SocketDomain::UNIX, type, SocketProtocol::DEFAULT);
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

std::size_t UdsTransport::send(const std::vector<std::uint8_t>& data,
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
            else if (ex.getErrorCode() ==
                     TransportErrorCode::CONNECTION_CLOSED)
            {
                m_state = TransportState::DISCONNECTED;
            }
            throw;
        }
    }

    if (m_state == TransportState::BOUND &&
        m_protocol == TransportProtocol::UDS_DATAGRAM)
    {
        if (m_remoteEndpoint.getAddress().empty())
        {
            throw TransportException(
                TransportErrorCode::SEND_FAILED,
                "Cannot send: destination endpoint is not set for bound UDS "
                "datagram transport");
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

    throw TransportException(TransportErrorCode::SEND_FAILED,
                             "Cannot send: transport is not connected");
}

std::vector<std::uint8_t> UdsTransport::receive(std::size_t maxBytes,
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
            else if (ex.getErrorCode() ==
                     TransportErrorCode::CONNECTION_CLOSED)
            {
                m_state = TransportState::DISCONNECTED;
            }
            throw;
        }
    }

    if (m_state == TransportState::BOUND &&
        m_protocol == TransportProtocol::UDS_DATAGRAM)
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

    throw TransportException(TransportErrorCode::RECEIVE_FAILED,
                             "Cannot receive: transport is not connected");
}

void UdsTransport::close()
{
    closeInternal();
}

void UdsTransport::closeInternal() noexcept
{
    if (m_socket)
    {
        m_socket->close();
    }
    m_state = TransportState::CLOSED;
    m_localEndpoint = TransportEndpoint();
    m_remoteEndpoint = TransportEndpoint();
}

bool UdsTransport::isOpen() const
{
    return m_socket && m_socket->isOpen();
}

TransportState UdsTransport::getState() const
{
    return m_state;
}

TransportProtocol UdsTransport::getProtocol() const
{
    return m_protocol;
}

const TransportEndpoint& UdsTransport::getLocalEndpoint() const
{
    return m_localEndpoint;
}

const TransportEndpoint& UdsTransport::getRemoteEndpoint() const
{
    return m_remoteEndpoint;
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
