#include "gtest/gtest.h"

#include "ConnixCore/Communication/ConnectionLayer/ConnectionManager.hpp"
#include "ConnixCore/Communication/ConnectionLayer/MockIConnection.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(ConnectionManagerTest, BasicMethods)
{
    auto manager =
        std::make_shared<ConnixCore::Communication::ConnectionManager>();
    EXPECT_NE(manager, nullptr);

    EXPECT_EQ(manager->getConnectionCount(), 0);
    auto mockConn = ConnixCore::Communication::MockIConnection::create();
    EXPECT_FALSE(manager->addConnection(mockConn));
    EXPECT_FALSE(manager->removeConnection(1));
}

} // namespace UnitTest
} // namespace ConnixCore
