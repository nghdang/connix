#include "gtest/gtest.h"

#include "ConnixCore/Communication/NodeLayer/ServerConfiguration.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(ServerConfigurationTest, SettersAndGetters)
{
    auto config =
        std::make_shared<ConnixCore::Communication::ServerConfiguration>();
    ASSERT_NE(config, nullptr);

    config->setTransportType(ConnixCore::Communication::TransportType::UDP);
    config->setMaximumConnections(100);
    config->setMaximumRequestsPerSecond(1000);
    config->setBufferSize(2048);

    EXPECT_EQ(config->getTransportType(),
              ConnixCore::Communication::TransportType::UDP);
    EXPECT_EQ(config->getMaximumConnections(), 100);
    EXPECT_EQ(config->getMaximumRequestsPerSecond(), 1000);
    EXPECT_EQ(config->getBufferSize(), 2048);
}

} // namespace UnitTest
} // namespace ConnixCore
