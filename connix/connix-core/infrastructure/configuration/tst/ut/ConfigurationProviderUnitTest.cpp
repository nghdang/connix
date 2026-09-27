#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIFileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIJsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIJsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

namespace {

ConnixConfig createSampleConfig()
{
    std::unordered_map<std::string, ServerNodeConfig> serverNodes;
    serverNodes.emplace("test_server",
                        ServerNodeConfig(NodeTransport::TCP,
                                         Endpoint("127.0.0.1", 8080),
                                         FrameConfig(), 10, 4096, {}));

    std::unordered_map<std::string, ClientNodeConfig> clientNodes;
    clientNodes.emplace("test_client",
                        ClientNodeConfig(NodeTransport::TCP, FrameConfig(),
                                         4096, 5000, 30000, {}));

    std::unordered_map<std::string, PeerNodeConfig> peerNodes;
    peerNodes.emplace("test_peer", PeerNodeConfig(NodeTransport::TCP,
                                                  Endpoint("127.0.0.1", 8080),
                                                  FrameConfig()));

    std::unordered_map<std::string, TimerConfig> timers;
    timers.emplace("test_timer", TimerConfig(1000, true, { "ACTION" }));

    std::unordered_map<std::string, FilesystemConfig> filesystems;
    filesystems.emplace("test_fs",
                        FilesystemConfig("/tmp", 100, { "RELOAD" }));

    std::unordered_map<std::string, ActionConfig> actions;
    actions.emplace("test_action",
                    ActionConfig(ActionType::SEND,
                                 ActionPayload(PayloadType::BYTES, "payload"),
                                 "nodeA", "nodeB"));

    return { "SampleConfig",         std::move(serverNodes),
             std::move(clientNodes), std::move(peerNodes),
             std::move(timers),      std::move(filesystems),
             std::move(actions) };
}

} // namespace

TEST(ConfigurationProviderTest, InitialStateEmpty)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    const ConfigurationProvider provider(reader, validator, parser);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getServerNodes().empty());
    EXPECT_TRUE(provider.getClientNodes().empty());
    EXPECT_TRUE(provider.getPeerNodes().empty());
    EXPECT_TRUE(provider.getTimers().empty());
    EXPECT_TRUE(provider.getFilesystems().empty());
    EXPECT_TRUE(provider.getActions().empty());
}

TEST(ConfigurationProviderTest, LoadSuccess)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    {
        const InSequence seq;
        EXPECT_CALL(reader, readAll("/etc/connix/config.json"))
            .WillOnce(Return(R"({"name": "test"})"));
        EXPECT_CALL(reader, readAll("/etc/connix/schema.json"))
            .WillOnce(Return(R"({"type": "object"})"));
    }

    EXPECT_CALL(validator,
                validate(R"({"name": "test"})", R"({"type": "object"})"))
        .Times(1);

    EXPECT_CALL(parser, parse(R"({"name": "test"})"))
        .WillOnce(Return(createSampleConfig()));

    ConfigurationProvider provider(reader, validator, parser);

    provider.load("/etc/connix/config.json", "/etc/connix/schema.json");

    EXPECT_EQ(provider.getName(), "SampleConfig");
    EXPECT_EQ(provider.getServerNodes().size(), 1);
    EXPECT_EQ(provider.getClientNodes().size(), 1);
    EXPECT_EQ(provider.getPeerNodes().size(), 1);
    EXPECT_EQ(provider.getTimers().size(), 1);
    EXPECT_EQ(provider.getFilesystems().size(), 1);
    EXPECT_EQ(provider.getActions().size(), 1);
}

TEST(ConfigurationProviderTest, LoadFailsWhenConfigFileReadThrows)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    EXPECT_CALL(reader, readAll("/etc/connix/config.json"))
        .WillOnce(Throw(ConfigurationException(
            ConfigurationErrorCode::FILE_NOT_FOUND,
            "Failed to read: /etc/connix/config.json")));

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        ConfigurationException);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getServerNodes().empty());
    EXPECT_TRUE(provider.getClientNodes().empty());
    EXPECT_TRUE(provider.getPeerNodes().empty());
}

TEST(ConfigurationProviderTest, LoadFailsWhenSchemaFileReadThrows)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    EXPECT_CALL(reader, readAll("/etc/connix/config.json"))
        .WillOnce(Return(R"({"name": "test"})"));
    EXPECT_CALL(reader, readAll("/etc/connix/schema.json"))
        .WillOnce(Throw(ConfigurationException(
            ConfigurationErrorCode::FILE_NOT_FOUND,
            "Failed to read: /etc/connix/schema.json")));

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        ConfigurationException);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getServerNodes().empty());
    EXPECT_TRUE(provider.getClientNodes().empty());
    EXPECT_TRUE(provider.getPeerNodes().empty());
}

TEST(ConfigurationProviderTest, LoadFailsWhenValidatorThrows)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    EXPECT_CALL(reader, readAll("/etc/connix/config.json"))
        .WillOnce(Return(R"({"name": "invalid"})"));
    EXPECT_CALL(reader, readAll("/etc/connix/schema.json"))
        .WillOnce(Return(R"({"type": "object"})"));

    EXPECT_CALL(validator,
                validate(R"({"name": "invalid"})", R"({"type": "object"})"))
        .WillOnce(Throw(std::invalid_argument("Validation failed")));

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        std::invalid_argument);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getServerNodes().empty());
    EXPECT_TRUE(provider.getClientNodes().empty());
    EXPECT_TRUE(provider.getPeerNodes().empty());
}

TEST(ConfigurationProviderTest, LoadFailsWhenParserThrows)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    EXPECT_CALL(reader, readAll("/etc/connix/config.json"))
        .WillOnce(Return("{corrupted}"));
    EXPECT_CALL(reader, readAll("/etc/connix/schema.json"))
        .WillOnce(Return(R"({"type": "object"})"));

    EXPECT_CALL(validator, validate("{corrupted}", R"({"type": "object"})"))
        .Times(1);

    EXPECT_CALL(parser, parse("{corrupted}"))
        .WillOnce(Throw(std::runtime_error("Parse error")));

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        std::runtime_error);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getServerNodes().empty());
    EXPECT_TRUE(provider.getClientNodes().empty());
    EXPECT_TRUE(provider.getPeerNodes().empty());
}

TEST(ConfigurationProviderTest, PolymorphicUsageViaInterface)
{
    const MockIFileReader reader;
    const MockIJsonValidator validator;
    const MockIJsonParser parser;

    {
        const InSequence seq;
        EXPECT_CALL(reader, readAll("config.json")).WillOnce(Return("{}"));
        EXPECT_CALL(reader, readAll("schema.json")).WillOnce(Return("{}"));
    }

    EXPECT_CALL(validator, validate("{}", "{}")).Times(1);

    EXPECT_CALL(parser, parse("{}")).WillOnce(Return(createSampleConfig()));

    std::unique_ptr<IConfigurationProvider> provider =
        std::make_unique<ConfigurationProvider>(reader, validator, parser);

    provider->load("config.json", "schema.json");

    EXPECT_EQ(provider->getName(), "SampleConfig");
    EXPECT_EQ(provider->getServerNodes().size(), 1);
    EXPECT_EQ(provider->getClientNodes().size(), 1);
    EXPECT_EQ(provider->getPeerNodes().size(), 1);
    EXPECT_EQ(provider->getTimers().size(), 1);
    EXPECT_EQ(provider->getFilesystems().size(), 1);
    EXPECT_EQ(provider->getActions().size(), 1);
}

} // namespace UnitTest
} // namespace ConnixCore
