#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Node/IConnixNode.hpp"

namespace ConnixCore {
namespace Node {

class MockIConnixNode : public ConnixCore::Node::IConnixNode
{
public:
    MOCK_METHOD(std::string, getName, (), (const, override));

    MOCK_METHOD(ConnixCore::Node::ConnixNodeRole, getRole, (),
                (const, override));

    MOCK_METHOD(ConnixCore::Node::ConnixTransportType, getTransportType, (),
                (const, override));

    MOCK_METHOD(
        std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>,
        getConfiguration, (), (const, override));

    static std::shared_ptr<ConnixCore::Node::MockIConnixNode> create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Node::MockIConnixNode>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Node::MockIConnixNode>>
    createNice();
};

} // namespace Node
} // namespace ConnixCore
