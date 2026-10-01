#include "ConnixCore/Infrastructure/Transport/Socket.hpp"

#include <arpa/inet.h>
#include <array>
#include <asm-generic/socket.h>
#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"

namespace {

struct SocketAddressBuffer {
    alignas(struct sockaddr_storage) struct sockaddr sa {
    };
    std::array<char, sizeof(struct sockaddr_storage) - sizeof(struct sockaddr)>
        padding{};
};

struct NativeAddress {
    SocketAddressBuffer addr{};
    socklen_t len{ 0 };
};

NativeAddress toNativeAddress(
    const ConnixCore::Infrastructure::Transport::TransportEndpoint& endpoint,
    ConnixCore::Infrastructure::Transport::SocketDomain domain)
{
    if (endpoint.getAddress().empty())
    {
        throw ConnixCore::Infrastructure::Transport::TransportException(
            ConnixCore::Infrastructure::Transport::TransportErrorCode::
                INVALID_ADDRESS,
            "Endpoint address cannot be empty");
    }

    NativeAddress result;

    switch (domain)
    {
        case ConnixCore::Infrastructure::Transport::SocketDomain::IPV4:
        {
            struct sockaddr_in in {
            };
            in.sin_family = AF_INET;
            in.sin_port = htons(endpoint.getPort());
            if (::inet_pton(AF_INET, endpoint.getAddress().c_str(),
                            &in.sin_addr) <= 0)
            {
                throw ConnixCore::Infrastructure::Transport::
                    TransportException(ConnixCore::Infrastructure::Transport::
                                           TransportErrorCode::INVALID_ADDRESS,
                                       "Invalid IPv4 address: " +
                                           endpoint.getAddress());
            }
            std::memcpy(&result.addr.sa, &in, sizeof(in));
            result.len = sizeof(struct sockaddr_in);
            break;
        }
        case ConnixCore::Infrastructure::Transport::SocketDomain::IPV6:
        {
            struct sockaddr_in6 in6 {
            };
            in6.sin6_family = AF_INET6;
            in6.sin6_port = htons(endpoint.getPort());
            if (::inet_pton(AF_INET6, endpoint.getAddress().c_str(),
                            &in6.sin6_addr) <= 0)
            {
                throw ConnixCore::Infrastructure::Transport::
                    TransportException(ConnixCore::Infrastructure::Transport::
                                           TransportErrorCode::INVALID_ADDRESS,
                                       "Invalid IPv6 address: " +
                                           endpoint.getAddress());
            }
            std::memcpy(&result.addr.sa, &in6, sizeof(in6));
            result.len = sizeof(struct sockaddr_in6);
            break;
        }
        case ConnixCore::Infrastructure::Transport::SocketDomain::UNIX:
        {
            struct sockaddr_un un {
            };
            un.sun_family = AF_UNIX;
            if (endpoint.getAddress().size() >= sizeof(un.sun_path))
            {
                throw ConnixCore::Infrastructure::Transport::
                    TransportException(ConnixCore::Infrastructure::Transport::
                                           TransportErrorCode::INVALID_ADDRESS,
                                       "Unix domain path exceeds max length");
            }
            std::strncpy(un.sun_path, endpoint.getAddress().c_str(),
                         sizeof(un.sun_path) - 1);
            std::memcpy(&result.addr.sa, &un, sizeof(un));
            result.len = sizeof(struct sockaddr_un);
            break;
        }
    }

    return result;
}

ConnixCore::Infrastructure::Transport::TransportEndpoint
fromNativeAddress(const struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
    {
        struct sockaddr_in in {
        };
        std::memcpy(&in, sa, sizeof(in));
        std::array<char, INET_ADDRSTRLEN> ipStr{};
        ::inet_ntop(AF_INET, &in.sin_addr, ipStr.data(), ipStr.size());
        return { std::string(ipStr.data()), ntohs(in.sin_port) };
    }
    if (sa->sa_family == AF_INET6)
    {
        struct sockaddr_in6 in6 {
        };
        std::memcpy(&in6, sa, sizeof(in6));
        std::array<char, INET6_ADDRSTRLEN> ipStr{};
        ::inet_ntop(AF_INET6, &in6.sin6_addr, ipStr.data(), ipStr.size());
        return { std::string(ipStr.data()), ntohs(in6.sin6_port) };
    }
    if (sa->sa_family == AF_UNIX)
    {
        struct sockaddr_un un {
        };
        std::memcpy(&un, sa, sizeof(un));
        return { std::string(un.sun_path), 0 };
    }
    return {};
}

} // namespace

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

namespace Detail {

namespace {

int defaultClose(int fd)
{
    return ::close(fd);
}

int defaultFcntl(int fd, int cmd, int arg)
{
    return ::fcntl(fd, cmd, arg);
}

const SyscallHooks s_defaultHooks = {
    ::socket,     ::bind,       ::listen,     ::accept4,    ::connect,
    ::send,       ::recv,       ::sendto,     ::recvfrom,   ::poll,
    defaultClose, ::setsockopt, ::getsockopt, defaultFcntl, ::getsockname
};

SyscallHooks s_activeHooks = s_defaultHooks;

} // namespace

SyscallHooks& getSyscallHooks()
{
    return s_activeHooks;
}

void resetSyscallHooks()
{
    s_activeHooks = s_defaultHooks;
}

} // namespace Detail

Socket::Socket()
    : m_handle(-1)
    , m_domain(SocketDomain::IPV4)
    , m_type(SocketType::STREAM)
    , m_protocol(SocketProtocol::DEFAULT)
    , m_localEndpoint()
    , m_remoteEndpoint()
    , m_isOpen(false)
    , m_udsPath()
    , m_isUdsBound(false)
{
}

Socket::Socket(std::int32_t handle, SocketDomain domain, SocketType type,
               SocketProtocol protocol, const TransportEndpoint& localEndpoint,
               const TransportEndpoint& remoteEndpoint)
    : m_handle(handle)
    , m_domain(domain)
    , m_type(type)
    , m_protocol(protocol)
    , m_localEndpoint(localEndpoint)
    , m_remoteEndpoint(remoteEndpoint)
    , m_isOpen(handle >= 0)
    , m_udsPath()
    , m_isUdsBound(false)
{
}

Socket::~Socket()
{
    closeInternal();
}

Socket::Socket(Socket&& other) noexcept
    : m_handle(other.m_handle)
    , m_domain(other.m_domain)
    , m_type(other.m_type)
    , m_protocol(other.m_protocol)
    , m_localEndpoint(other.m_localEndpoint)
    , m_remoteEndpoint(other.m_remoteEndpoint)
    , m_isOpen(other.m_isOpen)
    , m_udsPath(std::move(other.m_udsPath))
    , m_isUdsBound(other.m_isUdsBound)
{
    other.m_handle = -1;
    other.m_isOpen = false;
    other.m_isUdsBound = false;
    other.m_udsPath.clear();
}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this != &other)
    {
        closeInternal();
        m_handle = other.m_handle;
        m_domain = other.m_domain;
        m_type = other.m_type;
        m_protocol = other.m_protocol;
        m_localEndpoint = other.m_localEndpoint;
        m_remoteEndpoint = other.m_remoteEndpoint;
        m_isOpen = other.m_isOpen;
        m_udsPath = std::move(other.m_udsPath);
        m_isUdsBound = other.m_isUdsBound;

        other.m_handle = -1;
        other.m_isOpen = false;
        other.m_isUdsBound = false;
        other.m_udsPath.clear();
    }
    return *this;
}

void Socket::open(SocketDomain domain, SocketType type,
                  SocketProtocol protocol)
{
    if (m_isOpen && m_handle >= 0)
    {
        closeInternal();
    }

    int sysDomain = AF_INET;
    if (domain == SocketDomain::IPV6)
    {
        sysDomain = AF_INET6;
    }
    else if (domain == SocketDomain::UNIX)
    {
        sysDomain = AF_UNIX;
    }

    const int sysType =
        (type == SocketType::DATAGRAM) ? SOCK_DGRAM : SOCK_STREAM;

    int sysProtocol = 0;
    if (protocol == SocketProtocol::TCP)
    {
        sysProtocol = IPPROTO_TCP;
    }
    else if (protocol == SocketProtocol::UDP)
    {
        sysProtocol = IPPROTO_UDP;
    }

    const int fd = Detail::getSyscallHooks().socketFn(
        sysDomain, sysType | SOCK_CLOEXEC, sysProtocol);
    if (fd < 0)
    {
        throw TransportException(TransportErrorCode::SOCKET_CREATION_FAILED,
                                 "Failed to create socket: " +
                                     std::string(std::strerror(errno)));
    }

    const int flags = Detail::getSyscallHooks().fcntlFn(fd, F_GETFL, 0);
    if (flags >= 0)
    {
        Detail::getSyscallHooks().fcntlFn(fd, F_SETFL, flags | O_NONBLOCK);
    }

    m_handle = fd;
    m_domain = domain;
    m_type = type;
    m_protocol = protocol;
    m_isOpen = true;
    m_isUdsBound = false;
    m_udsPath.clear();
    m_localEndpoint = TransportEndpoint();
    m_remoteEndpoint = TransportEndpoint();
}

void Socket::bind(const TransportEndpoint& endpoint)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::BIND_FAILED,
                                 "Cannot bind: socket is not open");
    }

    const auto addr = toNativeAddress(endpoint, m_domain);

    if (m_domain == SocketDomain::UNIX)
    {
        ::unlink(endpoint.getAddress().c_str());
    }

    if (Detail::getSyscallHooks().bindFn(m_handle, &addr.addr.sa, addr.len) <
        0)
    {
        throw TransportException(TransportErrorCode::BIND_FAILED,
                                 "Failed to bind socket: " +
                                     std::string(std::strerror(errno)));
    }

    if (m_domain == SocketDomain::UNIX)
    {
        m_localEndpoint = endpoint;
        m_udsPath = endpoint.getAddress();
        m_isUdsBound = true;
    }
    else
    {
        updateLocalEndpoint();
    }
}

void Socket::listen(std::int32_t backlog)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::LISTEN_FAILED,
                                 "Cannot listen: socket is not open");
    }

    if (Detail::getSyscallHooks().listenFn(m_handle, backlog) < 0)
    {
        throw TransportException(TransportErrorCode::LISTEN_FAILED,
                                 "Failed to listen: " +
                                     std::string(std::strerror(errno)));
    }
}

std::unique_ptr<ISocket> Socket::accept(std::uint32_t timeoutMs)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::ACCEPT_FAILED,
                                 "Cannot accept: socket is not open");
    }

    pollWait(POLLIN, timeoutMs, TransportErrorCode::ACCEPT_FAILED, "Accept");

    SocketAddressBuffer clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    const int clientFd = Detail::getSyscallHooks().accept4Fn(
        m_handle, &clientAddr.sa, &addrLen, SOCK_CLOEXEC | SOCK_NONBLOCK);
    if (clientFd < 0)
    {
        throw TransportException(TransportErrorCode::ACCEPT_FAILED,
                                 "Failed to accept incoming connection: " +
                                     std::string(std::strerror(errno)));
    }

    const auto clientRemoteEndpoint = fromNativeAddress(&clientAddr.sa);

    TransportEndpoint clientLocalEndpoint = m_localEndpoint;
    SocketAddressBuffer localAddr{};
    socklen_t localLen = sizeof(localAddr);
    if (Detail::getSyscallHooks().getsocknameFn(clientFd, &localAddr.sa,
                                                &localLen) == 0)
    {
        clientLocalEndpoint = fromNativeAddress(&localAddr.sa);
    }

    return std::make_unique<Socket>(clientFd, m_domain, m_type, m_protocol,
                                    clientLocalEndpoint, clientRemoteEndpoint);
}

void Socket::connect(const TransportEndpoint& endpoint,
                     std::uint32_t timeoutMs)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::CONNECT_FAILED,
                                 "Cannot connect: socket is not open");
    }

    const auto addr = toNativeAddress(endpoint, m_domain);

    const int ret =
        Detail::getSyscallHooks().connectFn(m_handle, &addr.addr.sa, addr.len);

    if (ret < 0)
    {
        if (errno == EINPROGRESS)
        {
            pollWait(POLLOUT, timeoutMs, TransportErrorCode::CONNECT_FAILED,
                     "Connect");
            checkSocketError(TransportErrorCode::CONNECT_FAILED, "Connect");
        }
        else
        {
            throw TransportException(TransportErrorCode::CONNECT_FAILED,
                                     "Connect failed: " +
                                         std::string(std::strerror(errno)));
        }
    }

    m_remoteEndpoint = endpoint;
    if (m_domain != SocketDomain::UNIX)
    {
        updateLocalEndpoint();
    }
}

std::size_t Socket::send(const std::vector<std::uint8_t>& data,
                         std::uint32_t timeoutMs)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::SEND_FAILED,
                                 "Cannot send: socket is not open");
    }

    if (data.empty())
    {
        return 0;
    }

    const auto deadline = std::chrono::steady_clock::now() +
                          std::chrono::milliseconds(timeoutMs);
    std::size_t totalSent = 0;

    while (totalSent < data.size())
    {
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline)
        {
            throw TransportException(TransportErrorCode::OPERATION_TIMEOUT,
                                     "Send timed out");
        }
        const auto remaining =
            std::chrono::duration_cast<std::chrono::milliseconds>(deadline -
                                                                  now)
                .count();

        struct pollfd pfd {
        };
        pfd.fd = m_handle;
        pfd.events = POLLOUT;

        const int ret = Detail::getSyscallHooks().pollFn(
            &pfd, 1, static_cast<int>(remaining));
        if (ret == 0)
        {
            throw TransportException(TransportErrorCode::OPERATION_TIMEOUT,
                                     "Send timed out");
        }
        if (ret < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            throw TransportException(TransportErrorCode::SEND_FAILED,
                                     "Poll failed during send: " +
                                         std::string(std::strerror(errno)));
        }
        if ((pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) != 0 &&
            (pfd.revents & POLLOUT) == 0)
        {
            throw TransportException(TransportErrorCode::CONNECTION_CLOSED,
                                     "Connection closed or socket error");
        }

        const void* sendPtr =
            static_cast<const void*>(data.data() + totalSent);
        const std::size_t toSend = data.size() - totalSent;
        const ssize_t bytes = Detail::getSyscallHooks().sendFn(
            m_handle, sendPtr, toSend, MSG_NOSIGNAL);

        if (bytes < 0)
        {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            if (errno == EPIPE || errno == ECONNRESET)
            {
                throw TransportException(
                    TransportErrorCode::CONNECTION_CLOSED,
                    "Connection closed by peer during send");
            }
            throw TransportException(TransportErrorCode::SEND_FAILED,
                                     "Send failed: " +
                                         std::string(std::strerror(errno)));
        }
        totalSent += static_cast<std::size_t>(bytes);
    }
    return totalSent;
}

std::vector<std::uint8_t> Socket::receive(std::size_t maxBytes,
                                          std::uint32_t timeoutMs)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::RECEIVE_FAILED,
                                 "Cannot receive: socket is not open");
    }

    if (maxBytes == 0)
    {
        return {};
    }

    pollWait(POLLIN, timeoutMs, TransportErrorCode::RECEIVE_FAILED, "Receive");

    std::vector<std::uint8_t> buffer(maxBytes);
    void* recvPtr = static_cast<void*>(buffer.data());
    const ssize_t bytes =
        Detail::getSyscallHooks().recvFn(m_handle, recvPtr, maxBytes, 0);

    if (bytes == 0)
    {
        throw TransportException(TransportErrorCode::CONNECTION_CLOSED,
                                 "Connection closed by peer");
    }
    if (bytes < 0)
    {
        if (errno == ECONNRESET || errno == ENOTCONN)
        {
            throw TransportException(TransportErrorCode::CONNECTION_CLOSED,
                                     "Connection reset by peer");
        }
        throw TransportException(TransportErrorCode::RECEIVE_FAILED,
                                 "Receive failed: " +
                                     std::string(std::strerror(errno)));
    }

    buffer.resize(static_cast<std::size_t>(bytes));
    return buffer;
}

std::size_t Socket::sendTo(const std::vector<std::uint8_t>& data,
                           const TransportEndpoint& destination,
                           std::uint32_t timeoutMs)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::SEND_FAILED,
                                 "Cannot sendTo: socket is not open");
    }

    const auto addr = toNativeAddress(destination, m_domain);
    pollWait(POLLOUT, timeoutMs, TransportErrorCode::SEND_FAILED, "SendTo");

    const void* sendPtr = static_cast<const void*>(data.data());
    const ssize_t bytes = Detail::getSyscallHooks().sendtoFn(
        m_handle, sendPtr, data.size(), MSG_NOSIGNAL, &addr.addr.sa, addr.len);

    if (bytes < 0)
    {
        throw TransportException(TransportErrorCode::SEND_FAILED,
                                 "SendTo failed: " +
                                     std::string(std::strerror(errno)));
    }
    return static_cast<std::size_t>(bytes);
}

std::vector<std::uint8_t> Socket::receiveFrom(std::size_t maxBytes,
                                              TransportEndpoint& source,
                                              std::uint32_t timeoutMs)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::RECEIVE_FAILED,
                                 "Cannot receiveFrom: socket is not open");
    }

    if (maxBytes == 0)
    {
        return {};
    }

    pollWait(POLLIN, timeoutMs, TransportErrorCode::RECEIVE_FAILED,
             "ReceiveFrom");

    std::vector<std::uint8_t> buffer(maxBytes);
    SocketAddressBuffer srcAddr{};
    socklen_t srcLen = sizeof(srcAddr);

    void* recvPtr = static_cast<void*>(buffer.data());
    const ssize_t bytes = Detail::getSyscallHooks().recvfromFn(
        m_handle, recvPtr, maxBytes, 0, &srcAddr.sa, &srcLen);

    if (bytes < 0)
    {
        throw TransportException(TransportErrorCode::RECEIVE_FAILED,
                                 "ReceiveFrom failed: " +
                                     std::string(std::strerror(errno)));
    }

    source = fromNativeAddress(&srcAddr.sa);
    buffer.resize(static_cast<std::size_t>(bytes));
    return buffer;
}

void Socket::setOption(SocketOption option, bool enable)
{
    if (!m_isOpen || m_handle < 0)
    {
        throw TransportException(TransportErrorCode::SOCKET_OPTION_FAILED,
                                 "Cannot set option on closed socket");
    }

    switch (option)
    {
        case SocketOption::REUSE_ADDRESS:
        {
            const int optVal = enable ? 1 : 0;
            if (Detail::getSyscallHooks().setsockoptFn(m_handle, SOL_SOCKET,
                                                       SO_REUSEADDR, &optVal,
                                                       sizeof(optVal)) < 0)
            {
                throw TransportException(
                    TransportErrorCode::SOCKET_OPTION_FAILED,
                    "Failed to set SO_REUSEADDR: " +
                        std::string(std::strerror(errno)));
            }
            break;
        }
        case SocketOption::NON_BLOCKING:
        {
            int flags =
                Detail::getSyscallHooks().fcntlFn(m_handle, F_GETFL, 0);
            if (flags < 0)
            {
                throw TransportException(
                    TransportErrorCode::SOCKET_OPTION_FAILED,
                    "Failed to get socket flags: " +
                        std::string(std::strerror(errno)));
            }
            if (enable)
            {
                flags |= O_NONBLOCK;
            }
            else
            {
                flags &= ~O_NONBLOCK;
            }
            if (Detail::getSyscallHooks().fcntlFn(m_handle, F_SETFL, flags) <
                0)
            {
                throw TransportException(
                    TransportErrorCode::SOCKET_OPTION_FAILED,
                    "Failed to set O_NONBLOCK flag: " +
                        std::string(std::strerror(errno)));
            }
            break;
        }
        case SocketOption::RECEIVE_TIMEOUT:
        {
            struct timeval tv {
            };
            tv.tv_sec = enable ? 10 : 0;
            tv.tv_usec = 0;
            if (Detail::getSyscallHooks().setsockoptFn(
                    m_handle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
            {
                throw TransportException(
                    TransportErrorCode::SOCKET_OPTION_FAILED,
                    "Failed to set SO_RCVTIMEO: " +
                        std::string(std::strerror(errno)));
            }
            break;
        }
        case SocketOption::SEND_TIMEOUT:
        {
            struct timeval tv {
            };
            tv.tv_sec = enable ? 5 : 0;
            tv.tv_usec = 0;
            if (Detail::getSyscallHooks().setsockoptFn(
                    m_handle, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0)
            {
                throw TransportException(
                    TransportErrorCode::SOCKET_OPTION_FAILED,
                    "Failed to set SO_SNDTIMEO: " +
                        std::string(std::strerror(errno)));
            }
            break;
        }
    }
}

void Socket::close()
{
    closeInternal();
}

void Socket::closeInternal()
{
    if (m_handle >= 0)
    {
        Detail::getSyscallHooks().closeFn(m_handle);
        m_handle = -1;
    }
    if (m_isUdsBound && !m_udsPath.empty())
    {
        ::unlink(m_udsPath.c_str());
        m_udsPath.clear();
        m_isUdsBound = false;
    }
    m_isOpen = false;
    m_localEndpoint = TransportEndpoint();
    m_remoteEndpoint = TransportEndpoint();
}

bool Socket::isOpen() const
{
    return m_isOpen && m_handle >= 0;
}

std::int32_t Socket::getNativeHandle() const
{
    return m_handle;
}

const TransportEndpoint& Socket::getLocalEndpoint() const
{
    return m_localEndpoint;
}

const TransportEndpoint& Socket::getRemoteEndpoint() const
{
    return m_remoteEndpoint;
}

void Socket::pollWait(short events, std::uint32_t timeoutMs,
                      TransportErrorCode errCode, const std::string& opName)
{
    const auto deadline = std::chrono::steady_clock::now() +
                          std::chrono::milliseconds(timeoutMs);

    while (true)
    {
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline)
        {
            throw TransportException(TransportErrorCode::OPERATION_TIMEOUT,
                                     opName + " timed out");
        }
        const auto remaining =
            std::chrono::duration_cast<std::chrono::milliseconds>(deadline -
                                                                  now)
                .count();

        struct pollfd pfd {
        };
        pfd.fd = m_handle;
        pfd.events = events;

        const int ret = Detail::getSyscallHooks().pollFn(
            &pfd, 1, static_cast<int>(remaining));
        if (ret > 0)
        {
            if ((pfd.revents & (POLLERR | POLLNVAL)) != 0)
            {
                checkSocketError(errCode, opName);
            }
            if ((pfd.revents & POLLHUP) != 0 && (pfd.revents & events) == 0)
            {
                throw TransportException(
                    TransportErrorCode::CONNECTION_CLOSED,
                    opName + " failed: connection closed by peer");
            }
            return;
        }
        if (ret == 0)
        {
            throw TransportException(TransportErrorCode::OPERATION_TIMEOUT,
                                     opName + " timed out");
        }
        if (errno == EINTR)
        {
            continue;
        }
        throw TransportException(errCode,
                                 "Poll failed during " + opName + ": " +
                                     std::string(std::strerror(errno)));
    }
}

void Socket::checkSocketError(TransportErrorCode errCode,
                              const std::string& opName)
{
    int error = 0;
    socklen_t len = sizeof(error);
    if (Detail::getSyscallHooks().getsockoptFn(m_handle, SOL_SOCKET, SO_ERROR,
                                               &error, &len) < 0 ||
        error != 0)
    {
        const int err = (error != 0) ? error : EIO;
        throw TransportException(
            errCode, opName + " failed: " + std::string(std::strerror(err)));
    }
}

void Socket::updateLocalEndpoint()
{
    SocketAddressBuffer addr{};
    socklen_t len = sizeof(addr);
    if (Detail::getSyscallHooks().getsocknameFn(m_handle, &addr.sa, &len) == 0)
    {
        m_localEndpoint = fromNativeAddress(&addr.sa);
    }
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
