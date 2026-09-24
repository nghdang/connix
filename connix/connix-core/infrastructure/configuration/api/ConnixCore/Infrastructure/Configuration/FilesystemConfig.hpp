#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class FilesystemConfig
{
public:
    FilesystemConfig(std::string path, std::uint32_t debounce,
                     std::vector<std::string> onModified);

    const std::string& getPath() const;
    std::uint32_t getDebounce() const;
    const std::vector<std::string>& getOnModified() const;

private:
    std::string m_path;
    std::uint32_t m_debounce;
    std::vector<std::string> m_onModified;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
