#include "ConnixCore/Communication/TransportLayer/MockITransportSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockITransportSubscriber>
MockITransportSubscriber::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockITransportSubscriber>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockITransportSubscriber>>
MockITransportSubscriber::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockITransportSubscriber>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockITransportSubscriber>>
MockITransportSubscriber::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockITransportSubscriber>>();
}

} // namespace Communication
} // namespace ConnixCore
