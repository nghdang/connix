#include "gtest/gtest.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/IFileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeType.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

namespace {

class MockFileReader : public IFileReader
{
public:
    std::string readAll(const std::string& filePath) const override
    {
        readHistory.push_back(filePath);
        if (throwOnPath == filePath)
        {
            throw std::runtime_error("Failed to read: " + filePath);
        }
        auto it = fileContents.find(filePath);
        if (it != fileContents.end())
        {
            return it->second;
        }
        return "";
    }

    std::unordered_map<std::string, std::string> fileContents;
    std::string throwOnPath;
    mutable std::vector<std::string> readHistory;
};

class MockJsonValidator : public IJsonValidator
{
public:
    void validate(const std::string& jsonStr,
                  const std::string& schemaStr) const override
    {
        lastValidatedJson = jsonStr;
        lastValidatedSchema = schemaStr;
        if (shouldThrow)
        {
            throw std::invalid_argument("Validation failed");
        }
    }

    bool shouldThrow = false;
    mutable std::string lastValidatedJson;
    mutable std::string lastValidatedSchema;
};

class MockJsonParser : public IJsonParser
{
public:
    ConnixConfig parse(const std::string& jsonStr) const override
    {
        lastParsedJson = jsonStr;
        if (shouldThrow)
        {
            throw std::runtime_error("Parse error");
        }
        return parsedConfig;
    }

    bool shouldThrow = false;
    mutable std::string lastParsedJson;
    ConnixConfig parsedConfig;
};

ConnixConfig createSampleConfig()
{
    std::unordered_map<std::string, NodeConfig> nodes;
    nodes.emplace("test_node",
                  NodeConfig(NodeType::SERVER, NodeTransport::TCP,
                             std::nullopt, std::nullopt, std::nullopt,
                             std::nullopt, std::nullopt, std::nullopt, {}));

    std::unordered_map<std::string, TimerConfig> timers;
    timers.emplace("test_timer", TimerConfig(1000, true, { "ACTION" }));

    std::unordered_map<std::string, FilesystemConfig> filesystems;
    filesystems.emplace("test_fs",
                        FilesystemConfig("/tmp", 100, { "RELOAD" }));

    std::unordered_map<std::string, ActionConfig> actions;
    actions.emplace("test_action",
                    ActionConfig(ActionType::SEND, "payload", "nodeA", "nodeB",
                                 std::nullopt, std::nullopt, std::nullopt,
                                 std::nullopt));

    return ConnixConfig("SampleConfig", nodes, timers, filesystems, actions);
}

} // namespace

TEST(ConfigurationProviderTest, InitialStateEmpty)
{
    MockFileReader reader;
    MockJsonValidator validator;
    MockJsonParser parser;

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getNodes().empty());
    EXPECT_TRUE(provider.getTimers().empty());
    EXPECT_TRUE(provider.getFilesystems().empty());
    EXPECT_TRUE(provider.getActions().empty());
}

TEST(ConfigurationProviderTest, LoadSuccess)
{
    MockFileReader reader;
    reader.fileContents["/etc/connix/config.json"] = "{\"name\": \"test\"}";
    reader.fileContents["/etc/connix/schema.json"] = "{\"type\": \"object\"}";

    MockJsonValidator validator;
    MockJsonParser parser;
    parser.parsedConfig = createSampleConfig();

    ConfigurationProvider provider(reader, validator, parser);

    provider.load("/etc/connix/config.json", "/etc/connix/schema.json");

    ASSERT_EQ(reader.readHistory.size(), 2);
    EXPECT_EQ(reader.readHistory[0], "/etc/connix/config.json");
    EXPECT_EQ(reader.readHistory[1], "/etc/connix/schema.json");

    EXPECT_EQ(validator.lastValidatedJson, "{\"name\": \"test\"}");
    EXPECT_EQ(validator.lastValidatedSchema, "{\"type\": \"object\"}");

    EXPECT_EQ(parser.lastParsedJson, "{\"name\": \"test\"}");

    EXPECT_EQ(provider.getName(), "SampleConfig");
    EXPECT_EQ(provider.getNodes().size(), 1);
    EXPECT_EQ(provider.getTimers().size(), 1);
    EXPECT_EQ(provider.getFilesystems().size(), 1);
    EXPECT_EQ(provider.getActions().size(), 1);
}

TEST(ConfigurationProviderTest, LoadFailsWhenConfigFileReadThrows)
{
    MockFileReader reader;
    reader.throwOnPath = "/etc/connix/config.json";

    MockJsonValidator validator;
    MockJsonParser parser;

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        std::runtime_error);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getNodes().empty());
    EXPECT_TRUE(validator.lastValidatedJson.empty());
    EXPECT_TRUE(parser.lastParsedJson.empty());
}

TEST(ConfigurationProviderTest, LoadFailsWhenSchemaFileReadThrows)
{
    MockFileReader reader;
    reader.fileContents["/etc/connix/config.json"] = "{\"name\": \"test\"}";
    reader.throwOnPath = "/etc/connix/schema.json";

    MockJsonValidator validator;
    MockJsonParser parser;

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        std::runtime_error);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getNodes().empty());
    EXPECT_TRUE(validator.lastValidatedJson.empty());
    EXPECT_TRUE(parser.lastParsedJson.empty());
}

TEST(ConfigurationProviderTest, LoadFailsWhenValidatorThrows)
{
    MockFileReader reader;
    reader.fileContents["/etc/connix/config.json"] = "{\"name\": \"invalid\"}";
    reader.fileContents["/etc/connix/schema.json"] = "{\"type\": \"object\"}";

    MockJsonValidator validator;
    validator.shouldThrow = true;

    MockJsonParser parser;

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        std::invalid_argument);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getNodes().empty());
    EXPECT_TRUE(parser.lastParsedJson.empty());
}

TEST(ConfigurationProviderTest, LoadFailsWhenParserThrows)
{
    MockFileReader reader;
    reader.fileContents["/etc/connix/config.json"] = "{corrupted}";
    reader.fileContents["/etc/connix/schema.json"] = "{\"type\": \"object\"}";

    MockJsonValidator validator;
    MockJsonParser parser;
    parser.shouldThrow = true;

    ConfigurationProvider provider(reader, validator, parser);

    EXPECT_THROW(
        provider.load("/etc/connix/config.json", "/etc/connix/schema.json"),
        std::runtime_error);

    EXPECT_TRUE(provider.getName().empty());
    EXPECT_TRUE(provider.getNodes().empty());
}

TEST(ConfigurationProviderTest, PolymorphicUsageViaInterface)
{
    MockFileReader reader;
    reader.fileContents["config.json"] = "{}";
    reader.fileContents["schema.json"] = "{}";

    MockJsonValidator validator;
    MockJsonParser parser;
    parser.parsedConfig = createSampleConfig();

    std::unique_ptr<IConfigurationProvider> provider =
        std::make_unique<ConfigurationProvider>(reader, validator, parser);

    provider->load("config.json", "schema.json");

    EXPECT_EQ(provider->getName(), "SampleConfig");
    EXPECT_EQ(provider->getNodes().size(), 1);
    EXPECT_EQ(provider->getTimers().size(), 1);
    EXPECT_EQ(provider->getFilesystems().size(), 1);
    EXPECT_EQ(provider->getActions().size(), 1);
}

} // namespace UnitTest
} // namespace ConnixCore
