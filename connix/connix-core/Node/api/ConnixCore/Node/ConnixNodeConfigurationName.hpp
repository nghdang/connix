#pragma once

#include <string>

namespace ConnixCore {
namespace Node {
namespace ConnixNodeConfigurationName {

static const std::string CLIENT_CONNECT_TIMEOUT{ "connectTimeout" };
static const std::string CLIENT_AUTO_RECONNECT{ "autoReconnect" };
static const std::string CLIENT_RECONNECT_INTERVAL{ "reconnectInterval" };
static const std::string CLIENT_BUFFER_SIZE{ "bufferSize" };

static const std::string SERVER_ADDRESS{ "address" };
static const std::string SERVER_PORT{ "port" };
static const std::string SERVER_MAX_CONNECTIONS{ "maxConnections" };
static const std::string SERVER_MAX_QUEUE_CONNECTIONS{ "maxQueueConnections" };
static const std::string SERVER_BUFFER_SIZE{ "bufferSize" };

}; // namespace ConnixNodeConfigurationName
} // namespace Node
} // namespace ConnixCore
