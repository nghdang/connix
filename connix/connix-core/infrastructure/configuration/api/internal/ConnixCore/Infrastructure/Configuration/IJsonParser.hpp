#pragma once

#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ConnixConfig;

class IJsonParser
{
public:
    virtual ~IJsonParser() = default;

    virtual ConnixConfig parse(const std::string& jsonStr) const = 0;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
