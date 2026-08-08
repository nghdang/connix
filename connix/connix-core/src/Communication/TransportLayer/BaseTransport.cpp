#include "ConnixCore/Communication/TransportLayer/BaseTransport.hpp"

namespace ConnixCore {
namespace Communication {

BaseTransport::BaseTransport(ConnixCore::Communication::TransportId id,
                             ConnixCore::Communication::TransportType type)
    : m_id{ id }
    , m_type{ type }
    , m_state{ ConnixCore::Communication::TransportState::DISCONNECTED }
    , m_status{ ConnixCore::Communication::TransportStatus::IDLE }
    , m_sendBuffer{}
    , m_recvBuffer{}
{
}

} // namespace Communication
} // namespace ConnixCore
