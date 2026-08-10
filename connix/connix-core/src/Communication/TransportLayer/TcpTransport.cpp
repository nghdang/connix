#include "ConnixCore/Communication/TransportLayer/TcpTransport.hpp"

namespace ConnixCore {
namespace Communication {

TcpTransport::TcpTransport()
    : BaseTransport{ ConnixCore::Communication::TransportType::TCP }
{
}

} // namespace Communication
} // namespace ConnixCore
