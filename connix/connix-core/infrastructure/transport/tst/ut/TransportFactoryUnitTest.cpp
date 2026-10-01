#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <cstring>
#include <memory>
#include <utility>

#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransportFactory.hpp"
#include "ConnixCore/Infrastructure/Transport/MockISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/MockITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportFactory.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

TEST(TransportFactoryUnitTest, DefaultCreationTcpTransport)
{
    TransportFactory factory;
    const auto transport = factory.createTransport(TransportProtocol::TCP);
    ASSERT_NE(transport, nullptr);
    EXPECT_EQ(transport->getProtocol(), TransportProtocol::TCP);
    EXPECT_EQ(transport->getState(), TransportState::CLOSED);
}

TEST(TransportFactoryUnitTest, DefaultCreationUdpTransport)
{
    TransportFactory factory;
    const auto transport = factory.createTransport(TransportProtocol::UDP);
    ASSERT_NE(transport, nullptr);
    EXPECT_EQ(transport->getProtocol(), TransportProtocol::UDP);
    EXPECT_EQ(transport->getState(), TransportState::CLOSED);
}

TEST(TransportFactoryUnitTest, DefaultCreationUdsStreamTransport)
{
    TransportFactory factory;
    const auto transport =
        factory.createTransport(TransportProtocol::UDS_STREAM);
    ASSERT_NE(transport, nullptr);
    EXPECT_EQ(transport->getProtocol(), TransportProtocol::UDS_STREAM);
    EXPECT_EQ(transport->getState(), TransportState::CLOSED);
}

TEST(TransportFactoryUnitTest, DefaultCreationUdsDatagramTransport)
{
    TransportFactory factory;
    const auto transport =
        factory.createTransport(TransportProtocol::UDS_DATAGRAM);
    ASSERT_NE(transport, nullptr);
    EXPECT_EQ(transport->getProtocol(), TransportProtocol::UDS_DATAGRAM);
    EXPECT_EQ(transport->getState(), TransportState::CLOSED);
}

TEST(TransportFactoryUnitTest, InjectedSocketCreatorUsed)
{
    bool creatorCalled = false;
    const auto socketCreator = [&creatorCalled]() {
        creatorCalled = true;
        return std::make_unique<MockISocket>();
    };

    TransportFactory factory(socketCreator);
    const auto transport = factory.createTransport(TransportProtocol::TCP);
    ASSERT_NE(transport, nullptr);
    EXPECT_TRUE(creatorCalled);
}

TEST(TransportFactoryUnitTest, RegisterCustomTransportCreator)
{
    TransportFactory factory;
    bool customCreatorCalled = false;

    factory.registerTransport(
        TransportProtocol::TCP, [&customCreatorCalled]() {
            customCreatorCalled = true;
            auto mockTransport = std::make_unique<NiceMock<MockITransport>>();
            ON_CALL(*mockTransport, getProtocol())
                .WillByDefault(Return(TransportProtocol::TCP));
            return mockTransport;
        });

    const auto transport = factory.createTransport(TransportProtocol::TCP);
    ASSERT_NE(transport, nullptr);
    EXPECT_TRUE(customCreatorCalled);
    EXPECT_EQ(transport->getProtocol(), TransportProtocol::TCP);
}

TEST(TransportFactoryUnitTest, UnsupportedProtocolThrows)
{
    TransportFactory factory;
    const std::uint8_t rawVal = 10;
    TransportProtocol invalidProtocol{};
    std::memcpy(&invalidProtocol, &rawVal, sizeof(rawVal));

    try
    {
        factory.createTransport(invalidProtocol);
        FAIL() << "Expected TransportException";
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::UNSUPPORTED_PROTOCOL);
    }
}

TEST(TransportFactoryUnitTest, PolymorphicDeletionViaInterfacePointer)
{
    std::unique_ptr<ITransportFactory> factory =
        std::make_unique<TransportFactory>();
    const auto transport = factory->createTransport(TransportProtocol::TCP);
    EXPECT_NE(transport, nullptr);
    factory.reset();
}

TEST(TransportFactoryUnitTest, MoveConstructorAndAssignment)
{
    TransportFactory factory1;
    factory1.registerTransport(TransportProtocol::TCP, []() {
        return std::make_unique<NiceMock<MockITransport>>();
    });

    TransportFactory factory2(std::move(factory1));
    const auto transport2 = factory2.createTransport(TransportProtocol::TCP);
    EXPECT_NE(transport2, nullptr);

    TransportFactory factory3;
    factory3 = std::move(factory2);
    const auto transport3 = factory3.createTransport(TransportProtocol::TCP);
    EXPECT_NE(transport3, nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
