#pragma once

#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ConnixConfig
{
public:
    ConnixConfig();
    ConnixConfig(std::string name,
                 std::unordered_map<std::string, ServerNodeConfig> serverNodes,
                 std::unordered_map<std::string, ClientNodeConfig> clientNodes,
                 std::unordered_map<std::string, PeerNodeConfig> peerNodes,
                 std::unordered_map<std::string, TimerConfig> timers,
                 std::unordered_map<std::string, FilesystemConfig> filesystems,
                 std::unordered_map<std::string, ActionConfig> actions);

    const std::string& getName() const;
    const std::unordered_map<std::string, ServerNodeConfig>&
    getServerNodes() const;
    const std::unordered_map<std::string, ClientNodeConfig>&
    getClientNodes() const;
    const std::unordered_map<std::string, PeerNodeConfig>&
    getPeerNodes() const;
    const std::unordered_map<std::string, TimerConfig>& getTimers() const;
    const std::unordered_map<std::string, FilesystemConfig>&
    getFilesystems() const;
    const std::unordered_map<std::string, ActionConfig>& getActions() const;

private:
    std::string m_name;
    std::unordered_map<std::string, ServerNodeConfig> m_serverNodes;
    std::unordered_map<std::string, ClientNodeConfig> m_clientNodes;
    std::unordered_map<std::string, PeerNodeConfig> m_peerNodes;
    std::unordered_map<std::string, TimerConfig> m_timers;
    std::unordered_map<std::string, FilesystemConfig> m_filesystems;
    std::unordered_map<std::string, ActionConfig> m_actions;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
