#pragma once

#include <cstdint>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ServerNodeConfig
{
public:
    ServerNodeConfig(NodeTransport transport, Endpoint endpoint,
                     FrameConfig frame, std::uint32_t maxConnections,
                     std::uint32_t bufferSize,
                     std::vector<EventRule> onReceived);

    NodeTransport getTransport() const;
    const Endpoint& getEndpoint() const;
    const FrameConfig& getFrame() const;
    std::uint32_t getMaxConnections() const;
    std::uint32_t getBufferSize() const;
    const std::vector<EventRule>& getOnReceived() const;

private:
    NodeTransport m_transport;
    Endpoint m_endpoint;
    FrameConfig m_frame;
    std::uint32_t m_maxConnections;
    std::uint32_t m_bufferSize;
    std::vector<EventRule> m_onReceived;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
