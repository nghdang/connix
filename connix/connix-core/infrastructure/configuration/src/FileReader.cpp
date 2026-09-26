#include "ConnixCore/Infrastructure/Configuration/FileReader.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

std::string FileReader::readAll(const std::string& filePath) const
{
    if (!std::filesystem::exists(filePath))
    {
        throw ConfigurationException(ConfigurationErrorCode::FILE_NOT_FOUND,
                                     "File does not exist: " + filePath);
    }

    if (!std::filesystem::is_regular_file(filePath))
    {
        throw ConfigurationException(ConfigurationErrorCode::FILE_READ_FAILED,
                                     "Path is not a regular file: " +
                                         filePath);
    }

    std::ifstream fileStream(filePath);
    if (!fileStream.is_open())
    {
        throw ConfigurationException(ConfigurationErrorCode::FILE_READ_FAILED,
                                     "Failed to open file: " + filePath);
    }

    std::ostringstream stringStream;
    stringStream << fileStream.rdbuf();

    return stringStream.str();
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
