#include "ConnixCore/Communication/NodeLayer/MockIServerConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIServerConfiguration>
MockIServerConfiguration::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIServerConfiguration>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIServerConfiguration>>
MockIServerConfiguration::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIServerConfiguration>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIServerConfiguration>>
MockIServerConfiguration::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIServerConfiguration>>();
}

} // namespace Communication
} // namespace ConnixCore
