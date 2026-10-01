#include "gtest/gtest.h"

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <fcntl.h>
#include <memory>
#include <poll.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/Socket.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"

using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

namespace {

struct SyscallHookGuard {
    SyscallHookGuard() = default;

    ~SyscallHookGuard()
    {
        Detail::resetSyscallHooks();
    }

    SyscallHookGuard(const SyscallHookGuard&) = delete;
    SyscallHookGuard& operator=(const SyscallHookGuard&) = delete;
    SyscallHookGuard(SyscallHookGuard&&) = delete;
    SyscallHookGuard& operator=(SyscallHookGuard&&) = delete;
};

} // namespace

TEST(SocketUnitTest, DefaultConstructedIsClosed)
{
    const Socket socket;
    EXPECT_FALSE(socket.isOpen());
    EXPECT_EQ(socket.getNativeHandle(), -1);
    EXPECT_TRUE(socket.getLocalEndpoint().getAddress().empty());
    EXPECT_TRUE(socket.getRemoteEndpoint().getAddress().empty());
}

TEST(SocketUnitTest, OperationsOnClosedSocketThrow)
{
    Socket socket;
    const TransportEndpoint endpoint("127.0.0.1", 8080);
    const std::vector<std::uint8_t> data = { 0x01, 0x02 };

    EXPECT_THROW(socket.bind(endpoint), TransportException);
    EXPECT_THROW(socket.listen(5), TransportException);
    EXPECT_THROW(socket.accept(100), TransportException);
    EXPECT_THROW(socket.connect(endpoint, 100), TransportException);
    EXPECT_THROW(socket.send(data, 100), TransportException);
    EXPECT_THROW(socket.receive(1024, 100), TransportException);
    EXPECT_THROW(socket.sendTo(data, endpoint, 100), TransportException);
    TransportEndpoint source;
    EXPECT_THROW(socket.receiveFrom(1024, source, 100), TransportException);
    EXPECT_THROW(socket.setOption(SocketOption::REUSE_ADDRESS, true),
                 TransportException);
}

TEST(SocketUnitTest, OpenAndClose)
{
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    EXPECT_TRUE(socket.isOpen());
    EXPECT_GE(socket.getNativeHandle(), 0);

    socket.close();
    EXPECT_FALSE(socket.isOpen());
    EXPECT_EQ(socket.getNativeHandle(), -1);

    // Double close is safe and idempotent
    socket.close();
    EXPECT_FALSE(socket.isOpen());
}

TEST(SocketUnitTest, OpenExistingSocketReopens)
{
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    const auto handle1 = socket.getNativeHandle();

    socket.open(SocketDomain::IPV4, SocketType::DATAGRAM, SocketProtocol::UDP);
    const auto handle2 = socket.getNativeHandle();
    EXPECT_TRUE(socket.isOpen());
    EXPECT_NE(handle1, -1);
    EXPECT_NE(handle2, -1);
}

TEST(SocketUnitTest, SetOptionsOnOpenSocket)
{
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);

    EXPECT_NO_THROW(socket.setOption(SocketOption::REUSE_ADDRESS, true));
    EXPECT_NO_THROW(socket.setOption(SocketOption::NON_BLOCKING, true));
    EXPECT_NO_THROW(socket.setOption(SocketOption::NON_BLOCKING, false));
    EXPECT_NO_THROW(socket.setOption(SocketOption::RECEIVE_TIMEOUT, true));
    EXPECT_NO_THROW(socket.setOption(SocketOption::RECEIVE_TIMEOUT, false));
    EXPECT_NO_THROW(socket.setOption(SocketOption::SEND_TIMEOUT, true));
    EXPECT_NO_THROW(socket.setOption(SocketOption::SEND_TIMEOUT, false));
    socket.close();
}

TEST(SocketUnitTest, TcpLoopbackCommunication)
{
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(5);

    const auto serverEndpoint = server.getLocalEndpoint();
    EXPECT_EQ(serverEndpoint.getAddress(), "127.0.0.1");
    EXPECT_GT(serverEndpoint.getPort(), 0);

    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(serverEndpoint, 2000);
    EXPECT_TRUE(client.isOpen());

    const auto acceptedSocket = server.accept(2000);
    ASSERT_NE(acceptedSocket, nullptr);
    EXPECT_TRUE(acceptedSocket->isOpen());

    const std::vector<std::uint8_t> sendData = { 0xDE, 0xAD, 0xBE, 0xEF };
    const auto bytesSent = client.send(sendData, 2000);
    EXPECT_EQ(bytesSent, sendData.size());

    const auto recvData = acceptedSocket->receive(1024, 2000);
    EXPECT_EQ(recvData, sendData);

    const std::vector<std::uint8_t> replyData = { 0xCA, 0xFE };
    EXPECT_EQ(acceptedSocket->send(replyData, 2000), replyData.size());
    EXPECT_EQ(client.receive(1024, 2000), replyData);

    client.close();
    try
    {
        acceptedSocket->receive(1024, 2000);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::CONNECTION_CLOSED);
    }
}

TEST(SocketUnitTest, Ipv6LoopbackCommunication)
{
    Socket server;
    server.open(SocketDomain::IPV6, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("::1", 0));
    server.listen(5);

    const auto serverEndpoint = server.getLocalEndpoint();
    EXPECT_EQ(serverEndpoint.getAddress(), "::1");
    EXPECT_GT(serverEndpoint.getPort(), 0);

    Socket client;
    client.open(SocketDomain::IPV6, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(serverEndpoint, 2000);

    const auto accepted = server.accept(2000);
    ASSERT_NE(accepted, nullptr);

    const std::vector<std::uint8_t> msg = { 0x11, 0x22, 0x33 };
    EXPECT_EQ(client.send(msg, 2000), msg.size());
    EXPECT_EQ(accepted->receive(1024, 2000), msg);

    // Test sendTo and receiveFrom on IPv6
    Socket udpReceiver;
    udpReceiver.open(SocketDomain::IPV6, SocketType::DATAGRAM,
                     SocketProtocol::UDP);
    udpReceiver.bind(TransportEndpoint("::1", 0));

    Socket udpSender;
    udpSender.open(SocketDomain::IPV6, SocketType::DATAGRAM,
                   SocketProtocol::UDP);
    EXPECT_EQ(udpSender.sendTo(msg, udpReceiver.getLocalEndpoint(), 2000),
              msg.size());

    TransportEndpoint source;
    EXPECT_EQ(udpReceiver.receiveFrom(1024, source, 2000), msg);
    EXPECT_EQ(source.getAddress(), "::1");
}

TEST(SocketUnitTest, SendAndReceiveZeroBytes)
{
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(1);

    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(server.getLocalEndpoint(), 2000);

    EXPECT_EQ(client.send({}, 1000), 0);
    EXPECT_TRUE(client.receive(0, 1000).empty());

    TransportEndpoint source;
    EXPECT_TRUE(client.receiveFrom(0, source, 1000).empty());
}

TEST(SocketUnitTest, AcceptTimeoutThrows)
{
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(1);

    try
    {
        server.accept(50);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::OPERATION_TIMEOUT);
    }
}

TEST(SocketUnitTest, ReceiveTimeoutThrows)
{
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(1);

    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(server.getLocalEndpoint(), 2000);

    const auto accepted = server.accept(2000);
    ASSERT_NE(accepted, nullptr);

    try
    {
        accepted->receive(1024, 50);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::OPERATION_TIMEOUT);
    }
}

TEST(SocketUnitTest, UdpLoopbackSendToAndReceiveFrom)
{
    Socket receiver;
    receiver.open(SocketDomain::IPV4, SocketType::DATAGRAM,
                  SocketProtocol::UDP);
    receiver.setOption(SocketOption::REUSE_ADDRESS, true);
    receiver.bind(TransportEndpoint("127.0.0.1", 0));

    const auto receiverEndpoint = receiver.getLocalEndpoint();
    EXPECT_EQ(receiverEndpoint.getAddress(), "127.0.0.1");
    EXPECT_GT(receiverEndpoint.getPort(), 0);

    Socket sender;
    sender.open(SocketDomain::IPV4, SocketType::DATAGRAM, SocketProtocol::UDP);

    const std::vector<std::uint8_t> payload = { 0x01, 0x03, 0x05, 0x07 };
    const auto sent = sender.sendTo(payload, receiverEndpoint, 2000);
    EXPECT_EQ(sent, payload.size());

    TransportEndpoint source;
    const auto received = receiver.receiveFrom(1024, source, 2000);
    EXPECT_EQ(received, payload);
    EXPECT_EQ(source.getAddress(), "127.0.0.1");
    EXPECT_GT(source.getPort(), 0);
}

TEST(SocketUnitTest, UdsStreamCommunicationAndUnlink)
{
    const std::string socketPath =
        "/tmp/connix_test_uds_stream_" + std::to_string(::getpid()) + ".sock";

    Socket server;
    server.open(SocketDomain::UNIX, SocketType::STREAM,
                SocketProtocol::DEFAULT);
    server.bind(TransportEndpoint(socketPath, 0));
    server.listen(5);

    EXPECT_EQ(::access(socketPath.c_str(), F_OK), 0);

    Socket client;
    client.open(SocketDomain::UNIX, SocketType::STREAM,
                SocketProtocol::DEFAULT);
    client.connect(TransportEndpoint(socketPath, 0), 2000);

    const auto accepted = server.accept(2000);
    ASSERT_NE(accepted, nullptr);

    const std::vector<std::uint8_t> msg = { 'h', 'e', 'l', 'l', 'o' };
    EXPECT_EQ(client.send(msg, 2000), msg.size());
    EXPECT_EQ(accepted->receive(1024, 2000), msg);

    server.close();
    EXPECT_NE(::access(socketPath.c_str(), F_OK), 0);
}

TEST(SocketUnitTest, UdsDatagramCommunicationAndUnlink)
{
    const std::string socketPath =
        "/tmp/connix_test_uds_dgram_" + std::to_string(::getpid()) + ".sock";

    Socket receiver;
    receiver.open(SocketDomain::UNIX, SocketType::DATAGRAM,
                  SocketProtocol::DEFAULT);
    receiver.bind(TransportEndpoint(socketPath, 0));

    EXPECT_EQ(::access(socketPath.c_str(), F_OK), 0);

    Socket sender;
    sender.open(SocketDomain::UNIX, SocketType::DATAGRAM,
                SocketProtocol::DEFAULT);

    const std::vector<std::uint8_t> payload = { 0x42, 0x43 };
    EXPECT_EQ(sender.sendTo(payload, TransportEndpoint(socketPath, 0), 2000),
              payload.size());

    TransportEndpoint source;
    const auto received = receiver.receiveFrom(1024, source, 2000);
    EXPECT_EQ(received, payload);

    receiver.close();
    EXPECT_NE(::access(socketPath.c_str(), F_OK), 0);
}

TEST(SocketUnitTest, InvalidAddressesThrow)
{
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);

    EXPECT_THROW(socket.bind(TransportEndpoint("999.999.999.999", 8080)),
                 TransportException);
    EXPECT_THROW(
        socket.connect(TransportEndpoint("999.999.999.999", 8080), 100),
        TransportException);
    EXPECT_THROW(socket.sendTo({ 1 }, TransportEndpoint("", 8080), 100),
                 TransportException);

    Socket ipv6Socket;
    ipv6Socket.open(SocketDomain::IPV6, SocketType::STREAM,
                    SocketProtocol::TCP);
    EXPECT_THROW(ipv6Socket.bind(TransportEndpoint("invalid_ipv6_addr", 8080)),
                 TransportException);

    Socket udsSocket;
    udsSocket.open(SocketDomain::UNIX, SocketType::STREAM,
                   SocketProtocol::DEFAULT);
    EXPECT_THROW(udsSocket.bind(TransportEndpoint("", 0)), TransportException);

    const std::string longPath(200, 'x');
    EXPECT_THROW(udsSocket.bind(TransportEndpoint(longPath, 0)),
                 TransportException);
}

TEST(SocketUnitTest, MoveConstructorAndAssignment)
{
    Socket socket1;
    socket1.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    const auto handle = socket1.getNativeHandle();
    EXPECT_GE(handle, 0);

    Socket socket2(std::move(socket1));
    EXPECT_TRUE(socket2.isOpen());
    EXPECT_EQ(socket2.getNativeHandle(), handle);

    Socket socket3;
    socket3 = std::move(socket2);
    EXPECT_TRUE(socket3.isOpen());
    EXPECT_EQ(socket3.getNativeHandle(), handle);
}

TEST(SocketUnitTest, SyscallHooksSocketCreationFailure)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().socketFn = [](int, int, int) {
        return -1;
    };
    Socket socket;
    try
    {
        socket.open(SocketDomain::IPV4, SocketType::STREAM,
                    SocketProtocol::TCP);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  TransportErrorCode::SOCKET_CREATION_FAILED);
    }
}

TEST(SocketUnitTest, SyscallHooksBindFailure)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().bindFn = [](int, const struct sockaddr*,
                                          socklen_t) {
        return -1;
    };
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    EXPECT_THROW(socket.bind(TransportEndpoint("127.0.0.1", 0)),
                 TransportException);
}

TEST(SocketUnitTest, SyscallHooksListenFailure)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().listenFn = [](int, int) {
        return -1;
    };
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    EXPECT_THROW(socket.listen(5), TransportException);
}

TEST(SocketUnitTest, SyscallHooksAcceptFailure)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().accept4Fn = [](int, struct sockaddr*, socklen_t*,
                                             int) {
        return -1;
    };
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(1);

    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(server.getLocalEndpoint(), 2000);

    EXPECT_THROW(server.accept(2000), TransportException);
}

TEST(SocketUnitTest, SyscallHooksConnectFailure)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().connectFn = [](int, const struct sockaddr*,
                                             socklen_t) {
        errno = ECONNREFUSED;
        return -1;
    };
    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    EXPECT_THROW(client.connect(TransportEndpoint("127.0.0.1", 12345), 100),
                 TransportException);
}

TEST(SocketUnitTest, SyscallHooksPollRetriesOnEintrAndFailsOnEio)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t,
                                          int) -> int {
        static int counter = 0;
        if (counter++ == 0)
        {
            errno = EINTR;
            return -1;
        }
        fds->revents = POLLIN;
        return 1;
    };

    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    Detail::getSyscallHooks().accept4Fn = [](int, struct sockaddr*, socklen_t*,
                                             int) {
        return -1;
    };
    EXPECT_THROW(socket.accept(1000), TransportException);

    Detail::getSyscallHooks().pollFn = [](struct pollfd*, nfds_t, int) {
        errno = EIO;
        return -1;
    };
    EXPECT_THROW(socket.accept(1000), TransportException);
}

TEST(SocketUnitTest, SyscallHooksPollHupWithoutEventsThrowsConnectionClosed)
{
    const SyscallHookGuard guard;
    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t, int) {
        fds->revents = POLLHUP;
        return 1;
    };

    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    try
    {
        socket.receive(1024, 1000);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::CONNECTION_CLOSED);
    }
}

TEST(SocketUnitTest, SyscallHooksSendEpipeAndEagain)
{
    const SyscallHookGuard guard;
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(1);

    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(server.getLocalEndpoint(), 2000);

    Detail::getSyscallHooks().sendFn = [](int, const void*, size_t,
                                          int) -> ssize_t {
        errno = EPIPE;
        return -1;
    };
    try
    {
        client.send({ 1, 2 }, 1000);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::CONNECTION_CLOSED);
    }

    Detail::getSyscallHooks().sendFn = [](int, const void*, size_t,
                                          int) -> ssize_t {
        errno = EINVAL;
        return -1;
    };
    try
    {
        client.send({ 1, 2 }, 1000);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::SEND_FAILED);
    }

    Detail::getSyscallHooks().sendFn = [](int, const void*, size_t len,
                                          int) -> ssize_t {
        static int counter = 0;
        if (counter++ == 0)
        {
            errno = EAGAIN;
            return -1;
        }
        return static_cast<ssize_t>(len);
    };
    EXPECT_EQ(client.send({ 1, 2 }, 1000), 2);
}

TEST(SocketUnitTest, SyscallHooksReceiveErrors)
{
    const SyscallHookGuard guard;
    Socket server;
    server.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    server.setOption(SocketOption::REUSE_ADDRESS, true);
    server.bind(TransportEndpoint("127.0.0.1", 0));
    server.listen(1);

    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    client.connect(server.getLocalEndpoint(), 2000);
    const auto accepted = server.accept(2000);

    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t, int) {
        fds->revents = POLLIN;
        return 1;
    };

    Detail::getSyscallHooks().recvFn = [](int, void*, size_t, int) -> ssize_t {
        errno = ECONNRESET;
        return -1;
    };
    try
    {
        accepted->receive(1024, 1000);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::CONNECTION_CLOSED);
    }

    Detail::getSyscallHooks().recvFn = [](int, void*, size_t, int) -> ssize_t {
        errno = EINVAL;
        return -1;
    };
    try
    {
        accepted->receive(1024, 1000);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::RECEIVE_FAILED);
    }
}

TEST(SocketUnitTest, SyscallHooksSendToAndReceiveFromErrors)
{
    const SyscallHookGuard guard;
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::DATAGRAM, SocketProtocol::UDP);

    Detail::getSyscallHooks().sendtoFn = [](int, const void*, size_t, int,
                                            const struct sockaddr*,
                                            socklen_t) -> ssize_t {
        return -1;
    };
    EXPECT_THROW(
        socket.sendTo({ 1 }, TransportEndpoint("127.0.0.1", 1234), 1000),
        TransportException);

    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t, int) {
        fds->revents = POLLIN;
        return 1;
    };
    Detail::getSyscallHooks().recvfromFn =
        [](int, void*, size_t, int, struct sockaddr*, socklen_t*) -> ssize_t {
        return -1;
    };
    TransportEndpoint source;
    EXPECT_THROW(socket.receiveFrom(1024, source, 1000), TransportException);
}

TEST(SocketUnitTest, SyscallHooksOptionAndFcntlFailures)
{
    const SyscallHookGuard guard;
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);

    Detail::getSyscallHooks().setsockoptFn = [](int, int, int, const void*,
                                                socklen_t) {
        return -1;
    };
    EXPECT_THROW(socket.setOption(SocketOption::REUSE_ADDRESS, true),
                 TransportException);
    EXPECT_THROW(socket.setOption(SocketOption::RECEIVE_TIMEOUT, true),
                 TransportException);
    EXPECT_THROW(socket.setOption(SocketOption::SEND_TIMEOUT, true),
                 TransportException);

    Detail::resetSyscallHooks();
    Detail::getSyscallHooks().fcntlFn = [](int, int, int) {
        return -1;
    };
    EXPECT_THROW(socket.setOption(SocketOption::NON_BLOCKING, true),
                 TransportException);

    Detail::resetSyscallHooks();
    Detail::getSyscallHooks().fcntlFn = [](int, int cmd, int) {
        if (cmd == F_SETFL)
        {
            return -1;
        }
        return 0;
    };
    EXPECT_THROW(socket.setOption(SocketOption::NON_BLOCKING, false),
                 TransportException);
}

TEST(SocketUnitTest, SyscallHooksSendPollTimeoutAndErrors)
{
    const SyscallHookGuard guard;
    Socket client;
    client.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);

    EXPECT_THROW(client.send({ 1 }, 0), TransportException);

    Detail::getSyscallHooks().pollFn = [](struct pollfd*, nfds_t, int) {
        return 0;
    };
    EXPECT_THROW(client.send({ 1 }, 1000), TransportException);

    Detail::getSyscallHooks().pollFn = [](struct pollfd*, nfds_t, int) {
        errno = EIO;
        return -1;
    };
    EXPECT_THROW(client.send({ 1 }, 1000), TransportException);

    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t, int) {
        fds->revents = POLLERR;
        return 1;
    };
    EXPECT_THROW(client.send({ 1 }, 1000), TransportException);

    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t, int) {
        fds->revents = POLLOUT;
        return 1;
    };
    Detail::getSyscallHooks().sendFn = [](int, const void*, size_t,
                                          int) -> ssize_t {
        errno = ECONNRESET;
        return -1;
    };
    EXPECT_THROW(client.send({ 1 }, 1000), TransportException);

    Detail::getSyscallHooks().sendFn = [](int, const void*, size_t len,
                                          int) -> ssize_t {
        static int counter = 0;
        if (counter++ == 0)
        {
            errno = EINTR;
            return -1;
        }
        return static_cast<ssize_t>(len);
    };
    EXPECT_EQ(client.send({ 1 }, 1000), 1);
}

TEST(SocketUnitTest, PollWaitTimeoutZeroThrows)
{
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    EXPECT_THROW(socket.receive(1024, 0), TransportException);
}

TEST(SocketUnitTest, SyscallHooksCheckSocketError)
{
    const SyscallHookGuard guard;
    Socket socket;
    socket.open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);

    Detail::getSyscallHooks().pollFn = [](struct pollfd* fds, nfds_t, int) {
        fds->revents = POLLERR;
        return 1;
    };
    Detail::getSyscallHooks().getsockoptFn = [](int, int, int, void* val,
                                                socklen_t*) {
        *static_cast<int*>(val) = ECONNREFUSED;
        return 0;
    };

    EXPECT_THROW(socket.receive(1024, 1000), TransportException);
}

TEST(SocketUnitTest, PolymorphicDeletionViaInterfacePointer)
{
    std::unique_ptr<ISocket> socket = std::make_unique<Socket>();
    EXPECT_FALSE(socket->isOpen());
    socket.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
