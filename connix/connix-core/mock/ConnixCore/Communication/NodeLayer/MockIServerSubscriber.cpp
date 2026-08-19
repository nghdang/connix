#include "ConnixCore/Communication/NodeLayer/MockIServerSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIServerSubscriber>
MockIServerSubscriber::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIServerSubscriber>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIServerSubscriber>>
MockIServerSubscriber::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIServerSubscriber>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIServerSubscriber>>
MockIServerSubscriber::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIServerSubscriber>>();
}

} // namespace Communication
} // namespace ConnixCore
