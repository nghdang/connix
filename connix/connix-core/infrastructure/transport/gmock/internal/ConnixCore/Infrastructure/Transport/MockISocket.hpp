#pragma once

#include <gmock/gmock.h>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

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

    static std::shared_ptr<MockISocket> create();
    static std::shared_ptr<::testing::NiceMock<MockISocket>> createNice();
    static std::shared_ptr<::testing::StrictMock<MockISocket>> createStrict();
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
