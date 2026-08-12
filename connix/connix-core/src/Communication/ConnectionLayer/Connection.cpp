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

ConnixCore::Communication::ConnectionId Connection::getId() const
{
    return m_id;
}

ConnixCore::Communication::ConnectionEndpoint Connection::getRemote() const
{
    return m_remote;
}

ConnixCore::Communication::ConnectionEndpoint Connection::getLocal() const
{
    return m_local;
}

ConnixCore::Communication::ConnectionState Connection::getState() const
{
    return m_state;
}

bool Connection::addSubscriber(
    std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
        subscriber)
{
    return false;
}

bool Connection::removeSubscriber(
    std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
        subscriber)
{
    return false;
}

bool Connection::send(const std::vector<std::uint8_t>& data)
{
    return false;
}

bool Connection::close()
{
    return false;
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
