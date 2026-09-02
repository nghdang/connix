#include "ConnixCore/Node/ConnixNodeFactory.hpp"

#include "ConnixCore/Node/ConnixNode.hpp"
#include "ConnixCore/Node/ConnixNodeRole.hpp"

namespace ConnixCore {
namespace Node {

std::shared_ptr<ConnixCore::Node::IConnixNode> ConnixNodeFactory::createClient(
    const std::string& name,
    const ConnixCore::Node::ConnixTransportType transportType,
    const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
        configuration) const
{
    return std::make_shared<ConnixCore::Node::ConnixNode>(
        name, ConnixCore::Node::ConnixNodeRole::CLIENT, transportType,
        configuration);
}

std::shared_ptr<ConnixCore::Node::IConnixNode> ConnixNodeFactory::createServer(
    const std::string& name,
    const ConnixCore::Node::ConnixTransportType transportType,
    const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
        configuration) const
{
    return std::make_shared<ConnixCore::Node::ConnixNode>(
        name, ConnixCore::Node::ConnixNodeRole::SERVER, transportType,
        configuration);
}

} // namespace Node
} // namespace ConnixCore
