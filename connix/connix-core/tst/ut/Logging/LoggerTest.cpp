#include "gtest/gtest.h"

#include "ConnixCore/Logging/Logging.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

TEST(LoggerTest, ErrorMessage)
{
    LOG_E("This is error message");
}

TEST(LoggerTest, AllLogLevels)
{
    LOG_W("This is warn message");
    LOG_I("This is info message");
    LOG_D("This is debug message");
}

TEST(LoggerTest, MultiArguments)
{
    LOG_I("Testing multi-arguments: ", 123, " and ", 4.56, " and ",
          std::string("string"));
}

TEST(LoggerTest, AssertTrue)
{
    LOG_ASSERT(true, "Assert condition is true, should not log");
}

TEST(LoggerTest, AssertFalse)
{
    EXPECT_DEATH(
        { LOG_ASSERT(false, "Assert condition is false, should crash"); }, "");
}

} // namespace UnitTest
} // namespace ConnixCore
