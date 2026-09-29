#include "ConnixCore/Infrastructure/Timer/MockITimer.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

std::shared_ptr<MockITimer> MockITimer::create()
{
    return std::make_shared<MockITimer>();
}

std::shared_ptr<::testing::NiceMock<MockITimer>> MockITimer::createNice()
{
    return std::make_shared<::testing::NiceMock<MockITimer>>();
}

std::shared_ptr<::testing::StrictMock<MockITimer>> MockITimer::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockITimer>>();
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
