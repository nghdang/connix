#include "ConnixCore/Infrastructure/Configuration/NodeConfig.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

NodeConfig::NodeConfig(NodeType type, NodeTransport transport,
                       std::optional<Endpoint> endpoint,
                       std::optional<FrameConfig> frame,
                       std::optional<std::uint32_t> maxConnections,
                       std::optional<std::uint32_t> bufferSize,
                       std::optional<std::uint32_t> connectionTimeout,
                       std::optional<std::uint32_t> idleTimeout,
                       std::vector<EventRule> onReceived)
    : m_type(type)
    , m_transport(transport)
    , m_endpoint(std::move(endpoint))
    , m_frame(frame)
    , m_maxConnections(maxConnections)
    , m_bufferSize(bufferSize)
    , m_connectionTimeout(connectionTimeout)
    , m_idleTimeout(idleTimeout)
    , m_onReceived(std::move(onReceived))
{
}

NodeType NodeConfig::getType() const
{
    return m_type;
}

NodeTransport NodeConfig::getTransport() const
{
    return m_transport;
}

const std::optional<Endpoint>& NodeConfig::getEndpoint() const
{
    return m_endpoint;
}

const std::optional<FrameConfig>& NodeConfig::getFrame() const
{
    return m_frame;
}

const std::optional<std::uint32_t>& NodeConfig::getMaxConnections() const
{
    return m_maxConnections;
}

const std::optional<std::uint32_t>& NodeConfig::getBufferSize() const
{
    return m_bufferSize;
}

const std::optional<std::uint32_t>& NodeConfig::getConnectionTimeout() const
{
    return m_connectionTimeout;
}

const std::optional<std::uint32_t>& NodeConfig::getIdleTimeout() const
{
    return m_idleTimeout;
}

const std::vector<EventRule>& NodeConfig::getOnReceived() const
{
    return m_onReceived;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
