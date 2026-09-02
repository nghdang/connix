#include "ConnixCore/Common/ConnixConfiguration.hpp"

#include "ConnixCore/Common/Errors.hpp"

namespace ConnixCore {
namespace Common {

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::string& value) const
{
    if (m_stringConfigs.find(name) == m_stringConfigs.end())
    {
        throw ConnixCore::Common::KeyError("Config '" + name +
                                           "' does not exist");
    }

    value = m_stringConfigs.at(name);
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::uint8_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::uint16_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::uint32_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::uint64_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::int8_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::int16_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::int32_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    std::int64_t& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name, bool& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    float& value) const
{
}

void ConnixConfiguration::getConfig(const std::string& name,
                                    double& value) const
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::string& value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::uint8_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::uint16_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::uint32_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::uint64_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::int8_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::int16_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::int32_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const std::int64_t value)
{
}

void ConnixConfiguration::setConfig(const std::string& name, const bool value)
{
}

void ConnixConfiguration::setConfig(const std::string& name, const float value)
{
}

void ConnixConfiguration::setConfig(const std::string& name,
                                    const double value)
{
}
} // namespace Common
} // namespace ConnixCore
