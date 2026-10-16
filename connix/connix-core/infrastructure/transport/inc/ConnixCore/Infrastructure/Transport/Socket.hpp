#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

namespace Detail {

/**
 * @brief Function pointer table for POSIX socket system calls.
 *
 * Allows deterministic fault injection in unit tests while defaulting to
 * standard operating system calls in production.
 */
struct SyscallHooks {
    int (*socketFn)(int, int, int);
    int (*bindFn)(int, const struct sockaddr*, socklen_t);
    int (*listenFn)(int, int);
    int (*accept4Fn)(int, struct sockaddr*, socklen_t*, int);
    int (*connectFn)(int, const struct sockaddr*, socklen_t);
    ssize_t (*sendFn)(int, const void*, size_t, int);
    ssize_t (*recvFn)(int, void*, size_t, int);
    ssize_t (*sendtoFn)(int, const void*, size_t, int, const struct sockaddr*,
                        socklen_t);
    ssize_t (*recvfromFn)(int, void*, size_t, int, struct sockaddr*,
                          socklen_t*);
    int (*pollFn)(struct pollfd*, nfds_t, int);
    int (*closeFn)(int);
    int (*setsockoptFn)(int, int, int, const void*, socklen_t);
    int (*getsockoptFn)(int, int, int, void*, socklen_t*);
    int (*fcntlFn)(int, int, int);
    int (*getsocknameFn)(int, struct sockaddr*, socklen_t*);
};

SyscallHooks& getSyscallHooks();
void resetSyscallHooks();

} // namespace Detail

/**
 * @brief Concrete POSIX operating system socket implementation of ISocket.
 *
 * Encapsulates POSIX file descriptor lifecycle and non-blocking I/O operations
 * using poll(2) bounded by explicit operation timeouts.
 */
class Socket : public ISocket
{
public:
    /**
     * @brief Constructs an unallocated Socket instance.
     */
    Socket();

    /**
     * @brief Constructs a Socket wrapping an existing native descriptor.
     * @param handle Native OS file descriptor integer.
     * @param domain Address domain used by descriptor.
     * @param type Communication type used by descriptor.
     * @param protocol Socket protocol used by descriptor.
     * @param localEndpoint Cached local endpoint.
     * @param remoteEndpoint Cached remote peer endpoint.
     */
    Socket(std::int32_t handle, SocketDomain domain, SocketType type,
           SocketProtocol protocol, const TransportEndpoint& localEndpoint,
           const TransportEndpoint& remoteEndpoint);

    /**
     * @brief Destructor closing active socket descriptor and unlinking paths.
     */
    ~Socket() override;

    /**
     * @brief Deleted copy constructor (move-only resource).
     */
    Socket(const Socket&) = delete;

    /**
     * @brief Deleted copy assignment operator (move-only resource).
     */
    Socket& operator=(const Socket&) = delete;

    /**
     * @brief Move constructor transferring file descriptor ownership.
     * @param other Rvalue reference to existing Socket instance.
     */
    Socket(Socket&& other) noexcept;

    /**
     * @brief Move assignment operator transferring file descriptor ownership.
     * @param other Rvalue reference to existing Socket instance.
     * @return Reference to this Socket.
     */
    Socket& operator=(Socket&& other) noexcept;

    void open(SocketDomain domain, SocketType type,
              SocketProtocol protocol) override;

    void bind(const TransportEndpoint& endpoint) override;

    void listen(std::int32_t backlog) override;

    std::unique_ptr<ISocket> accept(std::uint32_t timeoutMs) override;

    void connect(const TransportEndpoint& endpoint,
                 std::uint32_t timeoutMs) override;

    std::size_t send(const std::vector<std::uint8_t>& data,
                     std::uint32_t timeoutMs) override;

    std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                      std::uint32_t timeoutMs) override;

    std::size_t sendTo(const std::vector<std::uint8_t>& data,
                       const TransportEndpoint& destination,
                       std::uint32_t timeoutMs) override;

    std::vector<std::uint8_t> receiveFrom(std::size_t maxBytes,
                                          TransportEndpoint& source,
                                          std::uint32_t timeoutMs) override;

    void setOption(SocketOption option, bool enable) override;

    void close() override;

    bool isOpen() const override;

    std::int32_t getNativeHandle() const override;

    const TransportEndpoint& getLocalEndpoint() const override;

    const TransportEndpoint& getRemoteEndpoint() const override;

private:
    void closeInternal();

    void pollWait(short events, std::uint32_t timeoutMs,
                  TransportErrorCode errCode, const std::string& opName);

    void checkSocketError(TransportErrorCode errCode,
                          const std::string& opName);

    void updateLocalEndpoint();

private:
    std::int32_t m_handle;
    SocketDomain m_domain;
    SocketType m_type;
    SocketProtocol m_protocol;
    TransportEndpoint m_localEndpoint;
    TransportEndpoint m_remoteEndpoint;
    bool m_isOpen;
    std::string m_udsPath;
    bool m_isUdsBound;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
