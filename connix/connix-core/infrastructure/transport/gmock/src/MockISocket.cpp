#include "ConnixCore/Infrastructure/Transport/MockISocket.hpp"

#include <gmock/gmock.h>
#include <memory>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

std::shared_ptr<MockISocket> MockISocket::create()
{
    return std::make_shared<MockISocket>();
}

std::shared_ptr<::testing::NiceMock<MockISocket>> MockISocket::createNice()
{
    return std::make_shared<::testing::NiceMock<MockISocket>>();
}

std::shared_ptr<::testing::StrictMock<MockISocket>> MockISocket::createStrict()
{
    return std::make_shared<::testing::StrictMock<MockISocket>>();
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
