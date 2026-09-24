#include "gtest/gtest.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/IFileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

class TestFileReader : public IFileReader
{
public:
    std::string readAll(const std::string& filePath) const override
    {
        return "content of " + filePath;
    }
};

class TestJsonValidator : public IJsonValidator
{
public:
    void validate(const std::string& jsonStr,
                  const std::string& schemaStr) const override
    {
        (void)jsonStr;
        (void)schemaStr;
    }
};

class TestJsonParser : public IJsonParser
{
public:
    ConnixConfig parse(const std::string& jsonStr) const override
    {
        (void)jsonStr;
        return ConnixConfig("parsed_config", {}, {}, {}, {}, {}, {});
    }
};

class TestConfigurationProvider : public IConfigurationProvider
{
public:
    void load(const std::string& configPath,
              const std::string& schemaPath) override
    {
        (void)configPath;
        (void)schemaPath;
    }

    const std::string& getName() const override
    {
        return m_name;
    }

    const std::unordered_map<std::string, ServerNodeConfig>&
    getServerNodes() const override
    {
        return m_serverNodes;
    }

    const std::unordered_map<std::string, ClientNodeConfig>&
    getClientNodes() const override
    {
        return m_clientNodes;
    }

    const std::unordered_map<std::string, PeerNodeConfig>&
    getPeerNodes() const override
    {
        return m_peerNodes;
    }

    const std::unordered_map<std::string, TimerConfig>&
    getTimers() const override
    {
        return m_timers;
    }

    const std::unordered_map<std::string, FilesystemConfig>&
    getFilesystems() const override
    {
        return m_filesystems;
    }

    const std::unordered_map<std::string, ActionConfig>&
    getActions() const override
    {
        return m_actions;
    }

private:
    std::string m_name;
    std::unordered_map<std::string, ServerNodeConfig> m_serverNodes;
    std::unordered_map<std::string, ClientNodeConfig> m_clientNodes;
    std::unordered_map<std::string, PeerNodeConfig> m_peerNodes;
    std::unordered_map<std::string, TimerConfig> m_timers;
    std::unordered_map<std::string, FilesystemConfig> m_filesystems;
    std::unordered_map<std::string, ActionConfig> m_actions;
};

TEST(ConfigurationInterfacesTest, FileReaderPolymorphism)
{
    std::unique_ptr<IFileReader> reader = std::make_unique<TestFileReader>();
    EXPECT_EQ(reader->readAll("test_path.json"), "content of test_path.json");
}

TEST(ConfigurationInterfacesTest, JsonValidatorPolymorphism)
{
    std::unique_ptr<IJsonValidator> validator =
        std::make_unique<TestJsonValidator>();
    EXPECT_NO_THROW(validator->validate("{}", "{}"));
}

TEST(ConfigurationInterfacesTest, JsonParserPolymorphism)
{
    std::unique_ptr<IJsonParser> parser = std::make_unique<TestJsonParser>();
    ConnixConfig config = parser->parse("{}");
    EXPECT_EQ(config.getName(), "parsed_config");
}

TEST(ConfigurationInterfacesTest, ConfigurationProviderPolymorphism)
{
    std::unique_ptr<IConfigurationProvider> provider =
        std::make_unique<TestConfigurationProvider>();
    EXPECT_NO_THROW(provider->load("config.json", "schema.json"));
    EXPECT_TRUE(provider->getName().empty());
    EXPECT_TRUE(provider->getServerNodes().empty());
    EXPECT_TRUE(provider->getClientNodes().empty());
    EXPECT_TRUE(provider->getPeerNodes().empty());
    EXPECT_TRUE(provider->getTimers().empty());
    EXPECT_TRUE(provider->getFilesystems().empty());
    EXPECT_TRUE(provider->getActions().empty());
}

} // namespace UnitTest
} // namespace ConnixCore
