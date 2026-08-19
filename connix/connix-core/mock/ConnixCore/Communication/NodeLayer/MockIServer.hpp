#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/IServer.hpp"

namespace ConnixCore {
namespace Communication {

class MockIServer : public ConnixCore::Communication::IServer
{
public:
    MOCK_METHOD(
        std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>,
        getConfiguration, (), (const, override));

    MOCK_METHOD(bool, addSubscriber,
                (std::shared_ptr<ConnixCore::Communication::IServerSubscriber>
                     subscriber),
                (override));

    MOCK_METHOD(bool, removeSubscriber,
                (std::shared_ptr<ConnixCore::Communication::IServerSubscriber>
                     subscriber),
                (override));

    MOCK_METHOD(bool, start,
                (const std::string& address, const std::uint16_t port),
                (override));

    MOCK_METHOD(bool, stop, (), (override));

    MOCK_METHOD(bool, send,
                (const ConnixCore::Communication::ConnectionId connectionId,
                 const std::vector<std::uint8_t>& data),
                (override));

    MOCK_METHOD(bool, closeConnection,
                (const ConnixCore::Communication::ConnectionId connectionId),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockIServer> create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Communication::MockIServer>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIServer>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
