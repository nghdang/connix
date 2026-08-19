#include "gtest/gtest.h"

#include "ConnixCore/Communication/TransportLayer/TcpTransport.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(TcpTransportTest, Creation)
{
    auto transport =
        std::make_shared<ConnixCore::Communication::TcpTransport>();
    EXPECT_NE(transport, nullptr);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::TCP);
}

} // namespace UnitTest
} // namespace ConnixCore
