#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/IClient.hpp"

namespace ConnixCore {
namespace Communication {

class MockIClient : public ConnixCore::Communication::IClient
{
public:
    MOCK_METHOD(
        std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>,
        getConfiguration, (), (const, override));

    MOCK_METHOD(bool, addSubscriber,
                (std::shared_ptr<ConnixCore::Communication::IClientSubscriber>
                     subscriber),
                (override));

    MOCK_METHOD(bool, removeSubscriber,
                (std::shared_ptr<ConnixCore::Communication::IClientSubscriber>
                     subscriber),
                (override));

    MOCK_METHOD(bool, connect,
                (const std::string& address, const std::uint16_t port),
                (override));

    MOCK_METHOD(bool, disconnect, (), (override));

    MOCK_METHOD(bool, send,
                (const ConnixCore::Communication::ConnectionId connectionId,
                 const std::vector<std::uint8_t>& data),
                (override));

    MOCK_METHOD(bool, closeConnection,
                (const ConnixCore::Communication::ConnectionId connectionId),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockIClient> create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Communication::MockIClient>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIClient>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
