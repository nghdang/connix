#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"

#include <cstdint>
#include <string>
#include <utility>

#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ActionConfig::ActionConfig(ActionType type, ActionPayload payload,
                           std::string sourceNode, std::string targetNode,
                           std::uint32_t executionDelay,
                           std::uint32_t executionTimeout,
                           std::uint32_t maxPending)
    : m_type(type)
    , m_payload(std::move(payload))
    , m_sourceNode(std::move(sourceNode))
    , m_targetNode(std::move(targetNode))
    , m_executionDelay(executionDelay)
    , m_executionTimeout(executionTimeout)
    , m_maxPending(maxPending)
{
}

ActionType ActionConfig::getType() const
{
    return m_type;
}

const ActionPayload& ActionConfig::getPayload() const
{
    return m_payload;
}

const std::string& ActionConfig::getSourceNode() const
{
    return m_sourceNode;
}

const std::string& ActionConfig::getTargetNode() const
{
    return m_targetNode;
}

std::uint32_t ActionConfig::getExecutionDelay() const
{
    return m_executionDelay;
}

std::uint32_t ActionConfig::getExecutionTimeout() const
{
    return m_executionTimeout;
}

std::uint32_t ActionConfig::getMaxPending() const
{
    return m_maxPending;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
