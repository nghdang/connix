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

} // namespace Communication
} // namespace ConnixCore
