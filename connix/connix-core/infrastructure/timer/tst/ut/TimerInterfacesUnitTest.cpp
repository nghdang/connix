#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <chrono>
#include <memory>
#include <string>

#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimerFactory.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimerService.hpp"
#include "ConnixCore/Infrastructure/Timer/MockIClock.hpp"
#include "ConnixCore/Infrastructure/Timer/MockITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/MockITimerFactory.hpp"
#include "ConnixCore/Infrastructure/Timer/MockITimerService.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerState.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Timer;

namespace ConnixCore {
namespace UnitTest {

TEST(TimerInterfacesTest, TimerPolymorphismAndMockMethods)
{
    const std::shared_ptr<ITimer> timer = std::make_shared<MockITimer>();
    auto* mockTimer = dynamic_cast<MockITimer*>(timer.get());
    ASSERT_NE(mockTimer, nullptr);

    EXPECT_CALL(*mockTimer, start()).Times(1);
    EXPECT_CALL(*mockTimer, stop()).Times(1);
    EXPECT_CALL(*mockTimer, reset()).Times(1);
    EXPECT_CALL(*mockTimer, isRunning()).WillOnce(Return(true));
    EXPECT_CALL(*mockTimer, getType()).WillOnce(Return(TimerType::PERIODIC));
    EXPECT_CALL(*mockTimer, getState()).WillOnce(Return(TimerState::RUNNING));
    EXPECT_CALL(*mockTimer, getInterval())
        .WillOnce(Return(std::chrono::milliseconds(1000)));
    EXPECT_CALL(*mockTimer, setCallback(_)).Times(1);

    timer->start();
    EXPECT_TRUE(timer->isRunning());
    EXPECT_EQ(timer->getType(), TimerType::PERIODIC);
    EXPECT_EQ(timer->getState(), TimerState::RUNNING);
    EXPECT_EQ(timer->getInterval(), std::chrono::milliseconds(1000));
    timer->setCallback([]() {
    });
    timer->reset();
    timer->stop();
}

TEST(TimerInterfacesTest, TimerFactoryPolymorphismAndMockMethods)
{
    const std::shared_ptr<ITimerFactory> factory =
        std::make_shared<MockITimerFactory>();
    auto* mockFactory = dynamic_cast<MockITimerFactory*>(factory.get());
    ASSERT_NE(mockFactory, nullptr);

    const auto mockTimerSingle = std::make_shared<MockITimer>();
    const auto mockTimerPeriodic = std::make_shared<MockITimer>();

    EXPECT_CALL(*mockFactory,
                createSingleShotTimer(std::chrono::milliseconds(500), _))
        .WillOnce(Return(mockTimerSingle));
    EXPECT_CALL(*mockFactory,
                createPeriodicTimer(std::chrono::milliseconds(1000), _))
        .WillOnce(Return(mockTimerPeriodic));

    const auto singleTimer =
        factory->createSingleShotTimer(std::chrono::milliseconds(500));
    EXPECT_EQ(singleTimer, mockTimerSingle);

    const auto periodicTimer =
        factory->createPeriodicTimer(std::chrono::milliseconds(1000));
    EXPECT_EQ(periodicTimer, mockTimerPeriodic);
}

TEST(TimerInterfacesTest, TimerServicePolymorphismAndMockMethods)
{
    const std::shared_ptr<ITimerService> service =
        std::make_shared<MockITimerService>();
    auto* mockService = dynamic_cast<MockITimerService*>(service.get());
    ASSERT_NE(mockService, nullptr);

    const std::shared_ptr<ITimer> mockTimer = std::make_shared<MockITimer>();

    EXPECT_CALL(*mockService, registerTimer(mockTimer))
        .WillOnce(Return("timer_1"));
    EXPECT_CALL(*mockService, hasTimer("timer_1")).WillOnce(Return(true));
    EXPECT_CALL(*mockService, getTimer("timer_1")).WillOnce(Return(mockTimer));
    EXPECT_CALL(*mockService, unregisterTimer("timer_1")).Times(1);
    EXPECT_CALL(*mockService, stopAll()).Times(1);
    EXPECT_CALL(*mockService, setEventHandler(_)).Times(1);

    const std::string id = service->registerTimer(mockTimer);
    EXPECT_EQ(id, "timer_1");
    EXPECT_TRUE(service->hasTimer("timer_1"));
    EXPECT_EQ(service->getTimer("timer_1"), mockTimer);
    service->setEventHandler([](const TimerEvent&) {
    });
    service->unregisterTimer("timer_1");
    service->stopAll();
}

TEST(TimerInterfacesTest, ClockPolymorphismAndMockMethods)
{
    const std::shared_ptr<IClock> clock = std::make_shared<MockIClock>();
    auto* mockClock = dynamic_cast<MockIClock*>(clock.get());
    ASSERT_NE(mockClock, nullptr);

    const auto fixedTime = std::chrono::steady_clock::now();
    EXPECT_CALL(*mockClock, now()).WillOnce(Return(fixedTime));
    EXPECT_CALL(*mockClock, sleepFor(std::chrono::milliseconds(100))).Times(1);

    EXPECT_EQ(clock->now(), fixedTime);
    clock->sleepFor(std::chrono::milliseconds(100));
}

TEST(TimerInterfacesTest, MockStaticFactoryMethods)
{
    EXPECT_NE(MockITimer::create(), nullptr);
    EXPECT_NE(MockITimer::createNice(), nullptr);
    EXPECT_NE(MockITimer::createStrict(), nullptr);

    EXPECT_NE(MockITimerFactory::create(), nullptr);
    EXPECT_NE(MockITimerFactory::createNice(), nullptr);
    EXPECT_NE(MockITimerFactory::createStrict(), nullptr);

    EXPECT_NE(MockITimerService::create(), nullptr);
    EXPECT_NE(MockITimerService::createNice(), nullptr);
    EXPECT_NE(MockITimerService::createStrict(), nullptr);

    EXPECT_NE(MockIClock::create(), nullptr);
    EXPECT_NE(MockIClock::createNice(), nullptr);
    EXPECT_NE(MockIClock::createStrict(), nullptr);
}

} // namespace UnitTest
} // namespace ConnixCore
