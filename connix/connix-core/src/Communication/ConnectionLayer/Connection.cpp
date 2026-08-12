#include "ConnixCore/Communication/ConnectionLayer/Connection.hpp"

namespace ConnixCore {
namespace Communication {

Connection::Connection(
    const ConnixCore::Communication::ConnectionId id,
    const ConnixCore::Communication::ConnectionEndpoint& remote,
    const ConnixCore::Communication::ConnectionEndpoint& local,
    std::shared_ptr<ConnixCore::Communication::ITransport> transport)
    : m_id{ id }
    , m_remote{ remote }
    , m_local{ local }
    , m_transport{ transport }
    , m_state{ ConnixCore::Communication::ConnectionState::CLOSED }
{
    m_transportSubscriber = std::make_shared<TransportSubscriber>();
    m_connectionSubscribers.clear();

    m_transport->addSubscriber(m_transportSubscriber);
}

void Connection::TransportSubscriber::onStateChanged()
{
}

void Connection::TransportSubscriber::onStatusChanged()
{
}

void Connection::TransportSubscriber::onDataSent(std::size_t count)
{
}

void Connection::TransportSubscriber::onDataReceived(
    std::vector<std::uint8_t>& receivedData,
    std::vector<std::uint8_t>& responseData)
{
}

} // namespace Communication
} // namespace ConnixCore
