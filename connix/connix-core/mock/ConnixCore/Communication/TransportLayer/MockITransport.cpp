#include "ConnixCore/Communication/TransportLayer/MockITransport.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockITransport>
MockITransport::create()
{
    return std::make_shared<ConnixCore::Communication::MockITransport>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockITransport>>
MockITransport::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Communication::MockITransport>>();
}

std::shared_ptr<::testing::NiceMock<ConnixCore::Communication::MockITransport>>
MockITransport::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Communication::MockITransport>>();
}

} // namespace Communication
} // namespace ConnixCore
