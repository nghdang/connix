#pragma once

#include <gmock/gmock.h>
#include <memory>
#include <string>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"
#include "ConnixCore/Infrastructure/Timer/ITimerService.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerEvent.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Google Mock implementation of the ITimerService interface.
 */
class MockITimerService : public ITimerService
{
public:
    MockITimerService() = default;
    ~MockITimerService() override = default;

    MOCK_METHOD(std::string, registerTimer, (std::shared_ptr<ITimer> timer),
                (override));
    MOCK_METHOD(void, unregisterTimer, (const std::string& timerId),
                (override));
    MOCK_METHOD(std::shared_ptr<ITimer>, getTimer,
                (const std::string& timerId), (const, override));
    MOCK_METHOD(bool, hasTimer, (const std::string& timerId),
                (const, override));
    MOCK_METHOD(void, stopAll, (), (override));
    MOCK_METHOD(void, setEventHandler, (TimerEventHandler handler),
                (override));

    /**
     * @brief Creates a shared pointer to a standard MockITimerService
     * instance.
     * @return Shared pointer to MockITimerService.
     */
    static std::shared_ptr<MockITimerService> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockITimerService
     * instance.
     * @return Shared pointer to NiceMock<MockITimerService>.
     */
    static std::shared_ptr<::testing::NiceMock<MockITimerService>>
    createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockITimerService
     * instance.
     * @return Shared pointer to StrictMock<MockITimerService>.
     */
    static std::shared_ptr<::testing::StrictMock<MockITimerService>>
    createStrict();
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
