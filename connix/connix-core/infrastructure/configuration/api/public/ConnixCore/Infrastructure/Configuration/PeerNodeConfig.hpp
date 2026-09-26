#pragma once

#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class PeerNodeConfig
{
public:
    PeerNodeConfig(NodeTransport transport, Endpoint endpoint,
                   FrameConfig frame);

    NodeTransport getTransport() const;
    const Endpoint& getEndpoint() const;
    const FrameConfig& getFrame() const;

private:
    NodeTransport m_transport;
    Endpoint m_endpoint;
    FrameConfig m_frame;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
