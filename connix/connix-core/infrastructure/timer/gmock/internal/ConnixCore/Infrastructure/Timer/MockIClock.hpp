#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "ConnixCore/Infrastructure/Timer/IClock.hpp"
#include "ConnixCore/Infrastructure/Timer/TimerTypes.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Google Mock implementation of the IClock interface.
 */
class MockIClock : public IClock
{
public:
    MockIClock() = default;
    ~MockIClock() override = default;

    MOCK_METHOD(TimerTimePoint, now, (), (const, override));
    MOCK_METHOD(void, sleepFor, (TimerDuration duration), (override));

    /**
     * @brief Creates a shared pointer to a standard MockIClock instance.
     * @return Shared pointer to MockIClock.
     */
    static std::shared_ptr<MockIClock> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockIClock instance.
     * @return Shared pointer to NiceMock<MockIClock>.
     */
    static std::shared_ptr<::testing::NiceMock<MockIClock>> createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockIClock instance.
     * @return Shared pointer to StrictMock<MockIClock>.
     */
    static std::shared_ptr<::testing::StrictMock<MockIClock>> createStrict();
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
