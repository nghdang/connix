#pragma once

#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class IJsonValidator
{
public:
    virtual ~IJsonValidator() = default;

    virtual void validate(const std::string& jsonStr,
                          const std::string& schemaStr) const = 0;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
