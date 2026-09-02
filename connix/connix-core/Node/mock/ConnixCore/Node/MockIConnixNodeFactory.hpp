#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Node/IConnixNodeFactory.hpp"

namespace ConnixCore {
namespace Node {

class MockIConnixNodeFactory : public ConnixCore::Node::IConnixNodeFactory
{
public:
    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Node::IConnixNode>, createClient,
        (const std::string& name,
         const ConnixCore::Node::ConnixTransportType transportType,
         const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
             configuration),
        (const, override));

    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Node::IConnixNode>, createServer,
        (const std::string& name,
         const ConnixCore::Node::ConnixTransportType transportType,
         const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
             configuration),
        (const, override));

    static std::shared_ptr<ConnixCore::Node::MockIConnixNodeFactory> create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Node::MockIConnixNodeFactory>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Node::MockIConnixNodeFactory>>
    createNice();
};

} // namespace Node
} // namespace ConnixCore
