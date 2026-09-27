#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/MockISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TcpTransport.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

class TcpTransportTest : public Test
{
protected:
    void SetUp() override
    {
        auto mock = std::make_unique<StrictMock<MockISocket>>();
        m_mockSocket = mock.get();
        m_transport = std::make_unique<TcpTransport>(std::move(mock));
    }

    void TestBody() override
    {
    }

    StrictMock<MockISocket>* m_mockSocket{ nullptr };
    std::unique_ptr<TcpTransport> m_transport;
};

TEST_F(TcpTransportTest, ConstructorThrowsOnNullSocket)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW(
        {
            try
            {
                const TcpTransport transport(nullptr);
            }
            catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::SOCKET_CREATION_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_THROW(
        {
            try
            {
                const TcpTransport transport(nullptr,
                                             TransportState::CONNECTED,
                                             TransportEndpoint(),
                                             TransportEndpoint());
            }
            catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::SOCKET_CREATION_FAILED);
                throw;
            }
        },
        TransportException);
}

TEST_F(TcpTransportTest, InitialState)
{
    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_FALSE(m_transport->isOpen());
    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);
    EXPECT_EQ(m_transport->getProtocol(), TransportProtocol::TCP);
    EXPECT_EQ(m_transport->getLocalEndpoint(), TransportEndpoint());
    EXPECT_EQ(m_transport->getRemoteEndpoint(), TransportEndpoint());

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, BindIpv4Success)
{
    const TransportEndpoint ep("127.0.0.1", 8080);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV4, SocketType::STREAM,
                                    SocketProtocol::TCP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);
    EXPECT_EQ(m_transport->getLocalEndpoint(), ep);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, BindIpv6Success)
{
    const TransportEndpoint ep("::1", 9090);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV6, SocketType::STREAM,
                                    SocketProtocol::TCP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);
    EXPECT_EQ(m_transport->getLocalEndpoint(), ep);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, BindWhenSocketAlreadyOpen)
{
    const TransportEndpoint ep("192.168.1.10", 4000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(0);
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, BindInInvalidStateThrows)
{
    const TransportEndpoint ep("127.0.0.1", 8080);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(_, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, bind(_)).Times(1);
    m_transport->bind(ep);

    EXPECT_CALL(*m_mockSocket, listen(5)).Times(1);
    m_transport->listen(5);

    EXPECT_THROW(
        {
            try
            {
                m_transport->bind(ep);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::BIND_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ListenSuccess)
{
    const TransportEndpoint ep("0.0.0.0", 5000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(_, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, bind(_)).Times(1);
    m_transport->bind(ep);

    EXPECT_CALL(*m_mockSocket, listen(128)).Times(1);
    m_transport->listen(128);

    EXPECT_EQ(m_transport->getState(), TransportState::LISTENING);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ListenWhenNotBoundThrows)
{
    EXPECT_THROW(
        {
            try
            {
                m_transport->listen(10);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::LISTEN_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, AcceptSuccess)
{
    const TransportEndpoint listenEp("0.0.0.0", 6000);
    const TransportEndpoint clientLocalEp("127.0.0.1", 6000);
    const TransportEndpoint clientRemoteEp("127.0.0.1", 45678);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(_, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, bind(_)).Times(1);
    m_transport->bind(listenEp);

    EXPECT_CALL(*m_mockSocket, listen(10)).Times(1);
    m_transport->listen(10);

    auto clientMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*clientMock, getLocalEndpoint())
        .WillOnce(ReturnRef(clientLocalEp));
    EXPECT_CALL(*clientMock, getRemoteEndpoint())
        .WillOnce(ReturnRef(clientRemoteEp));
    EXPECT_CALL(*clientMock, close()).Times(1);

    EXPECT_CALL(*m_mockSocket, accept(3000))
        .WillOnce(Return(ByMove(std::move(clientMock))));

    auto clientTransport = m_transport->accept(3000);
    ASSERT_NE(clientTransport, nullptr);
    EXPECT_EQ(clientTransport->getState(), TransportState::CONNECTED);
    EXPECT_EQ(clientTransport->getLocalEndpoint(), clientLocalEp);
    EXPECT_EQ(clientTransport->getRemoteEndpoint(), clientRemoteEp);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, AcceptWhenNotListeningThrows)
{
    EXPECT_THROW(
        {
            try
            {
                m_transport->accept(1000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::ACCEPT_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, AcceptReturnsNullThrows)
{
    const TransportEndpoint ep("0.0.0.0", 7000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(_, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, bind(_)).Times(1);
    m_transport->bind(ep);

    EXPECT_CALL(*m_mockSocket, listen(1)).Times(1);
    m_transport->listen(1);

    EXPECT_CALL(*m_mockSocket, accept(2000))
        .WillOnce(Return(ByMove(std::unique_ptr<ISocket>())));

    EXPECT_THROW(
        {
            try
            {
                m_transport->accept(2000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::ACCEPT_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ConnectIpv4Success)
{
    const TransportEndpoint target("192.168.1.100", 80);
    const TransportEndpoint local("192.168.1.50", 54321);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV4, SocketType::STREAM,
                                    SocketProtocol::TCP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, connect(target, 5000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));

    m_transport->connect(target, 5000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);
    EXPECT_EQ(m_transport->getRemoteEndpoint(), target);
    EXPECT_EQ(m_transport->getLocalEndpoint(), local);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ConnectIpv6Success)
{
    const TransportEndpoint target("2001:db8::1", 443);
    const TransportEndpoint local("2001:db8::2", 54322);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV6, SocketType::STREAM,
                                    SocketProtocol::TCP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, connect(target, 5000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));

    m_transport->connect(target, 5000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ConnectWhenSocketAlreadyOpen)
{
    const TransportEndpoint target("10.0.0.1", 9000);
    const TransportEndpoint local("10.0.0.2", 50000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(0);
    EXPECT_CALL(*m_mockSocket, connect(target, 2000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));

    m_transport->connect(target, 2000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ConnectInInvalidStateThrows)
{
    const TransportEndpoint target("10.0.0.1", 9000);
    const TransportEndpoint local("10.0.0.2", 50000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 2000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 2000);

    EXPECT_THROW(
        {
            try
            {
                m_transport->connect(target, 2000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::CONNECT_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ConnectFailureClosesSocket)
{
    const TransportEndpoint target("10.0.0.1", 9000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 5000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->connect(target, 5000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, SendSuccess)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> payload = { 0x01, 0x02, 0x03, 0x04 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 2000)).WillOnce(Return(4));
    EXPECT_EQ(m_transport->send(payload, 2000), 4);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, SendWhenNotConnectedThrows)
{
    const std::vector<std::uint8_t> payload = { 0xFF };

    EXPECT_THROW(
        {
            try
            {
                m_transport->send(payload, 1000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::SEND_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, SendTimeoutClosesSocket)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> payload = { 0x01 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 5000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Send timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->send(payload, 5000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, SendConnectionClosedTransitionsToDisconnected)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> payload = { 0x01 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 1000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::CONNECTION_CLOSED, "Broken pipe")));

    EXPECT_THROW({ m_transport->send(payload, 1000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::DISCONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, SendGenericExceptionRethrown)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> payload = { 0x01 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 1000))
        .WillOnce(Throw(TransportException(TransportErrorCode::SEND_FAILED,
                                           "Socket write error")));

    EXPECT_THROW({ m_transport->send(payload, 1000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ReceiveSuccess)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> expected = { 0xAA, 0xBB };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, receive(1024, 5000)).WillOnce(Return(expected));
    EXPECT_EQ(m_transport->receive(1024, 5000), expected);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ReceiveWhenNotConnectedThrows)
{
    EXPECT_THROW(
        {
            try
            {
                m_transport->receive(512, 1000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::RECEIVE_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ReceiveTimeoutClosesSocket)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, receive(256, 5000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Receive timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->receive(256, 5000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ReceiveConnectionClosedDisconnects)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, receive(256, 1000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::CONNECTION_CLOSED, "Peer disconnected")));

    EXPECT_THROW({ m_transport->receive(256, 1000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::DISCONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, ReceiveGenericExceptionRethrown)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, receive(256, 1000))
        .WillOnce(Throw(TransportException(TransportErrorCode::RECEIVE_FAILED,
                                           "Read error")));

    EXPECT_THROW({ m_transport->receive(256, 1000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(TcpTransportTest, CloseIdempotency)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(3);

    m_transport->close();
    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);
    EXPECT_EQ(m_transport->getLocalEndpoint(), TransportEndpoint());
    EXPECT_EQ(m_transport->getRemoteEndpoint(), TransportEndpoint());

    m_transport->close();
    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);
}

} // namespace UnitTest
} // namespace ConnixCore
