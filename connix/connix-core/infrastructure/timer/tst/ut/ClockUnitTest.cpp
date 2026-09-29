#include "gtest/gtest.h"

#include <chrono>
#include <memory>

#include "ConnixCore/Infrastructure/Timer/Clock.hpp"
#include "ConnixCore/Infrastructure/Timer/IClock.hpp"

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

TEST(ClockUnitTest, PolymorphicDeletionViaInterfacePointer)
{
    std::unique_ptr<IClock> clock = std::make_unique<Clock>();
    EXPECT_NO_THROW(clock->now());
    clock.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
