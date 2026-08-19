#pragma once

#include <memory>

#include "ConnixCore/Communication/NodeLayer/IClient.hpp"
#include "ConnixCore/Communication/NodeLayer/IClientConfiguration.hpp"
#include "ConnixCore/Communication/NodeLayer/IServer.hpp"
#include "ConnixCore/Communication/NodeLayer/IServerConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

class INodeFactory
{
public:
    virtual ~INodeFactory() = default;

    virtual std::shared_ptr<ConnixCore::Communication::IClientConfiguration>
    createClientConfiguration() = 0;

    virtual std::shared_ptr<ConnixCore::Communication::IClient> createClient(
        std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
            configuration) = 0;

    virtual std::shared_ptr<ConnixCore::Communication::IServerConfiguration>
    createServerConfiguration() = 0;

    virtual std::shared_ptr<ConnixCore::Communication::IServer> createServer(
        std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
            configuration) = 0;
};

} // namespace Communication
} // namespace ConnixCore
