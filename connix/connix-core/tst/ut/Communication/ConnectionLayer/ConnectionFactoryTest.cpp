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

} // namespace UnitTest
} // namespace ConnixCore
