#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"

#include <string>
#include <unordered_map>
#include <utility>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ConnixConfig::ConnixConfig()
    : m_name()
    , m_nodes()
    , m_timers()
    , m_filesystems()
    , m_actions()
{
}

ConnixConfig::ConnixConfig(
    std::string name, std::unordered_map<std::string, NodeConfig> nodes,
    std::unordered_map<std::string, TimerConfig> timers,
    std::unordered_map<std::string, FilesystemConfig> filesystems,
    std::unordered_map<std::string, ActionConfig> actions)
    : m_name(std::move(name))
    , m_nodes(std::move(nodes))
    , m_timers(std::move(timers))
    , m_filesystems(std::move(filesystems))
    , m_actions(std::move(actions))
{
}

const std::string& ConnixConfig::getName() const
{
    return m_name;
}

const std::unordered_map<std::string, NodeConfig>&
ConnixConfig::getNodes() const
{
    return m_nodes;
}

const std::unordered_map<std::string, TimerConfig>&
ConnixConfig::getTimers() const
{
    return m_timers;
}

const std::unordered_map<std::string, FilesystemConfig>&
ConnixConfig::getFilesystems() const
{
    return m_filesystems;
}

const std::unordered_map<std::string, ActionConfig>&
ConnixConfig::getActions() const
{
    return m_actions;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
