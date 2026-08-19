#include "ConnixCore/Communication/NodeLayer/Server.hpp"

namespace ConnixCore {
namespace Communication {

Server::Server(
    std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
        configuration)
    : m_configuration{ configuration }
{
}

std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
Server::getConfiguration() const
{
    return m_configuration;
}

bool Server::addSubscriber(
    std::shared_ptr<ConnixCore::Communication::IServerSubscriber> subscriber)
{
    return false;
}

bool Server::removeSubscriber(
    std::shared_ptr<ConnixCore::Communication::IServerSubscriber> subscriber)
{
    return false;
}

bool Server::start(const std::string& address, const std::uint16_t port)
{
    return false;
}

bool Server::stop()
{
    return false;
}

bool Server::send(const ConnixCore::Communication::ConnectionId connectionId,
                  const std::vector<std::uint8_t>& data)
{
    return false;
}

bool Server::closeConnection(
    const ConnixCore::Communication::ConnectionId connectionId)
{
    return false;
}

} // namespace Communication
} // namespace ConnixCore
