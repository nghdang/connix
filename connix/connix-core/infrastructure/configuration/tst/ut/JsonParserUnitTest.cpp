#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"
#include "ConnixCore/Infrastructure/Configuration/JsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"

using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

TEST(JsonParserTest, ParseValidFullJson)
{
    const std::string validJson = R"({
        "name": "CompleteConfig",
        "nodes": {
            "serverNodes": {
                "srv1": {
                    "transport": "TCP",
                    "endpoint": { "address": "0.0.0.0", "port": 8080 },
                    "frame": { "type": "FIXED_SIZE", "size": 128 },
                    "maxConnections": 50,
                    "bufferSize": 8192,
                    "onReceived": [
                        {
                            "patterns": ["01 02", "AA BB"],
                            "offset": 4,
                            "actions": ["act1"]
                        }
                    ]
                },
                "srv_uds": {
                    "transport": "UDS_STREAM",
                    "endpoint": { "address": "/tmp/srv.sock" },
                    "frame": { "type": "NONE" }
                }
            },
            "clientNodes": {
                "cli1": {
                    "transport": "UDP",
                    "frame": {
                        "type": "LENGTH_PREFIX",
                        "lengthOffset": 2,
                        "lengthSize": 4,
                        "byteOrder": "LITTLE_ENDIAN"
                    },
                    "bufferSize": 2048,
                    "connectionTimeout": 1000,
                    "idleTimeout": 5000,
                    "onReceived": []
                },
                "cli_uds": {
                    "transport": "UDS_DATAGRAM",
                    "frame": { "type": "NONE" }
                }
            },
            "peerNodes": {
                "peer1": {
                    "transport": "TCP",
                    "endpoint": { "address": "127.0.0.1", "port": 9090 },
                    "frame": { "type": "NONE" }
                }
            }
        },
        "timers": {
            "timer1": {
                "interval": 250,
                "singleShot": true,
                "onTimeout": ["act1"]
            }
        },
        "filesystems": {
            "fs1": {
                "path": "/tmp/watch",
                "debounce": 100,
                "onModified": ["act1"]
            }
        },
        "actions": {
            "act1": {
                "type": "SEND",
                "payload": { "type": "FILE", "source": "/tmp/data.bin" },
                "sourceNode": "cli1",
                "targetNode": "peer1",
                "executionDelay": 10,
                "executionTimeout": 1000,
                "maxPending": 20
            },
            "act2": {
                "type": "RESPOND",
                "payload": { "type": "BYTES", "source": "FF EE DD" }
            }
        }
    })";

    const JsonParser parser;
    const ConnixConfig config = parser.parse(validJson);

    EXPECT_EQ(config.getName(), "CompleteConfig");

    // Server nodes
    ASSERT_EQ(config.getServerNodes().size(), 2);
    const auto& srv1 = config.getServerNodes().at("srv1");
    EXPECT_EQ(srv1.getTransport(), NodeTransport::TCP);
    EXPECT_EQ(srv1.getEndpoint().getAddress(), "0.0.0.0");
    ASSERT_TRUE(srv1.getEndpoint().getPort().has_value());
    EXPECT_EQ(srv1.getEndpoint().getPort().value_or(0), 8080);
    EXPECT_EQ(srv1.getFrame().getType(), FrameType::FIXED_SIZE);
    EXPECT_EQ(srv1.getFrame().getSize(), 128);
    EXPECT_EQ(srv1.getMaxConnections(), 50);
    EXPECT_EQ(srv1.getBufferSize(), 8192);
    ASSERT_EQ(srv1.getOnReceived().size(), 1);
    EXPECT_EQ(srv1.getOnReceived()[0].getOffset(), 4);
    EXPECT_EQ(srv1.getOnReceived()[0].getPatterns().size(), 2);
    EXPECT_EQ(srv1.getOnReceived()[0].getActions().size(), 1);

    const auto& srvUds = config.getServerNodes().at("srv_uds");
    EXPECT_EQ(srvUds.getTransport(), NodeTransport::UDS_STREAM);
    EXPECT_FALSE(srvUds.getEndpoint().getPort().has_value());
    EXPECT_EQ(srvUds.getFrame().getType(), FrameType::NONE);

    // Client nodes
    ASSERT_EQ(config.getClientNodes().size(), 2);
    const auto& cli1 = config.getClientNodes().at("cli1");
    EXPECT_EQ(cli1.getTransport(), NodeTransport::UDP);
    EXPECT_EQ(cli1.getFrame().getType(), FrameType::LENGTH_PREFIX);
    EXPECT_EQ(cli1.getFrame().getLengthOffset(), 2);
    EXPECT_EQ(cli1.getFrame().getLengthSize(), 4);
    EXPECT_EQ(cli1.getFrame().getByteOrder(), ByteOrder::LITTLE_ENDIAN);
    EXPECT_EQ(cli1.getBufferSize(), 2048);
    EXPECT_EQ(cli1.getConnectionTimeout(), 1000);
    EXPECT_EQ(cli1.getIdleTimeout(), 5000);

    const auto& cliUds = config.getClientNodes().at("cli_uds");
    EXPECT_EQ(cliUds.getTransport(), NodeTransport::UDS_DATAGRAM);

    // Peer nodes
    ASSERT_EQ(config.getPeerNodes().size(), 1);
    const auto& peer1 = config.getPeerNodes().at("peer1");
    EXPECT_EQ(peer1.getTransport(), NodeTransport::TCP);

    // Timers
    ASSERT_EQ(config.getTimers().size(), 1);
    const auto& timer1 = config.getTimers().at("timer1");
    EXPECT_EQ(timer1.getInterval(), 250);
    EXPECT_TRUE(timer1.isSingleShot());
    const std::vector<std::string> expectedTimeout = { "act1" };
    EXPECT_EQ(timer1.getOnTimeout(), expectedTimeout);

    // Filesystems
    ASSERT_EQ(config.getFilesystems().size(), 1);
    const auto& fs1 = config.getFilesystems().at("fs1");
    EXPECT_EQ(fs1.getPath(), "/tmp/watch");
    EXPECT_EQ(fs1.getDebounce(), 100);

    // Actions
    ASSERT_EQ(config.getActions().size(), 2);
    const auto& act1 = config.getActions().at("act1");
    EXPECT_EQ(act1.getType(), ActionType::SEND);
    EXPECT_EQ(act1.getPayload().getType(), PayloadType::FILE);
    EXPECT_EQ(act1.getSourceNode(), "cli1");
    EXPECT_EQ(act1.getTargetNode(), "peer1");
    EXPECT_EQ(act1.getExecutionDelay(), 10);
    EXPECT_EQ(act1.getExecutionTimeout(), 1000);
    EXPECT_EQ(act1.getMaxPending(), 20);

    const auto& act2 = config.getActions().at("act2");
    EXPECT_EQ(act2.getType(), ActionType::RESPOND);
    EXPECT_EQ(act2.getPayload().getType(), PayloadType::BYTES);
}

TEST(JsonParserTest, ParseInvalidJsonThrows)
{
    const JsonParser parser;
    EXPECT_THROW(parser.parse("{ invalid json"), ConfigurationException);
}

TEST(JsonParserTest, ParseUnknownTransportThrows)
{
    const std::string invalidJson = R"({
        "name": "InvalidTransport",
        "nodes": {
            "serverNodes": {
                "s1": {
                    "transport": "SCTP",
                    "endpoint": { "address": "127.0.0.1" },
                    "frame": { "type": "NONE" }
                }
            }
        }
    })";

    const JsonParser parser;
    try
    {
        parser.parse(invalidJson);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConfigurationErrorCode::JSON_PARSE_FAILED);
        EXPECT_NE(std::string(ex.what()).find("Unknown transport"),
                  std::string::npos);
    }
}

TEST(JsonParserTest, ParseUnknownFrameTypeThrows)
{
    const std::string invalidJson = R"({
        "name": "InvalidFrame",
        "nodes": {
            "serverNodes": {
                "s1": {
                    "transport": "TCP",
                    "endpoint": { "address": "127.0.0.1" },
                    "frame": { "type": "MAGIC_HEADER" }
                }
            }
        }
    })";

    const JsonParser parser;
    try
    {
        parser.parse(invalidJson);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConfigurationErrorCode::JSON_PARSE_FAILED);
        EXPECT_NE(std::string(ex.what()).find("Unknown frame type"),
                  std::string::npos);
    }
}

TEST(JsonParserTest, ParseMalformedEventRuleThrows)
{
    const std::string invalidJson = R"({
        "name": "MalformedRule",
        "nodes": {
            "serverNodes": {
                "s1": {
                    "transport": "TCP",
                    "endpoint": { "address": "127.0.0.1" },
                    "frame": { "type": "NONE" },
                    "onReceived": [
                        { "offset": 0 }
                    ]
                }
            }
        }
    })";

    const JsonParser parser;
    EXPECT_THROW(parser.parse(invalidJson), ConfigurationException);
}

} // namespace UnitTest
} // namespace ConnixCore
