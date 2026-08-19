#include "gtest/gtest.h"

#include "ConnixCore/Communication/TransportLayer/TransportFactory.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(TransportFactoryTest, Creation)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::TransportFactory>();
    EXPECT_NE(factory, nullptr);
}

TEST(TransportFactoryTest, CreateTcpTransport)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::TransportFactory>();
    auto transport = factory->createTransport(
        ConnixCore::Communication::TransportType::TCP);
    EXPECT_NE(transport, nullptr);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::TCP);
}

TEST(TransportFactoryTest, CreateUdpTransport)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::TransportFactory>();
    auto transport = factory->createTransport(
        ConnixCore::Communication::TransportType::UDP);
    EXPECT_NE(transport, nullptr);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::UDP);
}

TEST(TransportFactoryTest, CreateUdsTransport)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::TransportFactory>();
    auto transport = factory->createTransport(
        ConnixCore::Communication::TransportType::UDS);
    EXPECT_NE(transport, nullptr);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::UDS);
}

TEST(TransportFactoryTest, CreateInvalidTransport)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::TransportFactory>();
    auto transport = factory->createTransport(
        static_cast<ConnixCore::Communication::TransportType>(999));
    EXPECT_EQ(transport, nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
