#pragma once

#include <cstdint>
#include <string>

#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ActionConfig
{
public:
    static constexpr std::uint32_t DEFAULT_EXECUTION_DELAY = 0U;
    static constexpr std::uint32_t DEFAULT_EXECUTION_TIMEOUT = 5000U;
    static constexpr std::uint32_t DEFAULT_MAX_PENDING = 100U;

    ActionConfig(ActionType type, ActionPayload payload,
                 std::string sourceNode = "", std::string targetNode = "",
                 std::uint32_t executionDelay = DEFAULT_EXECUTION_DELAY,
                 std::uint32_t executionTimeout = DEFAULT_EXECUTION_TIMEOUT,
                 std::uint32_t maxPending = DEFAULT_MAX_PENDING);

    ActionType getType() const;
    const ActionPayload& getPayload() const;
    const std::string& getSourceNode() const;
    const std::string& getTargetNode() const;
    std::uint32_t getExecutionDelay() const;
    std::uint32_t getExecutionTimeout() const;
    std::uint32_t getMaxPending() const;

private:
    ActionType m_type;
    ActionPayload m_payload;
    std::string m_sourceNode;
    std::string m_targetNode;
    std::uint32_t m_executionDelay;
    std::uint32_t m_executionTimeout;
    std::uint32_t m_maxPending;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
