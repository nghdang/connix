#include "ConnixCore/Infrastructure/Transport/MockITransport.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

std::shared_ptr<MockITransport> MockITransport::create()
{
    return std::make_shared<MockITransport>();
}

std::shared_ptr<::testing::NiceMock<MockITransport>>
MockITransport::createNice()
{
    return std::make_shared<::testing::NiceMock<MockITransport>>();
}

std::shared_ptr<::testing::StrictMock<MockITransport>>
MockITransport::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockITransport>>();
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
