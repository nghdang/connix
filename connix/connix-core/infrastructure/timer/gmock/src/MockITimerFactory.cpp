#include "ConnixCore/Infrastructure/Timer/MockITimerFactory.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

std::shared_ptr<MockITimerFactory> MockITimerFactory::create()
{
    return std::make_shared<MockITimerFactory>();
}

std::shared_ptr<::testing::NiceMock<MockITimerFactory>>
MockITimerFactory::createNice()
{
    return std::make_shared<::testing::NiceMock<MockITimerFactory>>();
}

std::shared_ptr<::testing::StrictMock<MockITimerFactory>>
MockITimerFactory::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockITimerFactory>>();
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
