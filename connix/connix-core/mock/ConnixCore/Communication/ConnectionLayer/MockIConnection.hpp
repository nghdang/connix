#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/ConnectionLayer/IConnection.hpp"

namespace ConnixCore {
namespace Communication {

class MockIConnection : public ConnixCore::Communication::IConnection
{
public:
    MOCK_METHOD(ConnixCore::Communication::ConnectionId, getId, (),
                (const, override));

    MOCK_METHOD(ConnixCore::Communication::ConnectionEndpoint, getRemote, (),
                (const, override));

    MOCK_METHOD(ConnixCore::Communication::ConnectionEndpoint, getLocal, (),
                (const, override));

    MOCK_METHOD(ConnixCore::Communication::ConnectionState, getState, (),
                (const, override));

    MOCK_METHOD(
        bool, addSubscriber,
        (std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
             subscriber),
        (override));

    MOCK_METHOD(
        bool, removeSubscriber,
        (std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
             subscriber),
        (override));

    MOCK_METHOD(bool, send, (const std::vector<std::uint8_t>& data),
                (override));

    MOCK_METHOD(void, close, (), (override));

    static std::shared_ptr<ConnixCore::Communication::MockIConnection>
    create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Communication::MockIConnection>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIConnection>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
