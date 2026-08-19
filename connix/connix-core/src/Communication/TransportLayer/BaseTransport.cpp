#include "ConnixCore/Communication/TransportLayer/BaseTransport.hpp"

namespace ConnixCore {
namespace Communication {

BaseTransport::BaseTransport(ConnixCore::Communication::TransportType type)
    : m_type{ type }
    , m_state{ ConnixCore::Communication::TransportState::DISCONNECTED }
    , m_status{ ConnixCore::Communication::TransportStatus::IDLE }
    , m_sendBuffer{}
    , m_recvBuffer{}
{
}

ConnixCore::Communication::TransportType BaseTransport::getType() const
{
    return m_type;
}

ConnixCore::Communication::TransportState BaseTransport::getState() const
{
    return m_state;
}

ConnixCore::Communication::TransportStatus BaseTransport::getStatus() const
{
    return m_status;
}

bool BaseTransport::addSubscriber(
    std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
        subscriber)
{
    return false;
}

bool BaseTransport::removeSubscriber(
    std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
        subscriber)
{
    return false;
}

bool BaseTransport::send(const std::vector<std::uint8_t>& data)
{
    return false;
}

void BaseTransport::close()
{
}

} // namespace Communication
} // namespace ConnixCore
