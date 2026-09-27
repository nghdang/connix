#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class TimerConfig
{
public:
    TimerConfig(std::uint32_t interval, bool singleShot,
                std::vector<std::string> onTimeout);

    std::uint32_t getInterval() const;
    bool isSingleShot() const;
    const std::vector<std::string>& getOnTimeout() const;

private:
    std::uint32_t m_interval;
    bool m_singleShot;
    std::vector<std::string> m_onTimeout;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
