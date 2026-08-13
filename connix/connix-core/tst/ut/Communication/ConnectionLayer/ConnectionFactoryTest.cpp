#include "gtest/gtest.h"

#include "ConnixCore/Communication/ConnectionLayer/ConnectionFactory.hpp"
#include "ConnixCore/Communication/TransportLayer/MockITransport.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(ConnectionFactoryTest, Creation)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::ConnectionFactory>();
    EXPECT_NE(factory, nullptr);
}

TEST(ConnectionFactoryTest, CreateConnectionManager)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::ConnectionFactory>();
    auto manager = factory->createConnectionManager();
    EXPECT_NE(manager, nullptr);
}

TEST(ConnectionFactoryTest, CreateConnection)
{
    auto factory =
        std::make_shared<ConnixCore::Communication::ConnectionFactory>();

    ConnixCore::Communication::ConnectionEndpoint remote{ "127.0.0.1", 8080 };
    ConnixCore::Communication::ConnectionEndpoint local{ "127.0.0.1", 9090 };

    auto mockTransport = ConnixCore::Communication::MockITransport::create();

    auto connection = factory->createConnection(remote, local, mockTransport);
    EXPECT_NE(connection, nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
