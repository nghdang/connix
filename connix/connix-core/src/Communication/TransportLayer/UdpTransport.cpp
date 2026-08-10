#include "ConnixCore/Communication/TransportLayer/UdpTransport.hpp"

namespace ConnixCore {
namespace Communication {

UdpTransport::UdpTransport()
    : BaseTransport{ ConnixCore::Communication::TransportType::UDP }
{
}

} // namespace Communication
} // namespace ConnixCore
