#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

class MockIConnectionManager
    : public ConnixCore::Communication::IConnectionManager
{
public:
    MOCK_METHOD(std::size_t, getConnectionCount, (), (const, override));

    MOCK_METHOD(
        bool, addConnection,
        (std::shared_ptr<ConnixCore::Communication::IConnection> connection),
        (const, override));

    MOCK_METHOD(bool, removeConnection,
                (ConnixCore::Communication::ConnectionId id), (override));

    static std::shared_ptr<ConnixCore::Communication::MockIConnectionManager>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIConnectionManager>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIConnectionManager>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
