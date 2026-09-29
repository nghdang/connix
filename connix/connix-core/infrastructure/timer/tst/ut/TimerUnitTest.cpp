#include "gtest/gtest.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/Timer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerException.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

using namespace ConnixCore::Infrastructure::Timer;

namespace ConnixCore {
namespace UnitTest {

TEST(TimerUnitTest, ZeroIntervalThrowsInvalidDuration)
{
    EXPECT_THROW(Timer(TimerDuration(0), TimerType::SINGLE_SHOT),
                 TimerException);
}

TEST(TimerUnitTest, SingleShotTimerExecutesCallbackAndExpires)
{
    std::atomic<bool> executed{ false };
    Timer timer(std::chrono::milliseconds(20), TimerType::SINGLE_SHOT, nullptr,
                [&executed]() {
                    executed = true;
                });

    EXPECT_EQ(timer.getState(), TimerState::STOPPED);
    EXPECT_FALSE(timer.isRunning());
    EXPECT_EQ(timer.getType(), TimerType::SINGLE_SHOT);
    EXPECT_EQ(timer.getInterval(), std::chrono::milliseconds(20));

    timer.start();
    EXPECT_TRUE(timer.isRunning());

    // Wait for timer completion
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_TRUE(executed);
    EXPECT_FALSE(timer.isRunning());
    EXPECT_EQ(timer.getState(), TimerState::EXPIRED);
}

TEST(TimerUnitTest, PeriodicTimerFiresMultipleTimes)
{
    std::atomic<int> counter{ 0 };
    Timer timer(std::chrono::milliseconds(20), TimerType::PERIODIC, nullptr,
                [&counter]() {
                    counter++;
                });

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(65));
    timer.stop();

    EXPECT_GE(counter.load(), 2);
    EXPECT_FALSE(timer.isRunning());
    EXPECT_EQ(timer.getState(), TimerState::STOPPED);
}

TEST(TimerUnitTest, StartRunningTimerThrowsAlreadyRunning)
{
    Timer timer(std::chrono::milliseconds(100), TimerType::SINGLE_SHOT);
    timer.start();

    EXPECT_THROW(timer.start(), TimerException);
    timer.stop();
}

TEST(TimerUnitTest, StopIdempotency)
{
    Timer timer(std::chrono::milliseconds(100), TimerType::SINGLE_SHOT);
    EXPECT_NO_THROW(timer.stop());
    EXPECT_NO_THROW(timer.stop());
}

TEST(TimerUnitTest, ResetReschedulesDeadline)
{
    std::atomic<bool> executed{ false };
    Timer timer(std::chrono::milliseconds(40), TimerType::SINGLE_SHOT, nullptr,
                [&executed]() {
                    executed = true;
                });

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    timer.reset(); // Delay expiration by another 40ms

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    EXPECT_FALSE(executed); // Not expired yet

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    EXPECT_TRUE(executed);
    timer.stop();
}

TEST(TimerUnitTest, ResetFromStoppedStartsTimer)
{
    std::atomic<bool> executed{ false };
    Timer timer(std::chrono::milliseconds(20), TimerType::SINGLE_SHOT, nullptr,
                [&executed]() {
                    executed = true;
                });

    timer.reset();
    EXPECT_TRUE(timer.isRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    EXPECT_TRUE(executed);
}

TEST(TimerUnitTest, SetCallbackUpdatesExecution)
{
    std::atomic<int> flag{ 0 };
    Timer timer(std::chrono::milliseconds(30), TimerType::SINGLE_SHOT);

    timer.setCallback([&flag]() {
        flag = 42;
    });
    timer.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(flag.load(), 42);
}

TEST(TimerUnitTest, RestartExpiredTimerRejoinsAndStarts)
{
    std::atomic<int> counter{ 0 };
    Timer timer(std::chrono::milliseconds(20), TimerType::SINGLE_SHOT, nullptr,
                [&counter]() {
                    counter++;
                });

    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(timer.getState(), TimerState::EXPIRED);
    EXPECT_EQ(counter.load(), 1);

    timer.start();
    EXPECT_TRUE(timer.isRunning());
    EXPECT_EQ(timer.getState(), TimerState::RUNNING);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(timer.getState(), TimerState::EXPIRED);
    EXPECT_EQ(counter.load(), 2);
}

TEST(TimerUnitTest, PolymorphicDeletionViaInterfacePointer)
{
    std::unique_ptr<ITimer> timer = std::make_unique<Timer>(
        std::chrono::milliseconds(20), TimerType::SINGLE_SHOT);
    EXPECT_EQ(timer->getState(), TimerState::STOPPED);
    timer.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
