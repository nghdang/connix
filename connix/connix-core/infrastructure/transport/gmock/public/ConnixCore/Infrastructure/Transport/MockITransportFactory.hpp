#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "ConnixCore/Infrastructure/Transport/ITransportFactory.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

class MockITransportFactory : public ITransportFactory
{
public:
    MockITransportFactory() = default;
    ~MockITransportFactory() override = default;

    MOCK_METHOD(std::unique_ptr<ITransport>, createTransport,
                (TransportProtocol protocol), (override));
    MOCK_METHOD(void, registerTransport,
                (TransportProtocol protocol, TransportCreator creator),
                (override));

    static std::shared_ptr<MockITransportFactory> create();
    static std::shared_ptr<::testing::NiceMock<MockITransportFactory>>
    createNice();
    static std::shared_ptr<::testing::StrictMock<MockITransportFactory>>
    createStrict();
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
