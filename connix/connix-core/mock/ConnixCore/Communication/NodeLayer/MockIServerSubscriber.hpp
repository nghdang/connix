#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/IServerSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class MockIServerSubscriber
    : public ConnixCore::Communication::IServerSubscriber
{
public:
    MOCK_METHOD(void, onConnection,
                (const std::shared_ptr<ConnixCore::Communication::IConnection>&
                     connection),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockIServerSubscriber>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIServerSubscriber>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIServerSubscriber>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
