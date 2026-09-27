#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/MockISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"
#include "ConnixCore/Infrastructure/Transport/UdsTransport.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

class UdsTransportTest : public Test
{
protected:
    void SetUp() override
    {
        auto mock = std::make_unique<StrictMock<MockISocket>>();
        m_mockSocket = mock.get();
        m_transport = std::make_unique<UdsTransport>(std::move(mock));
    }

    void TestBody() override
    {
    }

    StrictMock<MockISocket>* m_mockSocket{ nullptr };
    std::unique_ptr<UdsTransport> m_transport;
};

TEST_F(UdsTransportTest, ConstructorThrowsOnNullSocket)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW(
        {
            try
            {
                const UdsTransport transport(nullptr);
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
                const UdsTransport transport(
                    nullptr, TransportProtocol::UDS_STREAM,
                    TransportState::CONNECTED, TransportEndpoint(),
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

TEST_F(UdsTransportTest, ConstructorThrowsOnInvalidProtocol)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    auto s1 = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_THROW(
        {
            try
            {
                const UdsTransport transport(std::move(s1),
                                             TransportProtocol::TCP);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::UNSUPPORTED_PROTOCOL);
                throw;
            }
        },
        TransportException);

    auto s2 = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_THROW(
        {
            try
            {
                const UdsTransport transport(
                    std::move(s2), TransportProtocol::UDP,
                    TransportState::CONNECTED, TransportEndpoint(),
                    TransportEndpoint());
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::UNSUPPORTED_PROTOCOL);
                throw;
            }
        },
        TransportException);
}

TEST_F(UdsTransportTest, InitialState)
{
    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_FALSE(m_transport->isOpen());
    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);
    EXPECT_EQ(m_transport->getProtocol(), TransportProtocol::UDS_STREAM);
    EXPECT_EQ(m_transport->getLocalEndpoint(), TransportEndpoint());
    EXPECT_EQ(m_transport->getRemoteEndpoint(), TransportEndpoint());

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, BindStreamSuccess)
{
    const TransportEndpoint ep("/tmp/connix_stream.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::UNIX, SocketType::STREAM,
                                    SocketProtocol::DEFAULT))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);
    EXPECT_EQ(m_transport->getLocalEndpoint(), ep);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, BindDatagramSuccess)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    auto dgramMock = std::make_unique<StrictMock<MockISocket>>();
    const TransportEndpoint ep("/tmp/connix_dgram.sock");

    EXPECT_CALL(*dgramMock, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*dgramMock, open(SocketDomain::UNIX, SocketType::DATAGRAM,
                                 SocketProtocol::DEFAULT))
        .Times(1);
    EXPECT_CALL(*dgramMock, bind(ep)).Times(1);
    EXPECT_CALL(*dgramMock, close()).Times(1);

    UdsTransport transport(std::move(dgramMock),
                           TransportProtocol::UDS_DATAGRAM);
    transport.bind(ep);
    EXPECT_EQ(transport.getState(), TransportState::BOUND);
    EXPECT_EQ(transport.getProtocol(), TransportProtocol::UDS_DATAGRAM);
}

TEST_F(UdsTransportTest, BindWhenSocketAlreadyOpen)
{
    const TransportEndpoint ep("/tmp/connix_open.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(0);
    EXPECT_CALL(*m_mockSocket, bind(ep)).Times(1);

    m_transport->bind(ep);
    EXPECT_EQ(m_transport->getState(), TransportState::BOUND);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, BindEmptyPathThrows)
{
    const TransportEndpoint ep("");

    EXPECT_THROW(
        {
            try
            {
                m_transport->bind(ep);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::INVALID_ADDRESS);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, BindInInvalidStateThrows)
{
    const TransportEndpoint ep("/tmp/test.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
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

TEST_F(UdsTransportTest, ListenStreamSuccess)
{
    const TransportEndpoint ep("/tmp/server.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, bind(_)).Times(1);
    m_transport->bind(ep);

    EXPECT_CALL(*m_mockSocket, listen(64)).Times(1);
    m_transport->listen(64);

    EXPECT_EQ(m_transport->getState(), TransportState::LISTENING);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, ListenDatagramThrows)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    auto dgramMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*dgramMock, close()).Times(1);

    UdsTransport transport(std::move(dgramMock),
                           TransportProtocol::UDS_DATAGRAM);

    EXPECT_THROW(
        {
            try
            {
                transport.listen(10);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::LISTEN_FAILED);
                throw;
            }
        },
        TransportException);
}

TEST_F(UdsTransportTest, ListenWhenNotBoundThrows)
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

TEST_F(UdsTransportTest, AcceptStreamSuccess)
{
    const TransportEndpoint listenEp("/tmp/uds_listen.sock");
    const TransportEndpoint clientEp("/tmp/uds_peer.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
    EXPECT_CALL(*m_mockSocket, bind(_)).Times(1);
    m_transport->bind(listenEp);

    EXPECT_CALL(*m_mockSocket, listen(5)).Times(1);
    m_transport->listen(5);

    auto peerMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*peerMock, getLocalEndpoint()).WillOnce(ReturnRef(listenEp));
    EXPECT_CALL(*peerMock, getRemoteEndpoint()).WillOnce(ReturnRef(clientEp));
    EXPECT_CALL(*peerMock, close()).Times(1);

    EXPECT_CALL(*m_mockSocket, accept(3000))
        .WillOnce(Return(ByMove(std::move(peerMock))));

    auto accepted = m_transport->accept(3000);
    ASSERT_NE(accepted, nullptr);
    EXPECT_EQ(accepted->getState(), TransportState::CONNECTED);
    EXPECT_EQ(accepted->getLocalEndpoint(), listenEp);
    EXPECT_EQ(accepted->getRemoteEndpoint(), clientEp);
    EXPECT_EQ(accepted->getProtocol(), TransportProtocol::UDS_STREAM);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, AcceptDatagramThrows)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    auto dgramMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*dgramMock, close()).Times(1);

    UdsTransport transport(std::move(dgramMock),
                           TransportProtocol::UDS_DATAGRAM);

    EXPECT_THROW(
        {
            try
            {
                transport.accept(1000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::ACCEPT_FAILED);
                throw;
            }
        },
        TransportException);
}

TEST_F(UdsTransportTest, AcceptWhenNotListeningThrows)
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

TEST_F(UdsTransportTest, AcceptReturnsNullThrows)
{
    const TransportEndpoint ep("/tmp/null_test.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(_, _, _)).Times(1);
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

TEST_F(UdsTransportTest, ConnectStreamSuccess)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(false));
    EXPECT_CALL(*m_mockSocket, open(SocketDomain::UNIX, SocketType::STREAM,
                                    SocketProtocol::DEFAULT))
        .Times(1);
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 5000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));

    m_transport->connect(serverEp, 5000);
    EXPECT_EQ(m_transport->getState(), TransportState::CONNECTED);
    EXPECT_EQ(m_transport->getRemoteEndpoint(), serverEp);
    EXPECT_EQ(m_transport->getLocalEndpoint(), clientEp);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, ConnectEmptyPathThrows)
{
    const TransportEndpoint ep("");

    EXPECT_THROW(
        {
            try
            {
                m_transport->connect(ep, 2000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(),
                          TransportErrorCode::INVALID_ADDRESS);
                throw;
            }
        },
        TransportException);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, ConnectInInvalidStateThrows)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 2000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 2000);

    EXPECT_THROW(
        {
            try
            {
                m_transport->connect(serverEp, 2000);
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

TEST_F(UdsTransportTest, ConnectFailureClosesSocket)
{
    const TransportEndpoint serverEp("/tmp/fail.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 3000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW(
        { m_transport->connect(serverEp, 3000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, SendConnectedSuccess)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");
    const std::vector<std::uint8_t> payload = { 0x11, 0x22, 0x33 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 2000)).WillOnce(Return(3));
    EXPECT_EQ(m_transport->send(payload, 2000), 3);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, SendConnectedTimeoutClosesSocket)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");
    const std::vector<std::uint8_t> payload = { 0x01 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 5000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Send timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->send(payload, 5000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest,
       SendConnectedConnectionClosedTransitionsToDisconnected)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");
    const std::vector<std::uint8_t> payload = { 0x01 };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 1000);

    EXPECT_CALL(*m_mockSocket, send(payload, 1000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::CONNECTION_CLOSED, "Peer disconnected")));

    EXPECT_THROW({ m_transport->send(payload, 1000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::DISCONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, SendDatagramBoundSuccess)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("/tmp/dgram_bind.sock");
    const TransportEndpoint peerEp("/tmp/dgram_peer.sock");
    const std::vector<std::uint8_t> payload = { 0xAA, 0xBB };

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, sendTo(payload, peerEp, 2000)).WillOnce(Return(2));
    EXPECT_CALL(*boundMock, close()).Times(1);

    UdsTransport transport(std::move(boundMock),
                           TransportProtocol::UDS_DATAGRAM,
                           TransportState::BOUND, bindEp, peerEp);

    EXPECT_EQ(transport.send(payload, 2000), 2);
}

TEST_F(UdsTransportTest, SendDatagramBoundMissingDestinationThrows)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("/tmp/dgram_bind.sock");
    const TransportEndpoint emptyEp("");
    const std::vector<std::uint8_t> payload = { 0xAA };

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, close()).Times(1);

    UdsTransport transport(std::move(boundMock),
                           TransportProtocol::UDS_DATAGRAM,
                           TransportState::BOUND, bindEp, emptyEp);

    EXPECT_THROW(
        {
            try
            {
                transport.send(payload, 1000);
            } catch (const TransportException& ex)
            {
                EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::SEND_FAILED);
                throw;
            }
        },
        TransportException);
}

TEST_F(UdsTransportTest, SendDatagramBoundTimeoutClosesSocket)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("/tmp/dgram_bind.sock");
    const TransportEndpoint peerEp("/tmp/dgram_peer.sock");
    const std::vector<std::uint8_t> payload = { 0xAA };

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, sendTo(payload, peerEp, 3000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*boundMock, close()).Times(2);

    UdsTransport transport(std::move(boundMock),
                           TransportProtocol::UDS_DATAGRAM,
                           TransportState::BOUND, bindEp, peerEp);

    EXPECT_THROW({ transport.send(payload, 3000); }, TransportException);
    EXPECT_EQ(transport.getState(), TransportState::CLOSED);
}

TEST_F(UdsTransportTest, SendWhenNotConnectedOrBoundThrows)
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

TEST_F(UdsTransportTest, ReceiveConnectedSuccess)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");
    const std::vector<std::uint8_t> expected = { 0x0A, 0x0B };

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 1000);

    EXPECT_CALL(*m_mockSocket, receive(256, 3000)).WillOnce(Return(expected));
    EXPECT_EQ(m_transport->receive(256, 3000), expected);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, ReceiveConnectedTimeoutClosesSocket)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 1000);

    EXPECT_CALL(*m_mockSocket, receive(128, 2000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    EXPECT_THROW({ m_transport->receive(128, 2000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::CLOSED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, ReceiveConnectedConnectionClosedDisconnects)
{
    const TransportEndpoint serverEp("/tmp/server.sock");
    const TransportEndpoint clientEp("/tmp/client.sock");

    EXPECT_CALL(*m_mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*m_mockSocket, connect(serverEp, 1000)).Times(1);
    EXPECT_CALL(*m_mockSocket, getLocalEndpoint())
        .WillOnce(ReturnRef(clientEp));
    m_transport->connect(serverEp, 1000);

    EXPECT_CALL(*m_mockSocket, receive(128, 2000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::CONNECTION_CLOSED, "Peer closed socket")));

    EXPECT_THROW({ m_transport->receive(128, 2000); }, TransportException);

    EXPECT_EQ(m_transport->getState(), TransportState::DISCONNECTED);

    EXPECT_CALL(*m_mockSocket, close()).Times(1);
}

TEST_F(UdsTransportTest, ReceiveDatagramBoundSuccess)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("/tmp/dgram_bind.sock");
    const TransportEndpoint senderEp("/tmp/dgram_sender.sock");
    const std::vector<std::uint8_t> expected = { 0xCA, 0xFE };

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, receiveFrom(512, _, 3000))
        .WillOnce(DoAll(SetArgReferee<1>(senderEp), Return(expected)));
    EXPECT_CALL(*boundMock, close()).Times(1);

    UdsTransport transport(std::move(boundMock),
                           TransportProtocol::UDS_DATAGRAM,
                           TransportState::BOUND, bindEp, TransportEndpoint());

    const auto received = transport.receive(512, 3000);
    EXPECT_EQ(received, expected);
    EXPECT_EQ(transport.getRemoteEndpoint(), senderEp);
}

TEST_F(UdsTransportTest, ReceiveDatagramBoundTimeoutClosesSocket)
{
    EXPECT_CALL(*m_mockSocket, close()).Times(1);

    const TransportEndpoint bindEp("/tmp/dgram_bind.sock");

    auto boundMock = std::make_unique<StrictMock<MockISocket>>();
    EXPECT_CALL(*boundMock, receiveFrom(512, _, 3000))
        .WillOnce(Throw(TransportException(
            TransportErrorCode::OPERATION_TIMEOUT, "Timed out")));
    EXPECT_CALL(*boundMock, close()).Times(2);

    UdsTransport transport(std::move(boundMock),
                           TransportProtocol::UDS_DATAGRAM,
                           TransportState::BOUND, bindEp, TransportEndpoint());

    EXPECT_THROW({ transport.receive(512, 3000); }, TransportException);
    EXPECT_EQ(transport.getState(), TransportState::CLOSED);
}

TEST_F(UdsTransportTest, ReceiveWhenNotConnectedOrBoundThrows)
{
    EXPECT_THROW(
        {
            try
            {
                m_transport->receive(256, 1000);
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

TEST_F(UdsTransportTest, CloseIdempotency)
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
