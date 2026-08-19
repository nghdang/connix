#include "ConnixCore/Communication/ConnectionLayer/MockIConnectionFactory.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIConnectionFactory>
MockIConnectionFactory::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIConnectionFactory>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIConnectionFactory>>
MockIConnectionFactory::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIConnectionFactory>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIConnectionFactory>>
MockIConnectionFactory::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIConnectionFactory>>();
}

} // namespace Communication
} // namespace ConnixCore
