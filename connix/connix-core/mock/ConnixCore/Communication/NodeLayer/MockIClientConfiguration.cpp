#include "ConnixCore/Communication/NodeLayer/MockIClientConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockIClientConfiguration>
MockIClientConfiguration::create()
{
    return std::make_shared<
        ConnixCore::Communication::MockIClientConfiguration>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockIClientConfiguration>>
MockIClientConfiguration::createStrict()
{
    return std::make_shared<::testing::StrictMock<
        ConnixCore::Communication::MockIClientConfiguration>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockIClientConfiguration>>
MockIClientConfiguration::createNice()
{
    return std::make_shared<::testing::NiceMock<
        ConnixCore::Communication::MockIClientConfiguration>>();
}

} // namespace Communication
} // namespace ConnixCore
