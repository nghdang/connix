#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "ConnixCore/Infrastructure/Timer/ITimer.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Google Mock implementation of the ITimer interface.
 */
class MockITimer : public ITimer
{
public:
    MockITimer() = default;
    ~MockITimer() override = default;

    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(bool, isRunning, (), (const, override));
    MOCK_METHOD(TimerType, getType, (), (const, override));
    MOCK_METHOD(TimerState, getState, (), (const, override));
    MOCK_METHOD(TimerDuration, getInterval, (), (const, override));
    MOCK_METHOD(void, setCallback, (TimerCallback callback), (override));

    /**
     * @brief Creates a shared pointer to a standard MockITimer instance.
     * @return Shared pointer to MockITimer.
     */
    static std::shared_ptr<MockITimer> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockITimer instance.
     * @return Shared pointer to NiceMock<MockITimer>.
     */
    static std::shared_ptr<::testing::NiceMock<MockITimer>> createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockITimer instance.
     * @return Shared pointer to StrictMock<MockITimer>.
     */
    static std::shared_ptr<::testing::StrictMock<MockITimer>> createStrict();
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
