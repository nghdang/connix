#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class NodeConfig
{
public:
    NodeConfig(NodeType type, NodeTransport transport,
               std::optional<Endpoint> endpoint,
               std::optional<FrameConfig> frame,
               std::optional<std::uint32_t> maxConnections,
               std::optional<std::uint32_t> bufferSize,
               std::optional<std::uint32_t> connectionTimeout,
               std::optional<std::uint32_t> idleTimeout,
               std::vector<EventRule> onReceived);

    NodeType getType() const;
    NodeTransport getTransport() const;
    const std::optional<Endpoint>& getEndpoint() const;
    const std::optional<FrameConfig>& getFrame() const;
    const std::optional<std::uint32_t>& getMaxConnections() const;
    const std::optional<std::uint32_t>& getBufferSize() const;
    const std::optional<std::uint32_t>& getConnectionTimeout() const;
    const std::optional<std::uint32_t>& getIdleTimeout() const;
    const std::vector<EventRule>& getOnReceived() const;

private:
    NodeType m_type;
    NodeTransport m_transport;
    std::optional<Endpoint> m_endpoint;
    std::optional<FrameConfig> m_frame;
    std::optional<std::uint32_t> m_maxConnections;
    std::optional<std::uint32_t> m_bufferSize;
    std::optional<std::uint32_t> m_connectionTimeout;
    std::optional<std::uint32_t> m_idleTimeout;
    std::vector<EventRule> m_onReceived;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
