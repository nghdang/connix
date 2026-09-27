#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

EventRule::EventRule(std::vector<std::string> patterns, std::uint32_t offset,
                     std::vector<std::string> actions)
    : m_patterns(std::move(patterns))
    , m_offset(offset)
    , m_actions(std::move(actions))
{
}

const std::vector<std::string>& EventRule::getPatterns() const
{
    return m_patterns;
}

std::uint32_t EventRule::getOffset() const
{
    return m_offset;
}

const std::vector<std::string>& EventRule::getActions() const
{
    return m_actions;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
