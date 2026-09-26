#include "ConnixCore/Infrastructure/Configuration/MockIConfigurationProvider.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

std::shared_ptr<MockIConfigurationProvider>
MockIConfigurationProvider::create()
{
    return std::make_shared<MockIConfigurationProvider>();
}

std::shared_ptr<::testing::NiceMock<MockIConfigurationProvider>>
MockIConfigurationProvider::createNice()
{
    return std::make_shared<::testing::NiceMock<MockIConfigurationProvider>>();
}

std::shared_ptr<::testing::StrictMock<MockIConfigurationProvider>>
MockIConfigurationProvider::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<MockIConfigurationProvider>>();
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
