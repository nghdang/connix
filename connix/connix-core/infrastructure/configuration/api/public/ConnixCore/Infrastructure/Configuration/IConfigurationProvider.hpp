#pragma once

#include <string>
#include <unordered_map>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ServerNodeConfig;
class ClientNodeConfig;
class PeerNodeConfig;
class TimerConfig;
class FilesystemConfig;
class ActionConfig;

class IConfigurationProvider
{
public:
    virtual ~IConfigurationProvider() = default;

    virtual void load(const std::string& configPath,
                      const std::string& schemaPath) = 0;

    virtual const std::string& getName() const = 0;

    virtual const std::unordered_map<std::string, ServerNodeConfig>&
    getServerNodes() const = 0;

    virtual const std::unordered_map<std::string, ClientNodeConfig>&
    getClientNodes() const = 0;

    virtual const std::unordered_map<std::string, PeerNodeConfig>&
    getPeerNodes() const = 0;

    virtual const std::unordered_map<std::string, TimerConfig>&
    getTimers() const = 0;

    virtual const std::unordered_map<std::string, FilesystemConfig>&
    getFilesystems() const = 0;

    virtual const std::unordered_map<std::string, ActionConfig>&
    getActions() const = 0;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
