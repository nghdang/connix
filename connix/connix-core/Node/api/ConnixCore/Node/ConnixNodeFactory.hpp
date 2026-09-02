#pragma once

#include "ConnixCore/Node/IConnixNodeFactory.hpp"

namespace ConnixCore {
namespace Node {

class ConnixNodeFactory : public ConnixCore::Node::IConnixNodeFactory
{
public:
    explicit ConnixNodeFactory() = default;

    ~ConnixNodeFactory() override = default;

    std::shared_ptr<ConnixCore::Node::IConnixNode> createClient(
        const std::string& name,
        const ConnixCore::Node::ConnixTransportType transportType,
        const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
            configuration) const override;

    std::shared_ptr<ConnixCore::Node::IConnixNode> createServer(
        const std::string& name,
        const ConnixCore::Node::ConnixTransportType transportType,
        const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
            configuration) const override;
};

} // namespace Node
} // namespace ConnixCore
