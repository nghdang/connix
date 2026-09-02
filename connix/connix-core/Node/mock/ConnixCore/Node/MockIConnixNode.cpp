#include "ConnixCore/Node/MockIConnixNode.hpp"

namespace ConnixCore {
namespace Node {

std::shared_ptr<ConnixCore::Node::MockIConnixNode> MockIConnixNode::create()
{
    return std::make_shared<ConnixCore::Node::MockIConnixNode>();
}

std::shared_ptr<::testing::StrictMock<ConnixCore::Node::MockIConnixNode>>
MockIConnixNode::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Node::MockIConnixNode>>();
}

std::shared_ptr<::testing::NiceMock<ConnixCore::Node::MockIConnixNode>>
MockIConnixNode::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Node::MockIConnixNode>>();
}

} // namespace Node
} // namespace ConnixCore
