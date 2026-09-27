#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"

#include <cstdint>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ServerNodeConfig::ServerNodeConfig(NodeTransport transport, Endpoint endpoint,
                                   FrameConfig frame,
                                   std::uint32_t maxConnections,
                                   std::uint32_t bufferSize,
                                   std::vector<EventRule> onReceived)
    : m_transport(transport)
    , m_endpoint(std::move(endpoint))
    , m_frame(frame)
    , m_maxConnections(maxConnections)
    , m_bufferSize(bufferSize)
    , m_onReceived(std::move(onReceived))
{
}

NodeTransport ServerNodeConfig::getTransport() const
{
    return m_transport;
}

const Endpoint& ServerNodeConfig::getEndpoint() const
{
    return m_endpoint;
}

const FrameConfig& ServerNodeConfig::getFrame() const
{
    return m_frame;
}

std::uint32_t ServerNodeConfig::getMaxConnections() const
{
    return m_maxConnections;
}

std::uint32_t ServerNodeConfig::getBufferSize() const
{
    return m_bufferSize;
}

const std::vector<EventRule>& ServerNodeConfig::getOnReceived() const
{
    return m_onReceived;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
