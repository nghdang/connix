#include "ConnixCore/Communication/TransportLayer/MockITransportFactory.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockITransportFactory>
MockITransportFactory::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockITransportFactory>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockITransportFactory>>
MockITransportFactory::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockITransportFactory>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockITransportFactory>>
MockITransportFactory::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockITransportFactory>>();
}

} // namespace Communication
} // namespace ConnixCore
