#include "ConnixCore/Communication/ConnectionLayer/MockIConnectionSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIConnectionSubscriber>
MockIConnectionSubscriber::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIConnectionSubscriber>();
}

std::shared_ptr<::testing::StrictMock<
    ConnixCore::Communication::MockIConnectionSubscriber>>
MockIConnectionSubscriber::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIConnectionSubscriber>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIConnectionSubscriber>>
MockIConnectionSubscriber::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIConnectionSubscriber>>();
}

} // namespace Communication
} // namespace ConnixCore
