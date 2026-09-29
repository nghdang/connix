#include "ConnixCore/Infrastructure/Timer/MockITimerService.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

std::shared_ptr<MockITimerService> MockITimerService::create()
{
    return std::make_shared<MockITimerService>();
}

std::shared_ptr<::testing::NiceMock<MockITimerService>>
MockITimerService::createNice()
{
    return std::make_shared<::testing::NiceMock<MockITimerService>>();
}

std::shared_ptr<::testing::StrictMock<MockITimerService>>
MockITimerService::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockITimerService>>();
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
