#include "ConnixCore/Communication/NodeLayer/Client.hpp"

namespace ConnixCore {
namespace Communication {

Client::Client(
    std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
        configuration)
    : m_configuration{ configuration }
{
}

std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
Client::getConfiguration() const
{
    return m_configuration;
}

bool Client::addSubscriber(
    std::shared_ptr<ConnixCore::Communication::IClientSubscriber> subscriber)
{
    return false;
}

bool Client::removeSubscriber(
    std::shared_ptr<ConnixCore::Communication::IClientSubscriber> subscriber)
{
    return false;
}

bool Client::connect(const std::string& address, const std::uint16_t port)
{
    return false;
}

bool Client::disconnect()
{
    return false;
}

bool Client::send(const ConnixCore::Communication::ConnectionId connectionId,
                  const std::vector<std::uint8_t>& data)
{
    return false;
}

bool Client::closeConnection(
    const ConnixCore::Communication::ConnectionId connectionId)
{
    return false;
}

} // namespace Communication
} // namespace ConnixCore
