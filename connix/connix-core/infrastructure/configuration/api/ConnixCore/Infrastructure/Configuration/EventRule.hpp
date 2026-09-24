#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class EventRule
{
public:
    EventRule(std::vector<std::string> patterns, std::uint32_t offset,
              std::vector<std::string> actions);

    const std::vector<std::string>& getPatterns() const;
    std::uint32_t getOffset() const;
    const std::vector<std::string>& getActions() const;

private:
    std::vector<std::string> m_patterns;
    std::uint32_t m_offset;
    std::vector<std::string> m_actions;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
