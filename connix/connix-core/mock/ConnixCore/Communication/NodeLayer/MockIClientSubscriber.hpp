#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/IClientSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class MockIClientSubscriber
    : public ConnixCore::Communication::IClientSubscriber
{
public:
    MOCK_METHOD(void, onConnection,
                (const std::shared_ptr<ConnixCore::Communication::IConnection>&
                     connection),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockIClientSubscriber>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIClientSubscriber>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIClientSubscriber>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
