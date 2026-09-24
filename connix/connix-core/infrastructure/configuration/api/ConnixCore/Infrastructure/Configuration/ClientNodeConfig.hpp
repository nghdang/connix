#pragma once

#include <cstdint>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ClientNodeConfig
{
public:
    ClientNodeConfig(NodeTransport transport, FrameConfig frame,
                     std::uint32_t bufferSize, std::uint32_t connectionTimeout,
                     std::uint32_t idleTimeout,
                     std::vector<EventRule> onReceived);

    NodeTransport getTransport() const;
    const FrameConfig& getFrame() const;
    std::uint32_t getBufferSize() const;
    std::uint32_t getConnectionTimeout() const;
    std::uint32_t getIdleTimeout() const;
    const std::vector<EventRule>& getOnReceived() const;

private:
    NodeTransport m_transport;
    FrameConfig m_frame;
    std::uint32_t m_bufferSize;
    std::uint32_t m_connectionTimeout;
    std::uint32_t m_idleTimeout;
    std::vector<EventRule> m_onReceived;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
