#include "ConnixCore/Infrastructure/Transport/MockITransportFactory.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

std::shared_ptr<MockITransportFactory> MockITransportFactory::create()
{
    return std::make_shared<MockITransportFactory>();
}

std::shared_ptr<::testing::NiceMock<MockITransportFactory>>
MockITransportFactory::createNice()
{
    return std::make_shared<::testing::NiceMock<MockITransportFactory>>();
}

std::shared_ptr<::testing::StrictMock<MockITransportFactory>>
MockITransportFactory::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockITransportFactory>>();
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
