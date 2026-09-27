#pragma once

#include <gmock/gmock.h>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Google Mock implementation of the ITransport interface.
 */
class MockITransport : public ITransport
{
public:
    MockITransport() = default;
    ~MockITransport() override = default;

    MOCK_METHOD(void, bind, (const TransportEndpoint& endpoint), (override));
    MOCK_METHOD(void, listen, (std::uint32_t backlog), (override));
    MOCK_METHOD(std::unique_ptr<ITransport>, accept, (std::uint32_t timeoutMs),
                (override));
    MOCK_METHOD(void, connect,
                (const TransportEndpoint& endpoint, std::uint32_t timeoutMs),
                (override));
    MOCK_METHOD(std::size_t, send,
                (const std::vector<std::uint8_t>& data,
                 std::uint32_t timeoutMs),
                (override));
    MOCK_METHOD(std::vector<std::uint8_t>, receive,
                (std::size_t maxBytes, std::uint32_t timeoutMs), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(TransportState, getState, (), (const, override));
    MOCK_METHOD(TransportProtocol, getProtocol, (), (const, override));
    MOCK_METHOD(const TransportEndpoint&, getLocalEndpoint, (),
                (const, override));
    MOCK_METHOD(const TransportEndpoint&, getRemoteEndpoint, (),
                (const, override));

    /**
     * @brief Creates a shared pointer to a standard MockITransport instance.
     * @return Shared pointer to MockITransport.
     */
    static std::shared_ptr<MockITransport> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockITransport instance.
     * @return Shared pointer to NiceMock<MockITransport>.
     */
    static std::shared_ptr<::testing::NiceMock<MockITransport>> createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockITransport instance.
     * @return Shared pointer to StrictMock<MockITransport>.
     */
    static std::shared_ptr<::testing::StrictMock<MockITransport>>
    createStrict();
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
