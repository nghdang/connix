#include "ConnixCore/Infrastructure/Configuration/ConfigurationProvider.hpp"

#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IFileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ConfigurationProvider::ConfigurationProvider(
    const IFileReader& fileReader, const IJsonValidator& jsonValidator,
    const IJsonParser& jsonParser)
    : m_fileReader(fileReader)
    , m_jsonValidator(jsonValidator)
    , m_jsonParser(jsonParser)
    , m_name()
    , m_nodes()
    , m_timers()
    , m_filesystems()
    , m_actions()
{
}

void ConfigurationProvider::load(const std::string& configPath,
                                 const std::string& schemaPath)
{
    const std::string jsonStr = m_fileReader.get().readAll(configPath);
    const std::string schemaStr = m_fileReader.get().readAll(schemaPath);
    m_jsonValidator.get().validate(jsonStr, schemaStr);
    const ConnixConfig parsedConfig = m_jsonParser.get().parse(jsonStr);
    m_name = parsedConfig.getName();
    m_nodes = parsedConfig.getNodes();
    m_timers = parsedConfig.getTimers();
    m_filesystems = parsedConfig.getFilesystems();
    m_actions = parsedConfig.getActions();
}

const std::string& ConfigurationProvider::getName() const
{
    return m_name;
}

const std::unordered_map<std::string, NodeConfig>&
ConfigurationProvider::getNodes() const
{
    return m_nodes;
}

const std::unordered_map<std::string, TimerConfig>&
ConfigurationProvider::getTimers() const
{
    return m_timers;
}

const std::unordered_map<std::string, FilesystemConfig>&
ConfigurationProvider::getFilesystems() const
{
    return m_filesystems;
}

const std::unordered_map<std::string, ActionConfig>&
ConfigurationProvider::getActions() const
{
    return m_actions;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
