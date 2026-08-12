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

} // namespace UnitTest
} // namespace ConnixCore
