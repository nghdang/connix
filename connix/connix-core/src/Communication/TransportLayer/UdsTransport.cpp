#include "ConnixCore/Communication/TransportLayer/UdsTransport.hpp"

namespace ConnixCore {
namespace Communication {

UdsTransport::UdsTransport()
    : BaseTransport{ ConnixCore::Communication::TransportType::UDS }
{
}

} // namespace Communication
} // namespace ConnixCore
