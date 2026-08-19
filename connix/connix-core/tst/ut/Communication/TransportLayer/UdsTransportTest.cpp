#include "gtest/gtest.h"

#include "ConnixCore/Communication/TransportLayer/UdsTransport.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(UdsTransportTest, Creation)
{
    auto transport =
        std::make_shared<ConnixCore::Communication::UdsTransport>();
    EXPECT_NE(transport, nullptr);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::UDS);
}

} // namespace UnitTest
} // namespace ConnixCore
