#include "ConnixCore/Node/MockIConnixNodeFactory.hpp"

namespace ConnixCore {
namespace Node {

std::shared_ptr<ConnixCore::Node::MockIConnixNodeFactory>
MockIConnixNodeFactory::create()
{
    return std::make_shared<ConnixCore::Node::MockIConnixNodeFactory>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Node::MockIConnixNodeFactory>>
MockIConnixNodeFactory::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Node::MockIConnixNodeFactory>>();
}

std::shared_ptr<::testing::NiceMock<ConnixCore::Node::MockIConnixNodeFactory>>
MockIConnixNodeFactory::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Node::MockIConnixNodeFactory>>();
}

} // namespace Node
} // namespace ConnixCore
