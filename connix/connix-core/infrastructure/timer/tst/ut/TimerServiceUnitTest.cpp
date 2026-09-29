#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include "ConnixCore/Infrastructure/Timer/ITimerService.hpp"
#include "ConnixCore/Infrastructure/Timer/MockITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/Timer.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerException.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerService.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerType.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Timer;

namespace ConnixCore {
namespace UnitTest {

TEST(TimerServiceUnitTest, RegisterTimerAndAssignsUniqueId)
{
    TimerService service;
    const auto timer1 = std::make_shared<Timer>(std::chrono::milliseconds(100),
                                                TimerType::SINGLE_SHOT);
    const auto timer2 = std::make_shared<Timer>(std::chrono::milliseconds(100),
                                                TimerType::PERIODIC);

    const std::string id1 = service.registerTimer(timer1);
    const std::string id2 = service.registerTimer(timer2);

    EXPECT_NE(id1, id2);
    EXPECT_TRUE(service.hasTimer(id1));
    EXPECT_TRUE(service.hasTimer(id2));
    EXPECT_EQ(service.getTimer(id1), timer1);
    EXPECT_EQ(service.getTimer(id2), timer2);
}

TEST(TimerServiceUnitTest, RegisterNullTimerThrows)
{
    TimerService service;
    EXPECT_THROW(service.registerTimer(nullptr), TimerException);
}

TEST(TimerServiceUnitTest, GetNonExistentTimerThrows)
{
    const TimerService service;
    EXPECT_THROW(service.getTimer("unknown_id"), TimerException);
}

TEST(TimerServiceUnitTest, UnregisterTimerStopsAndRemoves)
{
    TimerService service;
    const auto timer = std::make_shared<Timer>(std::chrono::milliseconds(100),
                                               TimerType::SINGLE_SHOT);

    const std::string id = service.registerTimer(timer);
    timer->start();
    EXPECT_TRUE(timer->isRunning());

    service.unregisterTimer(id);
    EXPECT_FALSE(service.hasTimer(id));
    EXPECT_FALSE(timer->isRunning());
}

TEST(TimerServiceUnitTest, UnregisterNonExistentTimerThrows)
{
    TimerService service;
    EXPECT_THROW(service.unregisterTimer("unknown_id"), TimerException);
}

TEST(TimerServiceUnitTest, StopAllStopsAllTimers)
{
    TimerService service;
    const auto timer1 = std::make_shared<Timer>(std::chrono::milliseconds(100),
                                                TimerType::SINGLE_SHOT);
    const auto timer2 = std::make_shared<Timer>(std::chrono::milliseconds(100),
                                                TimerType::PERIODIC);

    service.registerTimer(timer1);
    service.registerTimer(timer2);

    timer1->start();
    timer2->start();
    EXPECT_TRUE(timer1->isRunning());
    EXPECT_TRUE(timer2->isRunning());

    service.stopAll();
    EXPECT_FALSE(timer1->isRunning());
    EXPECT_FALSE(timer2->isRunning());
}

TEST(TimerServiceUnitTest, TimerExpirationPumpsEventToHandler)
{
    TimerService service;
    std::atomic<bool> eventReceived{ false };
    std::string receivedTimerId;
    TimerType receivedType{ TimerType::PERIODIC };

    service.setEventHandler([&eventReceived, &receivedTimerId,
                             &receivedType](const TimerEvent& event) {
        receivedTimerId = event.getTimerId();
        receivedType = event.getType();
        eventReceived = true;
    });

    const auto timer = std::make_shared<Timer>(std::chrono::milliseconds(20),
                                               TimerType::SINGLE_SHOT);
    const std::string id = service.registerTimer(timer);

    timer->start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_TRUE(eventReceived);
    EXPECT_EQ(receivedTimerId, id);
    EXPECT_EQ(receivedType, TimerType::SINGLE_SHOT);
}

TEST(TimerServiceUnitTest,
     RegisterTimerRollsBackAndPropagatesOnCallbackFailure)
{
    TimerService service;
    const auto mockTimer = std::make_shared<MockITimer>();
    EXPECT_CALL(*mockTimer, getType())
        .WillOnce(Return(TimerType::SINGLE_SHOT));
    EXPECT_CALL(*mockTimer, setCallback(_))
        .WillOnce(Throw(std::runtime_error("Callback setup failed")));

    EXPECT_THROW(service.registerTimer(mockTimer), std::runtime_error);
    EXPECT_FALSE(service.hasTimer("timer_0"));
}

TEST(TimerServiceUnitTest, PolymorphicDeletionViaInterfacePointer)
{
    std::unique_ptr<ITimerService> service = std::make_unique<TimerService>();
    EXPECT_FALSE(service->hasTimer("nonexistent"));
    service.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
