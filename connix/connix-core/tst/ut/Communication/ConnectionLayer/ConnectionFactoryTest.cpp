#include "gtest/gtest.h"

#include "ConnixCore/Communication/ConnectionLayer/ConnectionFactory.hpp"

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

} // namespace UnitTest
} // namespace ConnixCore
