#pragma once

#include "ConnixCore/Node/ConnixNodeRole.hpp"
#include "ConnixCore/Node/ConnixTransportType.hpp"
#include "ConnixCore/Node/IConnixNode.hpp"

namespace ConnixCore {
namespace Node {

class ConnixNode : public ConnixCore::Node::IConnixNode
{
public:
    explicit ConnixNode(
        const std::string& name, const ConnixCore::Node::ConnixNodeRole role,
        const ConnixCore::Node::ConnixTransportType transportType,
        const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
            configuration);

    ~ConnixNode() override = default;

    std::string getName() const override;

    ConnixCore::Node::ConnixNodeRole getRole() const override;

    ConnixCore::Node::ConnixTransportType getTransportType() const override;

    std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
    getConfiguration() const override;

private:
    std::string m_name;

    ConnixCore::Node::ConnixNodeRole m_role;

    ConnixCore::Node::ConnixTransportType m_transportType;

    std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
        m_configuration;
};

} // namespace Node
} // namespace ConnixCore
