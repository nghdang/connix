#include "gtest/gtest.h"

#include <chrono>
#include <stdexcept>
#include <string>
#include <utility>

#include "ConnixCore/Infrastructure/Timer/TimerErrorCode.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerException.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

using namespace ConnixCore::Infrastructure::Timer;

namespace ConnixCore {
namespace UnitTest {

TEST(TimerDataStructuresTest, TimerTypeEnumValues)
{
    EXPECT_NE(TimerType::SINGLE_SHOT, TimerType::PERIODIC);
}

TEST(TimerDataStructuresTest, TimerStateEnumValues)
{
    EXPECT_NE(TimerState::STOPPED, TimerState::RUNNING);
    EXPECT_NE(TimerState::STOPPED, TimerState::EXPIRED);
    EXPECT_NE(TimerState::RUNNING, TimerState::EXPIRED);
}

TEST(TimerDataStructuresTest, TimerErrorCodeEnumValues)
{
    EXPECT_NE(TimerErrorCode::TIMER_NOT_FOUND,
              TimerErrorCode::INVALID_DURATION);
    EXPECT_NE(TimerErrorCode::TIMER_ALREADY_RUNNING,
              TimerErrorCode::TIMER_NOT_RUNNING);
    EXPECT_NE(TimerErrorCode::INVALID_DURATION,
              TimerErrorCode::SYSTEM_CLOCK_ERROR);
}

TEST(TimerDataStructuresTest, TimerExceptionProperties)
{
    const TimerException exception(TimerErrorCode::INVALID_DURATION,
                                   "Interval duration cannot be zero");

    EXPECT_EQ(exception.getErrorCode(), TimerErrorCode::INVALID_DURATION);
    EXPECT_STREQ(exception.what(), "Interval duration cannot be zero");
}

TEST(TimerDataStructuresTest, TimerExceptionPolymorphicCatch)
{
    try
    {
        throw TimerException(TimerErrorCode::TIMER_NOT_FOUND,
                             "Timer timer_1 not found");
    } catch (const std::runtime_error& error)
    {
        EXPECT_STREQ(error.what(), "Timer timer_1 not found");
    }
}

TEST(TimerDataStructuresTest, TimerEventProperties)
{
    const auto now = std::chrono::steady_clock::now();
    const TimerEvent event("timer_periodic_1", TimerType::PERIODIC, now);

    EXPECT_EQ(event.getTimerId(), "timer_periodic_1");
    EXPECT_EQ(event.getType(), TimerType::PERIODIC);
    EXPECT_EQ(event.getTimestamp(), now);
}

TEST(TimerDataStructuresTest, TimerEventCopyAndMove)
{
    const auto now = std::chrono::steady_clock::now();
    TimerEvent originalEvent("timer_orig", TimerType::SINGLE_SHOT, now);
    TimerEvent copiedEvent("timer_init", TimerType::PERIODIC, now);

    copiedEvent = originalEvent;
    EXPECT_EQ(copiedEvent.getTimerId(), "timer_orig");
    EXPECT_EQ(copiedEvent.getType(), TimerType::SINGLE_SHOT);
    EXPECT_EQ(copiedEvent.getTimestamp(), now);

    const TimerEvent movedEvent(std::move(originalEvent));
    EXPECT_EQ(movedEvent.getTimerId(), "timer_orig");
    EXPECT_EQ(movedEvent.getType(), TimerType::SINGLE_SHOT);
    EXPECT_EQ(movedEvent.getTimestamp(), now);
}

TEST(TimerDataStructuresTest, TimerTypesAliases)
{
    const TimerDuration duration = std::chrono::milliseconds(500);
    EXPECT_EQ(duration.count(), 500);

    const TimerTimePoint timePoint = std::chrono::steady_clock::now();
    EXPECT_GE(timePoint.time_since_epoch().count(), 0);
}

} // namespace UnitTest
} // namespace ConnixCore
