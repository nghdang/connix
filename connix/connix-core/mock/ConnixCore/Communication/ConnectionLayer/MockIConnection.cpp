#include "ConnixCore/Communication/ConnectionLayer/MockIConnection.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIConnection>
MockIConnection::create()
{
    return std::make_shared<ConnixCore::Communication::MockIConnection>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIConnection>>
MockIConnection::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Communication::MockIConnection>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIConnection>>
MockIConnection::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Communication::MockIConnection>>();
}

} // namespace Communication
} // namespace ConnixCore
