#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/MockISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"
#include "ConnixCore/Infrastructure/Transport/UdpTransport.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

class UdpTransportTest : public Test
{
protected:
    void SetUp() override
    {
        auto mock = std::make_unique<StrictMock<MockISocket>>();
        m_mockSocket = mock.get();
        m_transport = std::make_unique<UdpTransport>(std::move(mock));
    }

    void TestBody() override
    {
    }

    StrictMock<MockISocket>* m_mockSocket{ nullptr };
    std::unique_ptr<UdpTransport> m_transport;
};

TEST_F(UdpTransportTest, ConstructorThrowsOnNullSocket)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW(
        {
            try
            {
                const UdpTransport transport(nullptr);
            } catch (const TransportException& ex)
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
                const UdpTransport transport(
                    nullptr, TransportState::CONNECTED, TransportEndpoint(),
                    TransportEndpoint());
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::SOCKET_CREATION_FAILED);
                throw;
            }
        },
        TransportException);
}

TEST_F(UdpTransportTest, InitialState)
{
    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_FALSE(m_transport->isOpen());
    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);
    EXPECT_EQ(m_transport->getProtocol(), TransportProtocol::UDP);
    EXPECT_EQ(m_transport->getLocalEndpoint(), TransportEndpoint());
    EXPECT_EQ(m_transport->getRemoteEndpoint(), TransportEndpoint());

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, BindIpv4Success)
{
    const TransportEndpoint ep("127.0.0.1", 9000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV4, SocketType::DATAGRAM,
                                    SocketProtocol::UDP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);
    EXPECT_EQ(m_transport->getLocalEndpoint(), ep);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, BindIpv6Success)
{
    const TransportEndpoint ep("::1", 9001);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV6, SocketType::DATAGRAM,
                                    SocketProtocol::UDP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);
    EXPECT_EQ(m_transport->getLocalEndpoint(), ep);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, BindWhenSocketAlreadyOpen)
{
    const TransportEndpoint ep("192.168.1.1", 8888);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(0);
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, BindWhenAlreadyConnectedThrows)
{
    const TransportEndpoint target("10.0.0.1", 5000);
    const TransportEndpoint local("10.0.0.2", 40000);
    const TransportEndpoint bindEp("10.0.0.2", 6000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV4, SocketType::DATAGRAM,
                                    SocketProtocol::UDP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));

    m_transport->connect(target, 1000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);

    EXPECT_THROW(
        {
            try
            {
                m_transport->bind(bindEp);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::BIND_FAILED);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ListenThrowsUnsupported)
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

TEST_F(UdpTransportTest, AcceptThrowsUnsupported)
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

TEST_F(UdpTransportTest, ConnectIpv4Success)
{
    const TransportEndpoint target("192.168.1.100", 7000);
    const TransportEndpoint local("192.168.1.50", 45000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV4, SocketType::DATAGRAM,
                                    SocketProtocol::UDP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, connect(target, 5000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));

    m_transport->connect(target, 5000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);
    EXPECT_EQ(m_transport->getRemoteEndpoint(), target);
    EXPECT_EQ(m_transport->getLocalEndpoint(), local);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ConnectIpv6Success)
{
    const TransportEndpoint target("2001:db8::1", 7001);
    const TransportEndpoint local("2001:db8::2", 45001);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::IPV6, SocketType::DATAGRAM,
                                    SocketProtocol::UDP))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, connect(target, 5000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));

    m_transport->connect(target, 5000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ConnectWhenSocketAlreadyOpen)
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

TEST_F(UdpTransportTest, ConnectFailureClosesSocket)
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

TEST_F(UdpTransportTest, SendConnectedSuccess)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> payload = { 0x01, 0x02, 0x03 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 2000)).WillOnce(Return(3));
    EXPECT_EQ(m_transport->send(payload, 2000), 3);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, SendConnectedTimeoutClosesSocket)
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

TEST_F(UdpTransportTest, SendBoundSuccessWithDestination)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("0.0.0.0", 8888);
    const TransportEndpoint peerEp("192.168.1.5", 9999);
    const std::vector<std::uint8_t> payload = { 0xAA, 0xBB };

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, sendTo(payload, peerEp, 2000)).WillOnce(Return(2));
    EXPECT_CALL(*boundMock, close()).Times(1);

    UdpTransport transport(std::move(boundMock), TransportState::BOUND, bindEp,
                           peerEp);

    EXPECT_EQ(transport.send(payload, 2000), 2);
}

TEST_F(UdpTransportTest, SendBoundMissingRemoteEndpointThrows)
{
    const TransportEndpoint bindEp("0.0.0.0", 8888);
    const std::vector<std::uint8_t> payload = { 0xAA };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(bindEp)).Times(1);

    m_transport->bind(bindEp);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);

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

TEST_F(UdpTransportTest, SendBoundTimeoutClosesSocket)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("0.0.0.0", 8888);
    const TransportEndpoint peerEp("192.168.1.5", 9999);
    const std::vector<std::uint8_t> payload = { 0xAA };

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, sendTo(payload, peerEp, 3000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*boundMock, close()).Times(2);

    UdpTransport transport(std::move(boundMock), TransportState::BOUND, bindEp,
                           peerEp);

    EXPECT_THROW({ transport.send(payload, 3000); }, TransportException);

    EXPECT_EQ(transport.getState(), TransportState::CLOSED);
}

TEST_F(UdpTransportTest, SendWhenNotConnectedOrBoundThrows)
{
    const std::vector<std::uint8_t> payload = { 0x01 };

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

TEST_F(UdpTransportTest, ReceiveConnectedSuccess)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);
    const std::vector<std::uint8_t> expected = { 0x10, 0x20 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, receive(512, 5000)).WillOnce(Return(expected));
    EXPECT_EQ(m_transport->receive(512, 5000), expected);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ReceiveConnectedTimeoutClosesSocket)
{
    const TransportEndpoint target("127.0.0.1", 8080);
    const TransportEndpoint local("127.0.0.1", 50000);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(target, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(local));
    m_transport->connect(target, 1000);

    EXPECT_CALL(*m_mockSocket, receive(256, 3000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->receive(256, 3000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ReceiveBoundSuccess)
{
    const TransportEndpoint bindEp("0.0.0.0", 9999);
    const TransportEndpoint senderEp("192.168.1.20", 34567);
    const std::vector<std::uint8_t> expected = { 0xDE, 0xAD, 0xBE, 0xEF };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(bindEp)).Times(1);
    m_transport->bind(bindEp);

    EXPECT_CALL(*m_mockSocket, receiveFrom(1024, _, 4000))
        .WillOnce(DoAll(SetArgReferee<1>(senderEp), Return(expected)));

    const auto received = m_transport->receive(1024, 4000);
    EXPECT_EQ(received, expected);
    EXPECT_EQ(m_transport->getRemoteEndpoint(), senderEp);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ReceiveBoundTimeoutClosesSocket)
{
    const TransportEndpoint bindEp("0.0.0.0", 9999);

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(bindEp)).Times(1);
    m_transport->bind(bindEp);

    EXPECT_CALL(*m_mockSocket, receiveFrom(1024, _, 4000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->receive(1024, 4000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdpTransportTest, ReceiveWhenNotConnectedOrBoundThrows)
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

TEST_F(UdpTransportTest, CloseIdempotency)
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
