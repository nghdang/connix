#include "ConnixCore/Infrastructure/Configuration/JsonParser.hpp"

#include <cstdint>
#include <exception>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

namespace {

using json = nlohmann::json;

NodeTransport parseTransport(const std::string& transportStr)
{
    if (transportStr == "TCP")
    {
        return NodeTransport::TCP;
    }
    if (transportStr == "UDP")
    {
        return NodeTransport::UDP;
    }
    if (transportStr == "UDS_STREAM")
    {
        return NodeTransport::UDS_STREAM;
    }
    if (transportStr == "UDS_DATAGRAM")
    {
        return NodeTransport::UDS_DATAGRAM;
    }
    throw ConfigurationException(ConfigurationErrorCode::JSON_PARSE_FAILED,
                                 "Unknown transport: " + transportStr);
}

Endpoint parseEndpoint(const json& nodeJson)
{
    std::string address = nodeJson.at("address").get<std::string>();
    std::optional<std::uint16_t> port = std::nullopt;
    if (nodeJson.contains("port") && !nodeJson.at("port").is_null())
    {
        port = nodeJson.at("port").get<std::uint16_t>();
    }
    return { std::move(address), port };
}

FrameConfig parseFrame(const json& nodeJson)
{
    const std::string typeStr = nodeJson.at("type").get<std::string>();
    if (typeStr == "NONE")
    {
        return { FrameType::NONE, 0, 0, 0, ByteOrder::BIG_ENDIAN };
    }
    if (typeStr == "FIXED_SIZE")
    {
        const std::uint32_t size = nodeJson.at("size").get<std::uint32_t>();
        return { FrameType::FIXED_SIZE, size, 0, 0, ByteOrder::BIG_ENDIAN };
    }
    if (typeStr == "LENGTH_PREFIX")
    {
        const std::uint32_t lengthOffset =
            nodeJson.at("lengthOffset").get<std::uint32_t>();
        const std::uint32_t lengthSize =
            nodeJson.at("lengthSize").get<std::uint32_t>();
        const std::string byteOrderStr =
            nodeJson.at("byteOrder").get<std::string>();
        const ByteOrder byteOrder = (byteOrderStr == "LITTLE_ENDIAN")
                                        ? ByteOrder::LITTLE_ENDIAN
                                        : ByteOrder::BIG_ENDIAN;
        return { FrameType::LENGTH_PREFIX, 0, lengthOffset, lengthSize,
                 byteOrder };
    }
    throw ConfigurationException(ConfigurationErrorCode::JSON_PARSE_FAILED,
                                 "Unknown frame type: " + typeStr);
}

std::vector<EventRule> parseEventRules(const json& nodeJson)
{
    std::vector<EventRule> rules;
    for (const auto& ruleJson : nodeJson)
    {
        std::vector<std::string> patterns =
            ruleJson.at("patterns").get<std::vector<std::string>>();
        const std::uint32_t offset =
            ruleJson.at("offset").get<std::uint32_t>();
        std::vector<std::string> actions =
            ruleJson.at("actions").get<std::vector<std::string>>();
        rules.emplace_back(std::move(patterns), offset, std::move(actions));
    }
    return rules;
}

ServerNodeConfig parseServerNode(const json& nodeJson)
{
    const NodeTransport transport =
        parseTransport(nodeJson.at("transport").get<std::string>());
    Endpoint endpoint = parseEndpoint(nodeJson.at("endpoint"));
    const FrameConfig frame = parseFrame(nodeJson.at("frame"));

    const std::uint32_t maxConnections = nodeJson.value("maxConnections", 0U);
    const std::uint32_t bufferSize = nodeJson.value("bufferSize", 4096U);
    std::vector<EventRule> onReceived;
    if (nodeJson.contains("onReceived"))
    {
        onReceived = parseEventRules(nodeJson.at("onReceived"));
    }

    return { transport,      std::move(endpoint), frame,
             maxConnections, bufferSize,          std::move(onReceived) };
}

ClientNodeConfig parseClientNode(const json& nodeJson)
{
    const NodeTransport transport =
        parseTransport(nodeJson.at("transport").get<std::string>());
    const FrameConfig frame = parseFrame(nodeJson.at("frame"));

    const std::uint32_t bufferSize = nodeJson.value("bufferSize", 4096U);
    const std::uint32_t connectionTimeout =
        nodeJson.value("connectionTimeout", 0U);
    const std::uint32_t idleTimeout = nodeJson.value("idleTimeout", 0U);
    std::vector<EventRule> onReceived;
    if (nodeJson.contains("onReceived"))
    {
        onReceived = parseEventRules(nodeJson.at("onReceived"));
    }

    return { transport,         frame,       bufferSize,
             connectionTimeout, idleTimeout, std::move(onReceived) };
}

PeerNodeConfig parsePeerNode(const json& nodeJson)
{
    const NodeTransport transport =
        parseTransport(nodeJson.at("transport").get<std::string>());
    Endpoint endpoint = parseEndpoint(nodeJson.at("endpoint"));
    const FrameConfig frame = parseFrame(nodeJson.at("frame"));

    return { transport, std::move(endpoint), frame };
}

TimerConfig parseTimer(const json& nodeJson)
{
    const std::uint32_t interval =
        nodeJson.at("interval").get<std::uint32_t>();
    const bool singleShot = nodeJson.value("singleShot", false);
    std::vector<std::string> onTimeout =
        nodeJson.at("onTimeout").get<std::vector<std::string>>();
    return { interval, singleShot, std::move(onTimeout) };
}

FilesystemConfig parseFilesystem(const json& nodeJson)
{
    std::string path = nodeJson.at("path").get<std::string>();
    const std::uint32_t debounce =
        nodeJson.at("debounce").get<std::uint32_t>();
    std::vector<std::string> onModified =
        nodeJson.at("onModified").get<std::vector<std::string>>();
    return { std::move(path), debounce, std::move(onModified) };
}

ActionConfig parseAction(const json& nodeJson)
{
    const std::string typeStr = nodeJson.at("type").get<std::string>();
    const ActionType type =
        (typeStr == "SEND") ? ActionType::SEND : ActionType::RESPOND;

    const auto& payloadJson = nodeJson.at("payload");
    const std::string payloadTypeStr =
        payloadJson.at("type").get<std::string>();
    const PayloadType payloadType =
        (payloadTypeStr == "FILE") ? PayloadType::FILE : PayloadType::BYTES;
    std::string payloadSource = payloadJson.at("source").get<std::string>();
    ActionPayload payload(payloadType, std::move(payloadSource));

    std::string sourceNode = nodeJson.value("sourceNode", "");
    std::string targetNode = nodeJson.value("targetNode", "");
    const std::uint32_t executionDelay = nodeJson.value(
        "executionDelay", ActionConfig::DEFAULT_EXECUTION_DELAY);
    const std::uint32_t executionTimeout = nodeJson.value(
        "executionTimeout", ActionConfig::DEFAULT_EXECUTION_TIMEOUT);
    const std::uint32_t maxPending =
        nodeJson.value("maxPending", ActionConfig::DEFAULT_MAX_PENDING);

    return { type,
             std::move(payload),
             std::move(sourceNode),
             std::move(targetNode),
             executionDelay,
             executionTimeout,
             maxPending };
}

} // namespace

ConnixConfig JsonParser::parse(const std::string& jsonStr) const
{
    try
    {
        const auto parsedJson = json::parse(jsonStr);

        std::string name = parsedJson.at("name").get<std::string>();

        std::unordered_map<std::string, ServerNodeConfig> serverNodes;
        std::unordered_map<std::string, ClientNodeConfig> clientNodes;
        std::unordered_map<std::string, PeerNodeConfig> peerNodes;

        if (parsedJson.contains("nodes"))
        {
            const auto& nodesJson = parsedJson.at("nodes");
            if (nodesJson.contains("serverNodes"))
            {
                for (const auto& [key, value] :
                     nodesJson.at("serverNodes").items())
                {
                    serverNodes.emplace(key, parseServerNode(value));
                }
            }
            if (nodesJson.contains("clientNodes"))
            {
                for (const auto& [key, value] :
                     nodesJson.at("clientNodes").items())
                {
                    clientNodes.emplace(key, parseClientNode(value));
                }
            }
            if (nodesJson.contains("peerNodes"))
            {
                for (const auto& [key, value] :
                     nodesJson.at("peerNodes").items())
                {
                    peerNodes.emplace(key, parsePeerNode(value));
                }
            }
        }

        std::unordered_map<std::string, TimerConfig> timers;
        if (parsedJson.contains("timers"))
        {
            for (const auto& [key, value] : parsedJson.at("timers").items())
            {
                timers.emplace(key, parseTimer(value));
            }
        }

        std::unordered_map<std::string, FilesystemConfig> filesystems;
        if (parsedJson.contains("filesystems"))
        {
            for (const auto& [key, value] :
                 parsedJson.at("filesystems").items())
            {
                filesystems.emplace(key, parseFilesystem(value));
            }
        }

        std::unordered_map<std::string, ActionConfig> actions;
        if (parsedJson.contains("actions"))
        {
            for (const auto& [key, value] : parsedJson.at("actions").items())
            {
                actions.emplace(key, parseAction(value));
            }
        }

        return { std::move(name),        std::move(serverNodes),
                 std::move(clientNodes), std::move(peerNodes),
                 std::move(timers),      std::move(filesystems),
                 std::move(actions) };
    } catch (const ConfigurationException&)
    {
        throw;
    } catch (const std::exception& ex)
    {
        throw ConfigurationException(ConfigurationErrorCode::JSON_PARSE_FAILED,
                                     ex.what());
    }
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
