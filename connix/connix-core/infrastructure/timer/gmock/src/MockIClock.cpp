#include "ConnixCore/Infrastructure/Timer/MockIClock.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

std::shared_ptr<MockIClock> MockIClock::create()
{
    return std::make_shared<MockIClock>();
}

std::shared_ptr<::testing::NiceMock<MockIClock>> MockIClock::createNice()
{
    return std::make_shared<::testing::NiceMock<MockIClock>>();
}

std::shared_ptr<::testing::StrictMock<MockIClock>> MockIClock::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockIClock>>();
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
