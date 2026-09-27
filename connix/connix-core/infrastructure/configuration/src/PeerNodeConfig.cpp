#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"

#include <utility>

#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

PeerNodeConfig::PeerNodeConfig(NodeTransport transport, Endpoint endpoint,
                               FrameConfig frame)
    : m_transport(transport)
    , m_endpoint(std::move(endpoint))
    , m_frame(frame)
{
}

NodeTransport PeerNodeConfig::getTransport() const
{
    return m_transport;
}

const Endpoint& PeerNodeConfig::getEndpoint() const
{
    return m_endpoint;
}

const FrameConfig& PeerNodeConfig::getFrame() const
{
    return m_frame;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
