#pragma once

#include <cstdint>
#include <string>

namespace ConnixCore {
namespace Common {

class IConnixConfiguration
{
public:
    virtual ~IConnixConfiguration() = default;

    virtual void getConfig(const std::string& name,
                           std::string& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::uint8_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::uint16_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::uint32_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::uint64_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::int8_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::int16_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::int32_t& value) const = 0;

    virtual void getConfig(const std::string& name,
                           std::int64_t& value) const = 0;

    virtual void getConfig(const std::string& name, bool& value) const = 0;

    virtual void getConfig(const std::string& name, float& value) const = 0;

    virtual void getConfig(const std::string& name, double& value) const = 0;

    virtual void setConfig(const std::string& name,
                           const std::string& value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::uint8_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::uint16_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::uint32_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::uint64_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::int8_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::int16_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::int32_t value) = 0;

    virtual void setConfig(const std::string& name,
                           const std::int64_t value) = 0;

    virtual void setConfig(const std::string& name, const bool value) = 0;

    virtual void setConfig(const std::string& name, const float value) = 0;

    virtual void setConfig(const std::string& name, const double value) = 0;
};

} // namespace Common
} // namespace ConnixCore
