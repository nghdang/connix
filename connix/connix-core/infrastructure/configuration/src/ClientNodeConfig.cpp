#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"

#include <cstdint>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ClientNodeConfig::ClientNodeConfig(NodeTransport transport, FrameConfig frame,
                                   std::uint32_t bufferSize,
                                   std::uint32_t connectionTimeout,
                                   std::uint32_t idleTimeout,
                                   std::vector<EventRule> onReceived)
    : m_transport(transport)
    , m_frame(frame)
    , m_bufferSize(bufferSize)
    , m_connectionTimeout(connectionTimeout)
    , m_idleTimeout(idleTimeout)
    , m_onReceived(std::move(onReceived))
{
}

NodeTransport ClientNodeConfig::getTransport() const
{
    return m_transport;
}

const FrameConfig& ClientNodeConfig::getFrame() const
{
    return m_frame;
}

std::uint32_t ClientNodeConfig::getBufferSize() const
{
    return m_bufferSize;
}

std::uint32_t ClientNodeConfig::getConnectionTimeout() const
{
    return m_connectionTimeout;
}

std::uint32_t ClientNodeConfig::getIdleTimeout() const
{
    return m_idleTimeout;
}

const std::vector<EventRule>& ClientNodeConfig::getOnReceived() const
{
    return m_onReceived;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
