#include "ConnixCore/Communication/TransportLayer/TransportFactory.hpp"

#include "ConnixCore/Communication/TransportLayer/TcpTransport.hpp"
#include "ConnixCore/Communication/TransportLayer/UdpTransport.hpp"
#include "ConnixCore/Communication/TransportLayer/UdsTransport.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::ITransport>
TransportFactory::createTransport(
    ConnixCore::Communication::TransportType type)
{
    switch (type)
    {
        case ConnixCore::Communication::TransportType::TCP:
        {
            return std::make_shared<ConnixCore::Communication::TcpTransport>();
        }
        case ConnixCore::Communication::TransportType::UDP:
        {
            return std::make_shared<ConnixCore::Communication::UdpTransport>();
        }
        case ConnixCore::Communication::TransportType::UDS:
        {
            return std::make_shared<ConnixCore::Communication::UdsTransport>();
        }
    }

    return nullptr;
}

} // namespace Communication
} // namespace ConnixCore
