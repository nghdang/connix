#include "ConnixCore/Communication/NodeLayer/MockIClientSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIClientSubscriber>
MockIClientSubscriber::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIClientSubscriber>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIClientSubscriber>>
MockIClientSubscriber::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIClientSubscriber>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIClientSubscriber>>
MockIClientSubscriber::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIClientSubscriber>>();
}

} // namespace Communication
} // namespace ConnixCore
