#pragma once

#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class IFileReader
{
public:
    virtual ~IFileReader() = default;

    virtual std::string readAll(const std::string& filePath) const = 0;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
