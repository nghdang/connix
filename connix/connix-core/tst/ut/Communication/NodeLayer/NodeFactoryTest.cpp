#include "gtest/gtest.h"

#include "ConnixCore/Communication/NodeLayer/NodeFactory.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(NodeFactoryTest, Creation)
{
    auto factory = std::make_shared<ConnixCore::Communication::NodeFactory>();
    EXPECT_NE(factory, nullptr);
}

TEST(NodeFactoryTest, CreateClientAndConfiguration)
{
    auto factory = std::make_shared<ConnixCore::Communication::NodeFactory>();
    auto clientConfig = factory->createClientConfiguration();
    EXPECT_NE(clientConfig, nullptr);
    auto client = factory->createClient(clientConfig);
    EXPECT_NE(client, nullptr);
}

TEST(NodeFactoryTest, CreateServerAndConfiguration)
{
    auto factory = std::make_shared<ConnixCore::Communication::NodeFactory>();
    auto serverConfig = factory->createServerConfiguration();
    EXPECT_NE(serverConfig, nullptr);
    auto server = factory->createServer(serverConfig);
    EXPECT_NE(server, nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
