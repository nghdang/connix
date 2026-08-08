#include "ConnixCore/Communication/TransportLayer/TcpTransport.hpp"

namespace ConnixCore {
namespace Communication {

TcpTransport::TcpTransport(ConnixCore::Communication::TransportId id)
    : BaseTransport{ id, ConnixCore::Communication::TransportType::TCP }
{
}

} // namespace Communication
} // namespace ConnixCore
