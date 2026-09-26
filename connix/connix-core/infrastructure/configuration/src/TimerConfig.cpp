#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

TimerConfig::TimerConfig(std::uint32_t interval, bool singleShot,
                         std::vector<std::string> onTimeout)
    : m_interval(interval)
    , m_singleShot(singleShot)
    , m_onTimeout(std::move(onTimeout))
{
}

std::uint32_t TimerConfig::getInterval() const
{
    return m_interval;
}

bool TimerConfig::isSingleShot() const
{
    return m_singleShot;
}

const std::vector<std::string>& TimerConfig::getOnTimeout() const
{
    return m_onTimeout;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
