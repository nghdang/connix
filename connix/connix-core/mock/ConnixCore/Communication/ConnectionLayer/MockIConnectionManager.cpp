#include "ConnixCore/Communication/ConnectionLayer/MockIConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIConnectionManager>
MockIConnectionManager::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIConnectionManager>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIConnectionManager>>
MockIConnectionManager::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIConnectionManager>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIConnectionManager>>
MockIConnectionManager::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIConnectionManager>>();
}

} // namespace Communication
} // namespace ConnixCore
