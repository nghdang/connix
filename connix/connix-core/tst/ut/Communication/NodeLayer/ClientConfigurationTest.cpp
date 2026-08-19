#include "gtest/gtest.h"

#include "ConnixCore/Communication/NodeLayer/ClientConfiguration.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(ClientConfigurationTest, SettersAndGetters)
{
    auto config =
        std::make_shared<ConnixCore::Communication::ClientConfiguration>();
    ASSERT_NE(config, nullptr);

    config->setTransportType(ConnixCore::Communication::TransportType::TCP);
    config->setConnectionTimeout(5000);
    config->setKeepConnectionAlive(true);
    config->setBufferSize(1024);

    EXPECT_EQ(config->getTransportType(),
              ConnixCore::Communication::TransportType::TCP);
    EXPECT_EQ(config->getConnectionTimeout(), 5000);
    EXPECT_TRUE(config->getKeepConnectionAlive());
    EXPECT_EQ(config->getBufferSize(), 1024);
}

} // namespace UnitTest
} // namespace ConnixCore
