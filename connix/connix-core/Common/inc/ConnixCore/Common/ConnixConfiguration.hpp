#pragma once

#include "ConnixCore/Common/IConnixConfiguration.hpp"
#include <map>
#include <string>

namespace ConnixCore {
namespace Common {

class ConnixConfiguration : public ConnixCore::Common::IConnixConfiguration
{
public:
    explicit ConnixConfiguration() = default;

    ~ConnixConfiguration() override = default;

    void getConfig(const std::string& name, std::string& value) const override;

    void getConfig(const std::string& name,
                   std::uint8_t& value) const override;

    void getConfig(const std::string& name,
                   std::uint16_t& value) const override;

    void getConfig(const std::string& name,
                   std::uint32_t& value) const override;

    void getConfig(const std::string& name,
                   std::uint64_t& value) const override;

    void getConfig(const std::string& name, std::int8_t& value) const override;

    void getConfig(const std::string& name,
                   std::int16_t& value) const override;

    void getConfig(const std::string& name,
                   std::int32_t& value) const override;

    void getConfig(const std::string& name,
                   std::int64_t& value) const override;

    void getConfig(const std::string& name, bool& value) const override;

    void getConfig(const std::string& name, float& value) const override;

    void getConfig(const std::string& name, double& value) const override;

    void setConfig(const std::string& name, const std::string& value) override;

    void setConfig(const std::string& name, const std::uint8_t value) override;

    void setConfig(const std::string& name,
                   const std::uint16_t value) override;

    void setConfig(const std::string& name,
                   const std::uint32_t value) override;

    void setConfig(const std::string& name,
                   const std::uint64_t value) override;

    void setConfig(const std::string& name, const std::int8_t value) override;

    void setConfig(const std::string& name, const std::int16_t value) override;

    void setConfig(const std::string& name, const std::int32_t value) override;

    void setConfig(const std::string& name, const std::int64_t value) override;

    void setConfig(const std::string& name, const bool value) override;

    void setConfig(const std::string& name, const float value) override;

    void setConfig(const std::string& name, const double value) override;

private:
    std::map<std::string, std::string> m_stringConfigs;

    std::map<std::string, std::uint8_t> m_uint8Configs;

    std::map<std::string, std::uint16_t> m_uint16Configs;

    std::map<std::string, std::uint32_t> m_uint32Configs;

    std::map<std::string, std::uint64_t> m_uint64Configs;

    std::map<std::string, std::int8_t> m_int8Configs;

    std::map<std::string, std::int16_t> m_int16Configs;

    std::map<std::string, std::int32_t> m_int32Configs;

    std::map<std::string, std::int64_t> m_int64Configs;

    std::map<std::string, bool> m_boolConfigs;

    std::map<std::string, float> m_floatConfigs;

    std::map<std::string, double> m_doubleConfigs;
};

} // namespace Common
} // namespace ConnixCore
