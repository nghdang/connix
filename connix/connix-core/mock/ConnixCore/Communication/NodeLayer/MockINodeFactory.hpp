#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/INodeFactory.hpp"

namespace ConnixCore {
namespace Communication {

class MockINodeFactory : public ConnixCore::Communication::INodeFactory
{
public:
    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Communication::IClientConfiguration>,
        createClientConfiguration, (), (override));

    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Communication::IClient>, createClient,
        (std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
             configuration),
        (override));

    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Communication::IServerConfiguration>,
        createServerConfiguration, (), (override));

    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Communication::IServer>, createServer,
        (std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
             configuration),
        (override));

    static std::shared_ptr<ConnixCore::Communication::MockINodeFactory>
    create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Communication::MockINodeFactory>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockINodeFactory>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
