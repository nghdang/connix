#pragma once

#include "ConnixCore/Communication/NodeLayer/INodeFactory.hpp"

namespace ConnixCore {
namespace Communication {

class NodeFactory : public ConnixCore::Communication::INodeFactory
{
public:
    explicit NodeFactory() = default;

    ~NodeFactory() override = default;

    std::shared_ptr<ConnixCore::Communication::IClientConfiguration>
    createClientConfiguration() override;

    std::shared_ptr<ConnixCore::Communication::IClient> createClient(
        std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
            configuration) override;

    std::shared_ptr<ConnixCore::Communication::IServerConfiguration>
    createServerConfiguration() override;

    std::shared_ptr<ConnixCore::Communication::IServer> createServer(
        std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
            configuration) override;
};

} // namespace Communication
} // namespace ConnixCore
