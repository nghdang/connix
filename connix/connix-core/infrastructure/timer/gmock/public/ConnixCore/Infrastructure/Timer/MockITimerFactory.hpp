#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "ConnixCore/Infrastructure/Timer/ITimerFactory.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Google Mock implementation of the ITimerFactory interface.
 */
class MockITimerFactory : public ITimerFactory
{
public:
    MockITimerFactory() = default;
    ~MockITimerFactory() override = default;

    MOCK_METHOD(std::shared_ptr<ITimer>, createSingleShotTimer,
                (TimerDuration interval, TimerCallback callback), (override));
    MOCK_METHOD(std::shared_ptr<ITimer>, createPeriodicTimer,
                (TimerDuration interval, TimerCallback callback), (override));

    /**
     * @brief Creates a shared pointer to a standard MockITimerFactory
     * instance.
     * @return Shared pointer to MockITimerFactory.
     */
    static std::shared_ptr<MockITimerFactory> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockITimerFactory
     * instance.
     * @return Shared pointer to NiceMock<MockITimerFactory>.
     */
    static std::shared_ptr<::testing::NiceMock<MockITimerFactory>>
    createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockITimerFactory
     * instance.
     * @return Shared pointer to StrictMock<MockITimerFactory>.
     */
    static std::shared_ptr<::testing::StrictMock<MockITimerFactory>>
    createStrict();
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
