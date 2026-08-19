#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"

namespace ConnixCore {
namespace Communication {

class MockITransport : public ConnixCore::Communication::ITransport
{
public:
    MOCK_METHOD(ConnixCore::Communication::TransportType, getType, (),
                (const, override));

    MOCK_METHOD(ConnixCore::Communication::TransportState, getState, (),
                (const, override));

    MOCK_METHOD(ConnixCore::Communication::TransportStatus, getStatus, (),
                (const, override));

    MOCK_METHOD(
        bool, addSubscriber,
        (std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
             subscriber),
        (override));

    MOCK_METHOD(
        bool, removeSubscriber,
        (std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
             subscriber),
        (override));

    MOCK_METHOD(bool, send, (const std::vector<std::uint8_t>& data),
                (override));

    MOCK_METHOD(void, close, (), (override));

    static std::shared_ptr<ConnixCore::Communication::MockITransport> create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Communication::MockITransport>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockITransport>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
