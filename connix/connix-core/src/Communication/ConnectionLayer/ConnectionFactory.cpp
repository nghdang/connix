#include "ConnixCore/Communication/ConnectionLayer/ConnectionFactory.hpp"

#include "ConnixCore/Communication/ConnectionLayer/Connection.hpp"
#include "ConnixCore/Communication/ConnectionLayer/ConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

ConnectionFactory::ConnectionFactory()
    : m_connectionIdGenerator{ 0 }
{
}

std::shared_ptr<ConnixCore::Communication::IConnectionManager>
ConnectionFactory::createConnectionManager()
{
    return std::make_shared<ConnixCore::Communication::ConnectionManager>();
}

std::shared_ptr<ConnixCore::Communication::IConnection>
ConnectionFactory::createConnection(
    const ConnixCore::Communication::ConnectionEndpoint& remote,
    const ConnixCore::Communication::ConnectionEndpoint& local,
    std::shared_ptr<ConnixCore::Communication::ITransport> transport)
{
    return std::make_shared<ConnixCore::Communication::Connection>(
        m_connectionIdGenerator++, remote, local, transport);
}

} // namespace Communication
} // namespace ConnixCore
