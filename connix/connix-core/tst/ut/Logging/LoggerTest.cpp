#include "gtest/gtest.h"

#include "ConnixCore/Logging/Logging.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(LoggerTest, ErrorMessage)
{
    LOG_E("This is error message");
}

} // namespace UnitTest
} // namespace ConnixCore
