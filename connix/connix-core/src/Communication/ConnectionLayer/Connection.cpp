#include "ConnixCore/Communication/ConnectionLayer/Connection.hpp"

namespace ConnixCore {
namespace Communication {

Connection::Connection(
    const ConnixCore::Communication::ConnectionId id,
    const ConnixCore::Communication::ConnectionEndpoint& remote,
    const ConnixCore::Communication::ConnectionEndpoint& local,
    const std::vector<
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>&
        subscribers,
    std::shared_ptr<ConnixCore::Communication::ITransport> transport)
    : m_id{ id }
    , m_remote{ remote }
    , m_local{ local }
    , m_subscribers{ subscribers }
    , m_transport{ transport }
    , m_state{ ConnixCore::Communication::ConnectionState::CLOSED }
{
}

} // namespace Communication
} // namespace ConnixCore
