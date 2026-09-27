#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class ConfigurationErrorCode : std::uint8_t {
    FILE_NOT_FOUND,
    FILE_READ_FAILED,
    SCHEMA_VALIDATION_FAILED,
    JSON_PARSE_FAILED
};

class ConfigurationException : public std::runtime_error
{
public:
    ConfigurationException(ConfigurationErrorCode errorCode,
                           const std::string& message)
        : std::runtime_error(message)
        , m_errorCode(errorCode)
    {
    }

    ConfigurationErrorCode getErrorCode() const noexcept
    {
        return m_errorCode;
    }

private:
    ConfigurationErrorCode m_errorCode;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
