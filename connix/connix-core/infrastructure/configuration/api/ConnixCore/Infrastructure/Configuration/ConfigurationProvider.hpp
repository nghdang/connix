#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class IFileReader;
class IJsonValidator;
class IJsonParser;

class ConfigurationProvider : public IConfigurationProvider
{
public:
    ConfigurationProvider(const IFileReader& fileReader,
                          const IJsonValidator& jsonValidator,
                          const IJsonParser& jsonParser);
    ~ConfigurationProvider() override = default;

    void load(const std::string& configPath,
              const std::string& schemaPath) override;

    const std::string& getName() const override;

    const std::unordered_map<std::string, NodeConfig>&
    getNodes() const override;

    const std::unordered_map<std::string, TimerConfig>&
    getTimers() const override;

    const std::unordered_map<std::string, FilesystemConfig>&
    getFilesystems() const override;

    const std::unordered_map<std::string, ActionConfig>&
    getActions() const override;

private:
    std::reference_wrapper<const IFileReader> m_fileReader;
    std::reference_wrapper<const IJsonValidator> m_jsonValidator;
    std::reference_wrapper<const IJsonParser> m_jsonParser;
    std::string m_name;
    std::unordered_map<std::string, NodeConfig> m_nodes;
    std::unordered_map<std::string, TimerConfig> m_timers;
    std::unordered_map<std::string, FilesystemConfig> m_filesystems;
    std::unordered_map<std::string, ActionConfig> m_actions;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
