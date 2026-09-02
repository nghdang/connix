#include "ConnixCore/Common/MockIConnixConfiguration.hpp"

namespace ConnixCore {
namespace Common {

std::shared_ptr<ConnixCore::Common::MockIConnixConfiguration>
MockIConnixConfiguration::create()
{
    return std::make_shared<ConnixCore::Common::MockIConnixConfiguration>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Common::MockIConnixConfiguration>>
MockIConnixConfiguration::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Common::MockIConnixConfiguration>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Common::MockIConnixConfiguration>>
MockIConnixConfiguration::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Common::MockIConnixConfiguration>>();
}

} // namespace Common
} // namespace ConnixCore
