#pragma once

#include "ConnixCore/Node/IConnixNode.hpp"

namespace ConnixCore {
namespace Node {

class IConnixNodeFactory
{
public:
    virtual ~IConnixNodeFactory() = default;

    virtual std::shared_ptr<ConnixCore::Node::IConnixNode> createClient(
        const std::string& name,
        const ConnixCore::Node::ConnixTransportType transportType,
        const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
            configuration) const = 0;

    virtual std::shared_ptr<ConnixCore::Node::IConnixNode> createServer(
        const std::string& name,
        const ConnixCore::Node::ConnixTransportType transportType,
        const std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
            configuration) const = 0;
};

} // namespace Node
} // namespace ConnixCore
