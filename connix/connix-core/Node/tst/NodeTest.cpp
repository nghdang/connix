#include "gtest/gtest.h"

#include "ConnixCore/Common/CommonFactory.hpp"
#include "ConnixCore/Node/ConnixNodeFactory.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(NodeTest, Creation)
{
    ConnixCore::Node::ConnixNodeFactory factory;

    auto configuration =
        ConnixCore::Common::CommonFactory::createConfiguration();
    auto client = factory.createClient(
        "client", ConnixCore::Node::ConnixTransportType::TCP, configuration);

    EXPECT_NE(client, nullptr);
    EXPECT_EQ(client->getName(), "client");
    EXPECT_EQ(client->getTransportType(),
              ConnixCore::Node::ConnixTransportType::TCP);
    EXPECT_EQ(client->getConfiguration(), configuration);
}

} // namespace UnitTest
} // namespace ConnixCore
