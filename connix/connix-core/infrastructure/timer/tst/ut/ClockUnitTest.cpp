#include "gtest/gtest.h"

#include <chrono>

#include "ConnixCore/Infrastructure/Timer/Clock.hpp"

using namespace ConnixCore::Infrastructure::Timer;

namespace ConnixCore {
namespace UnitTest {

TEST(ClockUnitTest, NowReturnsMonotonicTime)
{
    const Clock clock;
    const auto t1 = clock.now();
    const auto t2 = clock.now();

    EXPECT_LE(t1, t2);
}

TEST(ClockUnitTest, SleepForSuspendsExecution)
{
    Clock clock;
    const auto t1 = clock.now();
    clock.sleepFor(std::chrono::milliseconds(20));
    const auto t2 = clock.now();

    EXPECT_GE(t2 - t1, std::chrono::milliseconds(15));
}

} // namespace UnitTest
} // namespace ConnixCore
