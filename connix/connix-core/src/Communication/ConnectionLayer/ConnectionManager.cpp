#include "ConnixCore/Communication/ConnectionLayer/ConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

ConnectionManager::ConnectionManager()
    : m_connections{}
{
}

std::size_t ConnectionManager::getConnectionCount() const
{
    return m_connections.size();
}

bool ConnectionManager::addConnection(
    std::shared_ptr<ConnixCore::Communication::IConnection> connection) const
{
    return false;
}

bool ConnectionManager::removeConnection(
    ConnixCore::Communication::ConnectionId id)
{
    return false;
}

} // namespace Communication
} // namespace ConnixCore
