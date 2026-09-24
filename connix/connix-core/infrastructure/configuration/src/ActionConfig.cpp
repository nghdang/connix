#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ActionConfig::ActionConfig(ActionType type, std::optional<std::string> bytes,
                           std::optional<std::string> sourceNode,
                           std::optional<std::string> targetNode,
                           std::optional<std::uint32_t> executionDelay,
                           std::optional<std::uint32_t> executionTimeout,
                           std::optional<std::uint32_t> maxPending,
                           std::optional<std::string> file)
    : m_type(type)
    , m_bytes(std::move(bytes))
    , m_sourceNode(std::move(sourceNode))
    , m_targetNode(std::move(targetNode))
    , m_executionDelay(executionDelay)
    , m_executionTimeout(executionTimeout)
    , m_maxPending(maxPending)
    , m_file(std::move(file))
{
}

ActionType ActionConfig::getType() const
{
    return m_type;
}

const std::optional<std::string>& ActionConfig::getBytes() const
{
    return m_bytes;
}

const std::optional<std::string>& ActionConfig::getSourceNode() const
{
    return m_sourceNode;
}

const std::optional<std::string>& ActionConfig::getTargetNode() const
{
    return m_targetNode;
}

const std::optional<std::uint32_t>& ActionConfig::getExecutionDelay() const
{
    return m_executionDelay;
}

const std::optional<std::uint32_t>& ActionConfig::getExecutionTimeout() const
{
    return m_executionTimeout;
}

const std::optional<std::uint32_t>& ActionConfig::getMaxPending() const
{
    return m_maxPending;
}

const std::optional<std::string>& ActionConfig::getFile() const
{
    return m_file;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
