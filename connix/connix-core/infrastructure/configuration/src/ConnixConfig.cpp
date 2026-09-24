#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"

#include <string>
#include <unordered_map>
#include <utility>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ConnixConfig::ConnixConfig()
    : m_name()
    , m_serverNodes()
    , m_clientNodes()
    , m_peerNodes()
    , m_timers()
    , m_filesystems()
    , m_actions()
{
}

ConnixConfig::ConnixConfig(
    std::string name,
    std::unordered_map<std::string, ServerNodeConfig> serverNodes,
    std::unordered_map<std::string, ClientNodeConfig> clientNodes,
    std::unordered_map<std::string, PeerNodeConfig> peerNodes,
    std::unordered_map<std::string, TimerConfig> timers,
    std::unordered_map<std::string, FilesystemConfig> filesystems,
    std::unordered_map<std::string, ActionConfig> actions)
    : m_name(std::move(name))
    , m_serverNodes(std::move(serverNodes))
    , m_clientNodes(std::move(clientNodes))
    , m_peerNodes(std::move(peerNodes))
    , m_timers(std::move(timers))
    , m_filesystems(std::move(filesystems))
    , m_actions(std::move(actions))
{
}

const std::string& ConnixConfig::getName() const
{
    return m_name;
}

const std::unordered_map<std::string, ServerNodeConfig>&
ConnixConfig::getServerNodes() const
{
    return m_serverNodes;
}

const std::unordered_map<std::string, ClientNodeConfig>&
ConnixConfig::getClientNodes() const
{
    return m_clientNodes;
}

const std::unordered_map<std::string, PeerNodeConfig>&
ConnixConfig::getPeerNodes() const
{
    return m_peerNodes;
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
