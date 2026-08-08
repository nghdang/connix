#include "ConnixCore/Communication/TransportLayer/UdsTransport.hpp"

namespace ConnixCore {
namespace Communication {

UdsTransport::UdsTransport(ConnixCore::Communication::TransportId id)
    : BaseTransport{ id, ConnixCore::Communication::TransportType::UDS }
{
}

} // namespace Communication
} // namespace ConnixCore
