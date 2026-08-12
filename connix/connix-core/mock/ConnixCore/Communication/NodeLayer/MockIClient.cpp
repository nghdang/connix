#include "ConnixCore/Communication/NodeLayer/MockIClient.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIClient> MockIClient::create()
{
    return std::make_shared<ConnixCore::Communication::MockIClient>();
}

std::shared_ptr<::testing::StrictMock<ConnixCore::Communication::MockIClient>>
MockIClient::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Communication::MockIClient>>();
}

std::shared_ptr<::testing::NiceMock<ConnixCore::Communication::MockIClient>>
MockIClient::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Communication::MockIClient>>();
}

} // namespace Communication
} // namespace ConnixCore
