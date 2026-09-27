#pragma once

#include <string>

#include "ConnixCore/Infrastructure/Configuration/IFileReader.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class FileReader : public IFileReader
{
public:
    FileReader() = default;
    ~FileReader() override = default;

    std::string readAll(const std::string& filePath) const override;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
