#include "ConnixCore/Communication/TransportLayer/UdpTransport.hpp"

namespace ConnixCore {
namespace Communication {

UdpTransport::UdpTransport(ConnixCore::Communication::TransportId id)
    : BaseTransport{ id, ConnixCore::Communication::TransportType::UDP }
{
}

} // namespace Communication
} // namespace ConnixCore
