#include "ConnixCore/Node/ConnixNode.hpp"

namespace ConnixCore {
namespace Node {

ConnixNode::ConnixNode(
    const std::string& name, const ConnixCore::Node::ConnixNodeRole role,
    const ConnixCore::Node::ConnixTransportType transportType,
    const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
        configuration)
    : m_name{ name }
    , m_role{ role }
    , m_transportType{ transportType }
    , m_configuration{ configuration }
{
}

std::string ConnixNode::getName() const
{
    return m_name;
}

ConnixCore::Node::ConnixNodeRole ConnixNode::getRole() const
{
    return m_role;
}

ConnixCore::Node::ConnixTransportType ConnixNode::getTransportType() const
{
    return m_transportType;
}

std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
ConnixNode::getConfiguration() const
{
    return m_configuration;
}

} // namespace Node
} // namespace ConnixCore
