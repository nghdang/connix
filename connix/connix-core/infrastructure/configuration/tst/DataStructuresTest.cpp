#include "gtest/gtest.h"

#include <optional>
#include <string>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/EventRule.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeType.hpp"
#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

TEST(DataStructuresTest, Endpoint)
{
    Endpoint endpoint("127.0.0.1", 8080);
    EXPECT_EQ(endpoint.getAddress(), "127.0.0.1");
    ASSERT_TRUE(endpoint.getPort().has_value());
    EXPECT_EQ(endpoint.getPort().value(), 8080);

    Endpoint endpointNoPort("localhost", std::nullopt);
    EXPECT_EQ(endpointNoPort.getAddress(), "localhost");
    EXPECT_FALSE(endpointNoPort.getPort().has_value());
}

TEST(DataStructuresTest, FrameConfig)
{
    FrameConfig frame(FrameType::LENGTH_PREFIX, 1024, 0, 4,
                      ByteOrder::BIG_ENDIAN);
    EXPECT_EQ(frame.getType(), FrameType::LENGTH_PREFIX);
    ASSERT_TRUE(frame.getSize().has_value());
    EXPECT_EQ(frame.getSize().value(), 1024);
    ASSERT_TRUE(frame.getLengthOffset().has_value());
    EXPECT_EQ(frame.getLengthOffset().value(), 0);
    ASSERT_TRUE(frame.getLengthSize().has_value());
    EXPECT_EQ(frame.getLengthSize().value(), 4);
    ASSERT_TRUE(frame.getByteOrder().has_value());
    EXPECT_EQ(frame.getByteOrder().value(), ByteOrder::BIG_ENDIAN);
}

TEST(DataStructuresTest, EventRule)
{
    std::vector<std::string> patterns = { "CMD", "DATA" };
    std::vector<std::string> actions = { "ACTION1", "ACTION2" };
    EventRule rule(patterns, 8, actions);

    EXPECT_EQ(rule.getPatterns(), patterns);
    EXPECT_EQ(rule.getOffset(), 8);
    EXPECT_EQ(rule.getActions(), actions);
}

TEST(DataStructuresTest, NodeConfig)
{
    Endpoint endpoint("127.0.0.1", 9000);
    FrameConfig frame(FrameType::FIXED_SIZE, 512, std::nullopt, std::nullopt,
                      std::nullopt);
    EventRule rule({ "P1" }, 0, { "A1" });

    NodeConfig node(NodeType::SERVER, NodeTransport::TCP, endpoint, frame, 100,
                    4096, 5000, 10000, { rule });

    EXPECT_EQ(node.getType(), NodeType::SERVER);
    EXPECT_EQ(node.getTransport(), NodeTransport::TCP);
    ASSERT_TRUE(node.getEndpoint().has_value());
    EXPECT_EQ(node.getEndpoint()->getAddress(), "127.0.0.1");
    ASSERT_TRUE(node.getFrame().has_value());
    EXPECT_EQ(node.getFrame()->getType(), FrameType::FIXED_SIZE);
    ASSERT_TRUE(node.getMaxConnections().has_value());
    EXPECT_EQ(node.getMaxConnections().value(), 100);
    ASSERT_TRUE(node.getBufferSize().has_value());
    EXPECT_EQ(node.getBufferSize().value(), 4096);
    ASSERT_TRUE(node.getConnectionTimeout().has_value());
    EXPECT_EQ(node.getConnectionTimeout().value(), 5000);
    ASSERT_TRUE(node.getIdleTimeout().has_value());
    EXPECT_EQ(node.getIdleTimeout().value(), 10000);
    ASSERT_EQ(node.getOnReceived().size(), 1);
    EXPECT_EQ(node.getOnReceived()[0].getOffset(), 0);
}

TEST(DataStructuresTest, TimerConfig)
{
    std::vector<std::string> timeouts = { "TRIGGER_ACTION" };
    TimerConfig timer(1000, true, timeouts);

    EXPECT_EQ(timer.getInterval(), 1000);
    EXPECT_TRUE(timer.isSingleShot());
    EXPECT_EQ(timer.getOnTimeout(), timeouts);
}

TEST(DataStructuresTest, FilesystemConfig)
{
    std::vector<std::string> modified = { "RELOAD_ACTION" };
    FilesystemConfig fs("/tmp/watch", 250, modified);

    EXPECT_EQ(fs.getPath(), "/tmp/watch");
    EXPECT_EQ(fs.getDebounce(), 250);
    EXPECT_EQ(fs.getOnModified(), modified);
}

TEST(DataStructuresTest, ActionPayload)
{
    ActionPayload payload(PayloadType::BYTES, "12 34 56 78");
    EXPECT_EQ(payload.getType(), PayloadType::BYTES);
    EXPECT_EQ(payload.getSource(), "12 34 56 78");

    ActionPayload filePayload(PayloadType::FILE, "/tmp/data.bin");
    EXPECT_EQ(filePayload.getType(), PayloadType::FILE);
    EXPECT_EQ(filePayload.getSource(), "/tmp/data.bin");
}

TEST(DataStructuresTest, ActionConfigCustomValues)
{
    ActionPayload payload(PayloadType::BYTES, "hello");
    ActionConfig action(ActionType::SEND, payload, "nodeA", "nodeB", 100, 500,
                        10);

    EXPECT_EQ(action.getType(), ActionType::SEND);
    EXPECT_EQ(action.getPayload().getType(), PayloadType::BYTES);
    EXPECT_EQ(action.getPayload().getSource(), "hello");
    EXPECT_EQ(action.getSourceNode(), "nodeA");
    EXPECT_EQ(action.getTargetNode(), "nodeB");
    EXPECT_EQ(action.getExecutionDelay(), 100);
    EXPECT_EQ(action.getExecutionTimeout(), 500);
    EXPECT_EQ(action.getMaxPending(), 10);
}

TEST(DataStructuresTest, ActionConfigDefaultValuesAndFilePayload)
{
    ActionPayload payload(PayloadType::FILE, "/tmp/out");
    ActionConfig action(ActionType::RESPOND, payload);

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
    std::unordered_map<std::string, NodeConfig> nodes;
    std::unordered_map<std::string, TimerConfig> timers;
    std::unordered_map<std::string, FilesystemConfig> filesystems;
    std::unordered_map<std::string, ActionConfig> actions;

    nodes.emplace("serverNode",
                  NodeConfig(NodeType::SERVER, NodeTransport::TCP,
                             std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt, std::nullopt, std::nullopt, {}));
    timers.emplace("t1", TimerConfig(500, false, {}));
    filesystems.emplace("fs1", FilesystemConfig("/etc/config", 100, {}));
    actions.emplace("act1",
                    ActionConfig(ActionType::RESPOND,
                                 ActionPayload(PayloadType::BYTES, "hello")));

    ConnixConfig config("MainConfig", nodes, timers, filesystems, actions);

    EXPECT_EQ(config.getName(), "MainConfig");
    EXPECT_EQ(config.getNodes().size(), 1);
    EXPECT_EQ(config.getTimers().size(), 1);
    EXPECT_EQ(config.getFilesystems().size(), 1);
    EXPECT_EQ(config.getActions().size(), 1);
}

TEST(DataStructuresTest, ConnixConfigDefaultConstructor)
{
    ConnixConfig config;

    EXPECT_EQ(config.getName(), "");
    EXPECT_TRUE(config.getNodes().empty());
    EXPECT_TRUE(config.getTimers().empty());
    EXPECT_TRUE(config.getFilesystems().empty());
    EXPECT_TRUE(config.getActions().empty());
}

} // namespace UnitTest
} // namespace ConnixCore
