#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

FilesystemConfig::FilesystemConfig(std::string path, std::uint32_t debounce,
                                   std::vector<std::string> onModified)
    : m_path(std::move(path))
    , m_debounce(debounce)
    , m_onModified(std::move(onModified))
{
}

const std::string& FilesystemConfig::getPath() const
{
    return m_path;
}

std::uint32_t FilesystemConfig::getDebounce() const
{
    return m_debounce;
}

const std::vector<std::string>& FilesystemConfig::getOnModified() const
{
    return m_onModified;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
