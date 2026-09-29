#include "gtest/gtest.h"

#include <chrono>
#include <memory>

#include "ConnixCore/Infrastructure/Timer/MockIClock.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerFactory.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"

using namespace ConnixCore::Infrastructure::Timer;

namespace ConnixCore {
namespace UnitTest {

TEST(TimerFactoryUnitTest, CreateSingleShotTimer)
{
    const auto mockClock = std::make_shared<MockIClock>();
    TimerFactory factory(mockClock);

    const auto timer =
        factory.createSingleShotTimer(std::chrono::milliseconds(50));
    ASSERT_NE(timer, nullptr);
    EXPECT_EQ(timer->getType(), TimerType::SINGLE_SHOT);
    EXPECT_EQ(timer->getInterval(), std::chrono::milliseconds(50));
    EXPECT_EQ(timer->getState(), TimerState::STOPPED);
}

TEST(TimerFactoryUnitTest, CreatePeriodicTimer)
{
    const auto mockClock = std::make_shared<MockIClock>();
    TimerFactory factory(mockClock);

    const auto timer =
        factory.createPeriodicTimer(std::chrono::milliseconds(100));
    ASSERT_NE(timer, nullptr);
    EXPECT_EQ(timer->getType(), TimerType::PERIODIC);
    EXPECT_EQ(timer->getInterval(), std::chrono::milliseconds(100));
    EXPECT_EQ(timer->getState(), TimerState::STOPPED);
}

TEST(TimerFactoryUnitTest, DefaultClockCreation)
{
    TimerFactory factory;
    const auto timer =
        factory.createSingleShotTimer(std::chrono::milliseconds(20));
    ASSERT_NE(timer, nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
