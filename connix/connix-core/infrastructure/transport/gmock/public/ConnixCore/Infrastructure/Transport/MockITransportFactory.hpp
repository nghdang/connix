#pragma once

#include <gmock/gmock.h>
#include <memory>

#include "ConnixCore/Infrastructure/Transport/ITransportFactory.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Google Mock implementation of the ITransportFactory interface.
 */
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

    /**
     * @brief Creates a shared pointer to a standard MockITransportFactory
     * instance.
     * @return Shared pointer to MockITransportFactory.
     */
    static std::shared_ptr<MockITransportFactory> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockITransportFactory
     * instance.
     * @return Shared pointer to NiceMock<MockITransportFactory>.
     */
    static std::shared_ptr<::testing::NiceMock<MockITransportFactory>>
    createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockITransportFactory
     * instance.
     * @return Shared pointer to StrictMock<MockITransportFactory>.
     */
    static std::shared_ptr<::testing::StrictMock<MockITransportFactory>>
    createStrict();
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
