#pragma once

#include <gmock/gmock.h>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Google Mock implementation of the ISocket interface.
 */
class MockISocket : public ISocket
{
public:
    MockISocket() = default;
    ~MockISocket() override = default;

    MOCK_METHOD(void, open,
                (SocketDomain domain, SocketType type,
                 SocketProtocol protocol),
                (override));
    MOCK_METHOD(void, bind, (const TransportEndpoint& endpoint), (override));
    MOCK_METHOD(void, listen, (std::int32_t backlog), (override));
    MOCK_METHOD(std::unique_ptr<ISocket>, accept, (std::uint32_t timeoutMs),
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
    MOCK_METHOD(std::size_t, sendTo,
                (const std::vector<std::uint8_t>& data,
                 const TransportEndpoint& destination,
                 std::uint32_t timeoutMs),
                (override));
    MOCK_METHOD(std::vector<std::uint8_t>, receiveFrom,
                (std::size_t maxBytes, TransportEndpoint& source,
                 std::uint32_t timeoutMs),
                (override));
    MOCK_METHOD(void, setOption, (SocketOption option, bool enable),
                (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(bool, isOpen, (), (const, override));
    MOCK_METHOD(std::int32_t, getNativeHandle, (), (const, override));
    MOCK_METHOD(const TransportEndpoint&, getLocalEndpoint, (),
                (const, override));
    MOCK_METHOD(const TransportEndpoint&, getRemoteEndpoint, (),
                (const, override));

    /**
     * @brief Creates a shared pointer to a standard MockISocket instance.
     * @return Shared pointer to MockISocket.
     */
    static std::shared_ptr<MockISocket> create();

    /**
     * @brief Creates a shared pointer to a NiceMock MockISocket instance.
     * @return Shared pointer to NiceMock<MockISocket>.
     */
    static std::shared_ptr<::testing::NiceMock<MockISocket>> createNice();

    /**
     * @brief Creates a shared pointer to a StrictMock MockISocket instance.
     * @return Shared pointer to StrictMock<MockISocket>.
     */
    static std::shared_ptr<::testing::StrictMock<MockISocket>> createStrict();
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
