#include "ConnixCore/Infrastructure/Transport/TransportFactory.hpp"

#include <memory>
#include <utility>

#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransportFactory.hpp"
#include "ConnixCore/Infrastructure/Transport/Socket.hpp"
#include "ConnixCore/Infrastructure/Transport/TcpTransport.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/UdpTransport.hpp"
#include "ConnixCore/Infrastructure/Transport/UdsTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

TransportFactory::TransportFactory(SocketCreator socketCreator)
    : m_socketCreator(std::move(socketCreator))
    , m_creators()
{
}

std::unique_ptr<ITransport>
TransportFactory::createTransport(TransportProtocol protocol)
{
    const auto it = m_creators.find(protocol);
    if (it != m_creators.end() && it->second)
    {
        return it->second();
    }

    auto socket =
        m_socketCreator ? m_socketCreator() : std::make_unique<Socket>();

    switch (protocol)
    {
        case TransportProtocol::TCP:
            return std::make_unique<TcpTransport>(std::move(socket));
        case TransportProtocol::UDP:
            return std::make_unique<UdpTransport>(std::move(socket));
        case TransportProtocol::UDS_STREAM:
            return std::make_unique<UdsTransport>(
                std::move(socket), TransportProtocol::UDS_STREAM);
        case TransportProtocol::UDS_DATAGRAM:
            return std::make_unique<UdsTransport>(
                std::move(socket), TransportProtocol::UDS_DATAGRAM);
        default:
            throw TransportException(TransportErrorCode::UNSUPPORTED_PROTOCOL,
                                     "Unsupported transport protocol");
    }
}

void TransportFactory::registerTransport(TransportProtocol protocol,
                                         TransportCreator creator)
{
    m_creators[protocol] = std::move(creator);
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
