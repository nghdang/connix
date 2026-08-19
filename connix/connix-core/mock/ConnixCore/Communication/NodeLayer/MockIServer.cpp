#include "ConnixCore/Communication/NodeLayer/MockIServer.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIServer> MockIServer::create()
{
    return std::make_shared<ConnixCore::Communication::MockIServer>();
}

std::shared_ptr<::testing::StrictMock<ConnixCore::Communication::MockIServer>>
MockIServer::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Communication::MockIServer>>();
}

std::shared_ptr<::testing::NiceMock<ConnixCore::Communication::MockIServer>>
MockIServer::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Communication::MockIServer>>();
}

} // namespace Communication
} // namespace ConnixCore
