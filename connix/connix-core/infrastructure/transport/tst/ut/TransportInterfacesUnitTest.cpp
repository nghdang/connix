#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransportFactory.hpp"
#include "ConnixCore/Infrastructure/Transport/MockISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/MockITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/MockITransportFactory.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

TEST(TransportInterfacesTest, TransportPolymorphism)
{
    const std::unique_ptr<ITransport> transport =
        std::make_unique<MockITransport>();
    auto* mockTransport = dynamic_cast<MockITransport*>(transport.get());
    ASSERT_NE(mockTransport, nullptr);

    const TransportEndpoint localEndpoint("127.0.0.1", 8080);
    const TransportEndpoint remoteEndpoint("127.0.0.1", 9090);
    const std::vector<std::uint8_t> sendData = { 0x01, 0x02, 0x03 };
    const std::vector<std::uint8_t> recvData = { 0x04, 0x05 };

    EXPECT_CALL(*mockTransport, bind(localEndpoint)).Times(1);
    EXPECT_CALL(*mockTransport, listen(10)).Times(1);
    EXPECT_CALL(*mockTransport, accept(5000))
        .WillOnce(Return(ByMove(std::make_unique<MockITransport>())));
    EXPECT_CALL(*mockTransport, connect(remoteEndpoint, 5000)).Times(1);
    EXPECT_CALL(*mockTransport, send(sendData, 5000)).WillOnce(Return(3));
    EXPECT_CALL(*mockTransport, receive(1024, 10000))
        .WillOnce(Return(recvData));
    EXPECT_CALL(*mockTransport, close()).Times(1);
    EXPECT_CALL(*mockTransport, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*mockTransport, getState())
        .WillOnce(Return(TransportState::CONNECTED));
    EXPECT_CALL(*mockTransport, getProtocol())
        .WillOnce(Return(TransportProtocol::TCP));
    EXPECT_CALL(*mockTransport, getLocalEndpoint())
        .WillOnce(ReturnRef(localEndpoint));
    EXPECT_CALL(*mockTransport, getRemoteEndpoint())
        .WillOnce(ReturnRef(remoteEndpoint));

    transport->bind(localEndpoint);
    transport->listen(10);
    const auto accepted = transport->accept(5000);
    EXPECT_NE(accepted, nullptr);
    transport->connect(remoteEndpoint, 5000);
    EXPECT_EQ(transport->send(sendData, 5000), 3);
    EXPECT_EQ(transport->receive(1024, 10000), recvData);
    transport->close();
    EXPECT_TRUE(transport->isOpen());
    EXPECT_EQ(transport->getState(), TransportState::CONNECTED);
    EXPECT_EQ(transport->getProtocol(), TransportProtocol::TCP);
    EXPECT_EQ(transport->getLocalEndpoint(), localEndpoint);
    EXPECT_EQ(transport->getRemoteEndpoint(), remoteEndpoint);
}

TEST(TransportInterfacesTest, TransportFactoryPolymorphism)
{
    const std::unique_ptr<ITransportFactory> factory =
        std::make_unique<MockITransportFactory>();
    auto* mockFactory = dynamic_cast<MockITransportFactory*>(factory.get());
    ASSERT_NE(mockFactory, nullptr);

    EXPECT_CALL(*mockFactory, createTransport(TransportProtocol::TCP))
        .WillOnce(Return(ByMove(std::make_unique<MockITransport>())));
    EXPECT_CALL(*mockFactory, registerTransport(TransportProtocol::UDP, _))
        .Times(1);

    const auto transport = factory->createTransport(TransportProtocol::TCP);
    EXPECT_NE(transport, nullptr);

    factory->registerTransport(TransportProtocol::UDP, []() {
        return std::make_unique<MockITransport>();
    });
}

TEST(TransportInterfacesTest, SocketPolymorphism)
{
    const std::unique_ptr<ISocket> socket = std::make_unique<MockISocket>();
    auto* mockSocket = dynamic_cast<MockISocket*>(socket.get());
    ASSERT_NE(mockSocket, nullptr);

    const TransportEndpoint localEp("127.0.0.1", 7000);
    const TransportEndpoint remoteEp("127.0.0.1", 7001);
    const std::vector<std::uint8_t> payload = { 0xAA, 0xBB };

    EXPECT_CALL(*mockSocket, open(SocketDomain::IPV4, SocketType::STREAM,
                                  SocketProtocol::TCP))
        .Times(1);
    EXPECT_CALL(*mockSocket, bind(localEp)).Times(1);
    EXPECT_CALL(*mockSocket, listen(5)).Times(1);
    EXPECT_CALL(*mockSocket, accept(3000))
        .WillOnce(Return(ByMove(std::make_unique<MockISocket>())));
    EXPECT_CALL(*mockSocket, connect(remoteEp, 3000)).Times(1);
    EXPECT_CALL(*mockSocket, send(payload, 3000)).WillOnce(Return(2));
    EXPECT_CALL(*mockSocket, receive(512, 5000)).WillOnce(Return(payload));
    EXPECT_CALL(*mockSocket, sendTo(payload, remoteEp, 3000))
        .WillOnce(Return(2));
    EXPECT_CALL(*mockSocket, receiveFrom(512, _, 5000))
        .WillOnce(Return(payload));
    EXPECT_CALL(*mockSocket, setOption(SocketOption::REUSE_ADDRESS, true))
        .Times(1);
    EXPECT_CALL(*mockSocket, close()).Times(1);
    EXPECT_CALL(*mockSocket, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*mockSocket, getNativeHandle()).WillOnce(Return(42));
    EXPECT_CALL(*mockSocket, getLocalEndpoint()).WillOnce(ReturnRef(localEp));
    EXPECT_CALL(*mockSocket, getRemoteEndpoint())
        .WillOnce(ReturnRef(remoteEp));

    socket->open(SocketDomain::IPV4, SocketType::STREAM, SocketProtocol::TCP);
    socket->bind(localEp);
    socket->listen(5);
    const auto clientSock = socket->accept(3000);
    EXPECT_NE(clientSock, nullptr);
    socket->connect(remoteEp, 3000);
    EXPECT_EQ(socket->send(payload, 3000), 2);
    EXPECT_EQ(socket->receive(512, 5000), payload);
    EXPECT_EQ(socket->sendTo(payload, remoteEp, 3000), 2);
    TransportEndpoint source;
    EXPECT_EQ(socket->receiveFrom(512, source, 5000), payload);
    socket->setOption(SocketOption::REUSE_ADDRESS, true);
    socket->close();
    EXPECT_TRUE(socket->isOpen());
    EXPECT_EQ(socket->getNativeHandle(), 42);
    EXPECT_EQ(socket->getLocalEndpoint(), localEp);
    EXPECT_EQ(socket->getRemoteEndpoint(), remoteEp);
}

TEST(TransportInterfacesTest, MockFactoryCreationMethods)
{
    const auto transportMock = MockITransport::create();
    ASSERT_NE(transportMock, nullptr);

    const auto transportNice = MockITransport::createNice();
    ASSERT_NE(transportNice, nullptr);

    const auto transportStrict = MockITransport::createStrict();
    ASSERT_NE(transportStrict, nullptr);

    const auto factoryMock = MockITransportFactory::create();
    ASSERT_NE(factoryMock, nullptr);

    const auto factoryNice = MockITransportFactory::createNice();
    ASSERT_NE(factoryNice, nullptr);

    const auto factoryStrict = MockITransportFactory::createStrict();
    ASSERT_NE(factoryStrict, nullptr);

    const auto socketMock = MockISocket::create();
    ASSERT_NE(socketMock, nullptr);

    const auto socketNice = MockISocket::createNice();
    ASSERT_NE(socketNice, nullptr);

    const auto socketStrict = MockISocket::createStrict();
    ASSERT_NE(socketStrict, nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
