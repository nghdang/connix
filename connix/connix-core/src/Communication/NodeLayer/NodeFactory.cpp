#include "ConnixCore/Communication/NodeLayer/NodeFactory.hpp"

#include "ConnixCore/Communication/NodeLayer/Client.hpp"
#include "ConnixCore/Communication/NodeLayer/ClientConfiguration.hpp"
#include "ConnixCore/Communication/NodeLayer/Server.hpp"
#include "ConnixCore/Communication/NodeLayer/ServerConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::IClientConfiguration>
NodeFactory::createClientConfiguration()
{
    return std::make_shared<ConnixCore::Communication::ClientConfiguration>();
}

std::shared_ptr<ConnixCore::Communication::IClient> NodeFactory::createClient(
    std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
        configuration)
{
    return std::make_shared<ConnixCore::Communication::Client>(configuration);
}

std::shared_ptr<ConnixCore::Communication::IServerConfiguration>
NodeFactory::createServerConfiguration()
{
    return std::make_shared<ConnixCore::Communication::ServerConfiguration>();
}

std::shared_ptr<ConnixCore::Communication::IServer> NodeFactory::createServer(
    std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
        configuration)
{
    return std::make_shared<ConnixCore::Communication::Server>(configuration);
}

} // namespace Communication
} // namespace ConnixCore
