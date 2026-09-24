#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ActionConfig
{
public:
    ActionConfig(ActionType type, std::optional<std::string> bytes,
                 std::optional<std::string> sourceNode,
                 std::optional<std::string> targetNode,
                 std::optional<std::uint32_t> executionDelay,
                 std::optional<std::uint32_t> executionTimeout,
                 std::optional<std::uint32_t> maxPending,
                 std::optional<std::string> file);

    ActionType getType() const;
    const std::optional<std::string>& getBytes() const;
    const std::optional<std::string>& getSourceNode() const;
    const std::optional<std::string>& getTargetNode() const;
    const std::optional<std::uint32_t>& getExecutionDelay() const;
    const std::optional<std::uint32_t>& getExecutionTimeout() const;
    const std::optional<std::uint32_t>& getMaxPending() const;
    const std::optional<std::string>& getFile() const;

private:
    ActionType m_type;
    std::optional<std::string> m_bytes;
    std::optional<std::string> m_sourceNode;
    std::optional<std::string> m_targetNode;
    std::optional<std::uint32_t> m_executionDelay;
    std::optional<std::uint32_t> m_executionTimeout;
    std::optional<std::uint32_t> m_maxPending;
    std::optional<std::string> m_file;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
