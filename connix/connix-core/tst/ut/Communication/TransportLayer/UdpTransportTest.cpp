#include "gtest/gtest.h"

#include "ConnixCore/Communication/TransportLayer/UdpTransport.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(UdpTransportTest, Creation)
{
    auto transport =
        std::make_shared<ConnixCore::Communication::UdpTransport>();
    EXPECT_NE(transport, nullptr);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::UDP);
}

} // namespace UnitTest
} // namespace ConnixCore
