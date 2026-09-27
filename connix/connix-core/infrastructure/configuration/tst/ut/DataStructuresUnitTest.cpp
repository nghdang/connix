#include "gtest/gtest.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
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

using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

TEST(DataStructuresTest, Endpoint)
{
    const Endpoint endpoint("127.0.0.1", 8080);
    EXPECT_EQ(endpoint.getAddress(), "127.0.0.1");
    ASSERT_TRUE(endpoint.getPort().has_value());
    EXPECT_EQ(endpoint.getPort().value_or(0), 8080);

    const Endpoint endpointNoPort("localhost", std::nullopt);
    EXPECT_EQ(endpointNoPort.getAddress(), "localhost");
    EXPECT_FALSE(endpointNoPort.getPort().has_value());
}

TEST(DataStructuresTest, FrameConfig)
{
    const FrameConfig frame(FrameType::LENGTH_PREFIX, 1024, 0, 4,
                            ByteOrder::BIG_ENDIAN);
    EXPECT_EQ(frame.getType(), FrameType::LENGTH_PREFIX);
    EXPECT_EQ(frame.getSize(), 1024);
    EXPECT_EQ(frame.getLengthOffset(), 0);
    EXPECT_EQ(frame.getLengthSize(), 4);
    EXPECT_EQ(frame.getByteOrder(), ByteOrder::BIG_ENDIAN);

    const FrameConfig defaultFrame;
    EXPECT_EQ(defaultFrame.getType(), FrameType::NONE);
    EXPECT_EQ(defaultFrame.getSize(), 0);
    EXPECT_EQ(defaultFrame.getLengthOffset(), 0);
    EXPECT_EQ(defaultFrame.getLengthSize(), 0);
    EXPECT_EQ(defaultFrame.getByteOrder(), ByteOrder::BIG_ENDIAN);
}

TEST(DataStructuresTest, EventRule)
{
    const std::vector<std::string> patterns = { "CMD", "DATA" };
    const std::vector<std::string> actions = { "ACTION1", "ACTION2" };
    const EventRule rule(patterns, 8, actions);

    EXPECT_EQ(rule.getPatterns(), patterns);
    EXPECT_EQ(rule.getOffset(), 8);
    EXPECT_EQ(rule.getActions(), actions);
}

TEST(DataStructuresTest, ServerNodeConfig)
{
    const Endpoint endpoint("127.0.0.1", 9000);
    const FrameConfig frame(FrameType::FIXED_SIZE, 512, 0, 0,
                            ByteOrder::BIG_ENDIAN);
    const EventRule rule({ "P1" }, 0, { "A1" });

    const ServerNodeConfig node(NodeTransport::TCP, endpoint, frame, 100, 4096,
                                { rule });

    EXPECT_EQ(node.getTransport(), NodeTransport::TCP);
    EXPECT_EQ(node.getEndpoint().getAddress(), "127.0.0.1");
    EXPECT_EQ(node.getFrame().getType(), FrameType::FIXED_SIZE);
    EXPECT_EQ(node.getMaxConnections(), 100);
    EXPECT_EQ(node.getBufferSize(), 4096);
    ASSERT_EQ(node.getOnReceived().size(), 1);
    EXPECT_EQ(node.getOnReceived()[0].getOffset(), 0);
}

TEST(DataStructuresTest, ClientNodeConfig)
{
    const FrameConfig frame(FrameType::FIXED_SIZE, 512, 0, 0,
                            ByteOrder::BIG_ENDIAN);
    const EventRule rule({ "P1" }, 0, { "A1" });

    const ClientNodeConfig node(NodeTransport::TCP, frame, 4096, 5000, 10000,
                                { rule });

    EXPECT_EQ(node.getTransport(), NodeTransport::TCP);
    EXPECT_EQ(node.getFrame().getType(), FrameType::FIXED_SIZE);
    EXPECT_EQ(node.getBufferSize(), 4096);
    EXPECT_EQ(node.getConnectionTimeout(), 5000);
    EXPECT_EQ(node.getIdleTimeout(), 10000);
    ASSERT_EQ(node.getOnReceived().size(), 1);
    EXPECT_EQ(node.getOnReceived()[0].getOffset(), 0);
}

TEST(DataStructuresTest, PeerNodeConfig)
{
    const Endpoint endpoint("127.0.0.1", 9000);
    const FrameConfig frame(FrameType::FIXED_SIZE, 512, 0, 0,
                            ByteOrder::BIG_ENDIAN);

    const PeerNodeConfig node(NodeTransport::TCP, endpoint, frame);

    EXPECT_EQ(node.getTransport(), NodeTransport::TCP);
    EXPECT_EQ(node.getEndpoint().getAddress(), "127.0.0.1");
    EXPECT_EQ(node.getFrame().getType(), FrameType::FIXED_SIZE);
}

TEST(DataStructuresTest, TimerConfig)
{
    const std::vector<std::string> timeouts = { "TRIGGER_ACTION" };
    const TimerConfig timer(1000, true, timeouts);

    EXPECT_EQ(timer.getInterval(), 1000);
    EXPECT_TRUE(timer.isSingleShot());
    EXPECT_EQ(timer.getOnTimeout(), timeouts);
}

TEST(DataStructuresTest, FilesystemConfig)
{
    const std::vector<std::string> modified = { "TRIGGER_ACTION" };
    const FilesystemConfig fs("/tmp/watch", 500, modified);

    EXPECT_EQ(fs.getPath(), "/tmp/watch");
    EXPECT_EQ(fs.getDebounce(), 500);
    EXPECT_EQ(fs.getOnModified(), modified);
}

TEST(DataStructuresTest, ActionPayload)
{
    const ActionPayload payload(PayloadType::BYTES, "01 02 03");
    EXPECT_EQ(payload.getType(), PayloadType::BYTES);
    EXPECT_EQ(payload.getSource(), "01 02 03");
}

TEST(DataStructuresTest, ActionConfig)
{
    const ActionPayload payload(PayloadType::BYTES, "01 02 03");
    const ActionConfig action(ActionType::SEND, payload, "sourceNode",
                              "targetNode", 100, 500, 10);

    EXPECT_EQ(action.getType(), ActionType::SEND);
    EXPECT_EQ(action.getPayload().getType(), PayloadType::BYTES);
    EXPECT_EQ(action.getPayload().getSource(), "01 02 03");
    EXPECT_EQ(action.getSourceNode(), "sourceNode");
    EXPECT_EQ(action.getTargetNode(), "targetNode");
    EXPECT_EQ(action.getExecutionDelay(), 100);
    EXPECT_EQ(action.getExecutionTimeout(), 500);
    EXPECT_EQ(action.getMaxPending(), 10);
}

TEST(DataStructuresTest, ActionConfigDefaultValuesAndFilePayload)
{
    const ActionPayload payload(PayloadType::FILE, "/tmp/out");
    const ActionConfig action(ActionType::RESPOND, payload);

    EXPECT_EQ(action.getType(), ActionType::RESPOND);
    EXPECT_EQ(action.getPayload().getType(), PayloadType::FILE);
    EXPECT_EQ(action.getPayload().getSource(), "/tmp/out");
    EXPECT_EQ(action.getSourceNode(), "");
    EXPECT_EQ(action.getTargetNode(), "");
    EXPECT_EQ(action.getExecutionDelay(),
              ActionConfig::DEFAULT_EXECUTION_DELAY);
    EXPECT_EQ(action.getExecutionTimeout(),
              ActionConfig::DEFAULT_EXECUTION_TIMEOUT);
    EXPECT_EQ(action.getMaxPending(), ActionConfig::DEFAULT_MAX_PENDING);
}

TEST(DataStructuresTest, ConnixConfig)
{
    std::unordered_map<std::string, ServerNodeConfig> serverNodes;
    std::unordered_map<std::string, ClientNodeConfig> clientNodes;
    std::unordered_map<std::string, PeerNodeConfig> peerNodes;
    std::unordered_map<std::string, TimerConfig> timers;
    std::unordered_map<std::string, FilesystemConfig> filesystems;
    std::unordered_map<std::string, ActionConfig> actions;

    serverNodes.emplace("serverNode",
                        ServerNodeConfig(NodeTransport::TCP,
                                         Endpoint("127.0.0.1", 8080),
                                         FrameConfig(), 10, 4096, {}));
    clientNodes.emplace("clientNode",
                        ClientNodeConfig(NodeTransport::TCP, FrameConfig(),
                                         4096, 5000, 30000, {}));
    peerNodes.emplace("peerNode", PeerNodeConfig(NodeTransport::TCP,
                                                 Endpoint("127.0.0.1", 8080),
                                                 FrameConfig()));

    timers.emplace("t1", TimerConfig(500, false, {}));
    filesystems.emplace("fs1", FilesystemConfig("/etc/config", 100, {}));
    actions.emplace("act1",
                    ActionConfig(ActionType::RESPOND,
                                 ActionPayload(PayloadType::BYTES, "hello")));

    const ConnixConfig config("MainConfig", serverNodes, clientNodes,
                              peerNodes, timers, filesystems, actions);

    EXPECT_EQ(config.getName(), "MainConfig");
    EXPECT_EQ(config.getServerNodes().size(), 1);
    EXPECT_EQ(config.getClientNodes().size(), 1);
    EXPECT_EQ(config.getPeerNodes().size(), 1);
    EXPECT_EQ(config.getTimers().size(), 1);
    EXPECT_EQ(config.getFilesystems().size(), 1);
    EXPECT_EQ(config.getActions().size(), 1);
}

TEST(DataStructuresTest, ConnixConfigDefaultConstructor)
{
    const ConnixConfig config;

    EXPECT_EQ(config.getName(), "");
    EXPECT_TRUE(config.getServerNodes().empty());
    EXPECT_TRUE(config.getClientNodes().empty());
    EXPECT_TRUE(config.getPeerNodes().empty());
    EXPECT_TRUE(config.getTimers().empty());
    EXPECT_TRUE(config.getFilesystems().empty());
    EXPECT_TRUE(config.getActions().empty());
}

} // namespace UnitTest
} // namespace ConnixCore
