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

} // namespace UnitTest
} // namespace ConnixCore
