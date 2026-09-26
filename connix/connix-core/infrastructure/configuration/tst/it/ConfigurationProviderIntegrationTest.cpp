#include "gtest/gtest.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ActionType.hpp"
#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"
#include "ConnixCore/Infrastructure/Configuration/ConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/FileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/JsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/JsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/NodeTransport.hpp"
#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace IntegrationTest {

namespace {

std::string getRealConfigPath()
{
    const std::filesystem::path projectRoot(CONNIX_PROJECT_ROOT);
    return (projectRoot / "docs" / "connix-config.json").string();
}

std::string getRealSchemaPath()
{
    const std::filesystem::path projectRoot(CONNIX_PROJECT_ROOT);
    return (projectRoot / "connix" / "connix-core" / "infrastructure" /
            "configuration" / "res" / "connix-config.schema.json")
        .string();
}

} // namespace

class ConfigurationProviderIntegrationTest : public ::testing::Test
{
public:
    void TestBody() override
    {
    }

protected:
    void SetUp() override
    {
        m_tempDirPath =
            (std::filesystem::temp_directory_path() / "connix_it_temp")
                .string();
        std::filesystem::create_directories(m_tempDirPath);
    }

    void TearDown() override
    {
        std::filesystem::remove_all(m_tempDirPath);
    }

    std::string createTempFile(const std::string& fileName,
                               const std::string& content) const
    {
        const std::filesystem::path filePath =
            std::filesystem::path(m_tempDirPath) / fileName;
        std::ofstream stream(filePath);
        stream << content;
        stream.close();
        return filePath.string();
    }

    std::string m_tempDirPath;
};

TEST_F(ConfigurationProviderIntegrationTest,
       LoadRealConfigAndSchemaSuccessfully)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    ConnixCore::Infrastructure::Configuration::ConfigurationProvider provider(
        fileReader, jsonValidator, jsonParser);

    const std::string configPath = getRealConfigPath();
    const std::string schemaPath = getRealSchemaPath();

    ASSERT_TRUE(std::filesystem::exists(configPath))
        << "Config file not found at: " << configPath;
    ASSERT_TRUE(std::filesystem::exists(schemaPath))
        << "Schema file not found at: " << schemaPath;

    EXPECT_NO_THROW(provider.load(configPath, schemaPath));

    EXPECT_EQ(provider.getName(), "Test full workflow");

    // Server nodes validation
    const auto& serverNodes = provider.getServerNodes();
    ASSERT_EQ(serverNodes.size(), 4U);

    ASSERT_NE(serverNodes.find("test-tcp-server"), serverNodes.end());
    const auto& tcpServer = serverNodes.at("test-tcp-server");
    EXPECT_EQ(tcpServer.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::TCP);
    EXPECT_EQ(tcpServer.getEndpoint().getAddress(), "127.0.0.1");
    ASSERT_TRUE(tcpServer.getEndpoint().getPort().has_value());
    EXPECT_EQ(tcpServer.getEndpoint().getPort().value_or(0), 56789);
    EXPECT_EQ(
        tcpServer.getFrame().getType(),
        ConnixCore::Infrastructure::Configuration::FrameType::FIXED_SIZE);
    EXPECT_EQ(tcpServer.getFrame().getSize(), 4U);
    EXPECT_EQ(tcpServer.getMaxConnections(), 20U);
    EXPECT_EQ(tcpServer.getBufferSize(), 4096U);
    ASSERT_EQ(tcpServer.getOnReceived().size(), 1U);
    EXPECT_EQ(tcpServer.getOnReceived()[0].getOffset(), 0U);
    EXPECT_EQ(tcpServer.getOnReceived()[0].getPatterns(),
              std::vector<std::string>({ "12 34 56 78" }));
    EXPECT_EQ(tcpServer.getOnReceived()[0].getActions(),
              std::vector<std::string>({ "respond" }));

    ASSERT_NE(serverNodes.find("test-udp-server"), serverNodes.end());
    const auto& udpServer = serverNodes.at("test-udp-server");
    EXPECT_EQ(udpServer.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::UDP);
    EXPECT_EQ(udpServer.getFrame().getType(),
              ConnixCore::Infrastructure::Configuration::FrameType::NONE);

    ASSERT_NE(serverNodes.find("test-uds-server"), serverNodes.end());
    const auto& udsServer = serverNodes.at("test-uds-server");
    EXPECT_EQ(
        udsServer.getTransport(),
        ConnixCore::Infrastructure::Configuration::NodeTransport::UDS_STREAM);
    EXPECT_EQ(udsServer.getEndpoint().getAddress(),
              "/tmp/test-uds-server.sock");
    EXPECT_FALSE(udsServer.getEndpoint().getPort().has_value());
    EXPECT_EQ(
        udsServer.getFrame().getType(),
        ConnixCore::Infrastructure::Configuration::FrameType::LENGTH_PREFIX);
    EXPECT_EQ(udsServer.getFrame().getLengthOffset(), 0U);
    EXPECT_EQ(udsServer.getFrame().getLengthSize(), 2U);
    EXPECT_EQ(
        udsServer.getFrame().getByteOrder(),
        ConnixCore::Infrastructure::Configuration::ByteOrder::BIG_ENDIAN);

    ASSERT_NE(serverNodes.find("test-uds-datagram-server"), serverNodes.end());
    const auto& udsDatagramServer = serverNodes.at("test-uds-datagram-server");
    EXPECT_EQ(udsDatagramServer.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::
                  UDS_DATAGRAM);
    EXPECT_EQ(udsDatagramServer.getFrame().getType(),
              ConnixCore::Infrastructure::Configuration::FrameType::NONE);

    // Client nodes validation
    const auto& clientNodes = provider.getClientNodes();
    ASSERT_EQ(clientNodes.size(), 4U);

    ASSERT_NE(clientNodes.find("test-tcp-client"), clientNodes.end());
    const auto& tcpClient = clientNodes.at("test-tcp-client");
    EXPECT_EQ(tcpClient.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::TCP);
    EXPECT_EQ(tcpClient.getConnectionTimeout(), 5000U);
    EXPECT_EQ(tcpClient.getIdleTimeout(), 30000U);
    EXPECT_EQ(tcpClient.getBufferSize(), 1024U);
    EXPECT_EQ(
        tcpClient.getFrame().getType(),
        ConnixCore::Infrastructure::Configuration::FrameType::FIXED_SIZE);
    EXPECT_EQ(tcpClient.getFrame().getSize(), 4U);

    ASSERT_NE(clientNodes.find("test-udp-client"), clientNodes.end());
    const auto& udpClient = clientNodes.at("test-udp-client");
    EXPECT_EQ(udpClient.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::UDP);
    EXPECT_EQ(udpClient.getBufferSize(), 4096U);
    EXPECT_EQ(udpClient.getFrame().getType(),
              ConnixCore::Infrastructure::Configuration::FrameType::NONE);

    ASSERT_NE(clientNodes.find("test-uds-client"), clientNodes.end());
    const auto& udsClient = clientNodes.at("test-uds-client");
    EXPECT_EQ(
        udsClient.getTransport(),
        ConnixCore::Infrastructure::Configuration::NodeTransport::UDS_STREAM);
    EXPECT_EQ(
        udsClient.getFrame().getType(),
        ConnixCore::Infrastructure::Configuration::FrameType::LENGTH_PREFIX);
    EXPECT_EQ(udsClient.getFrame().getLengthOffset(), 0U);
    EXPECT_EQ(udsClient.getFrame().getLengthSize(), 2U);
    EXPECT_EQ(
        udsClient.getFrame().getByteOrder(),
        ConnixCore::Infrastructure::Configuration::ByteOrder::BIG_ENDIAN);

    ASSERT_NE(clientNodes.find("test-uds-datagram-client"), clientNodes.end());
    const auto& udsDatagramClient = clientNodes.at("test-uds-datagram-client");
    EXPECT_EQ(udsDatagramClient.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::
                  UDS_DATAGRAM);
    EXPECT_EQ(udsDatagramClient.getFrame().getType(),
              ConnixCore::Infrastructure::Configuration::FrameType::NONE);

    // Peer nodes validation
    const auto& peerNodes = provider.getPeerNodes();
    ASSERT_EQ(peerNodes.size(), 1U);
    ASSERT_NE(peerNodes.find("user-tcp-server"), peerNodes.end());
    const auto& peerNode = peerNodes.at("user-tcp-server");
    EXPECT_EQ(peerNode.getTransport(),
              ConnixCore::Infrastructure::Configuration::NodeTransport::TCP);
    EXPECT_EQ(peerNode.getEndpoint().getAddress(), "127.0.0.1");
    ASSERT_TRUE(peerNode.getEndpoint().getPort().has_value());
    EXPECT_EQ(peerNode.getEndpoint().getPort().value_or(0), 12345);
    EXPECT_EQ(
        peerNode.getFrame().getType(),
        ConnixCore::Infrastructure::Configuration::FrameType::FIXED_SIZE);
    EXPECT_EQ(peerNode.getFrame().getSize(), 4U);

    // Timers validation
    const auto& timers = provider.getTimers();
    ASSERT_EQ(timers.size(), 1U);
    ASSERT_NE(timers.find("timer-1000"), timers.end());
    const auto& timer = timers.at("timer-1000");
    EXPECT_EQ(timer.getInterval(), 1000U);
    EXPECT_FALSE(timer.isSingleShot());
    EXPECT_EQ(timer.getOnTimeout(),
              std::vector<std::string>(
                  { "send-test-data", "send-tcp", "send-uds-datagram" }));

    // Filesystems validation
    const auto& filesystems = provider.getFilesystems();
    ASSERT_EQ(filesystems.size(), 2U);
    ASSERT_NE(filesystems.find("test-uds-data"), filesystems.end());
    EXPECT_EQ(filesystems.at("test-uds-data").getPath(),
              "/tmp/test-uds-data.bin");
    EXPECT_EQ(filesystems.at("test-uds-data").getDebounce(), 100U);
    ASSERT_NE(filesystems.find("test-udp-data"), filesystems.end());
    EXPECT_EQ(filesystems.at("test-udp-data").getPath(),
              "/tmp/test-udp-event");
    EXPECT_EQ(filesystems.at("test-udp-data").getDebounce(), 100U);

    // Actions validation
    const auto& actions = provider.getActions();
    ASSERT_EQ(actions.size(), 6U);

    ASSERT_NE(actions.find("respond"), actions.end());
    const auto& respondAction = actions.at("respond");
    EXPECT_EQ(respondAction.getType(),
              ConnixCore::Infrastructure::Configuration::ActionType::RESPOND);
    EXPECT_EQ(respondAction.getPayload().getType(),
              ConnixCore::Infrastructure::Configuration::PayloadType::BYTES);
    EXPECT_EQ(respondAction.getPayload().getSource(), "12 34 56 78");

    ASSERT_NE(actions.find("send-test-data"), actions.end());
    const auto& sendTestData = actions.at("send-test-data");
    EXPECT_EQ(sendTestData.getType(),
              ConnixCore::Infrastructure::Configuration::ActionType::SEND);
    EXPECT_EQ(sendTestData.getSourceNode(), "test-tcp-client");
    EXPECT_EQ(sendTestData.getTargetNode(), "user-tcp-server");
    EXPECT_EQ(sendTestData.getExecutionDelay(), 10U);
    EXPECT_EQ(sendTestData.getExecutionTimeout(), 5000U);
    EXPECT_EQ(sendTestData.getMaxPending(), 100U);

    ASSERT_NE(actions.find("send-udp"), actions.end());
    const auto& sendUdp = actions.at("send-udp");
    EXPECT_EQ(sendUdp.getType(),
              ConnixCore::Infrastructure::Configuration::ActionType::SEND);
    EXPECT_EQ(sendUdp.getPayload().getType(),
              ConnixCore::Infrastructure::Configuration::PayloadType::FILE);
    EXPECT_EQ(sendUdp.getPayload().getSource(), "/tmp/test-udp-data.bin");
}

TEST_F(ConfigurationProviderIntegrationTest, PolymorphicInterfaceExecution)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    const std::unique_ptr<
        ConnixCore::Infrastructure::Configuration::IConfigurationProvider>
        provider = std::make_unique<
            ConnixCore::Infrastructure::Configuration::ConfigurationProvider>(
            fileReader, jsonValidator, jsonParser);

    const std::string configPath = getRealConfigPath();
    const std::string schemaPath = getRealSchemaPath();

    EXPECT_NO_THROW(provider->load(configPath, schemaPath));

    EXPECT_EQ(provider->getName(), "Test full workflow");
    EXPECT_EQ(provider->getServerNodes().size(), 4U);
    EXPECT_EQ(provider->getClientNodes().size(), 4U);
    EXPECT_EQ(provider->getPeerNodes().size(), 1U);
    EXPECT_EQ(provider->getTimers().size(), 1U);
    EXPECT_EQ(provider->getFilesystems().size(), 2U);
    EXPECT_EQ(provider->getActions().size(), 6U);
}

TEST_F(ConfigurationProviderIntegrationTest,
       NonExistentConfigFileThrowsFileNotFound)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    ConnixCore::Infrastructure::Configuration::ConfigurationProvider provider(
        fileReader, jsonValidator, jsonParser);

    const std::string nonExistentConfig =
        (std::filesystem::path(m_tempDirPath) / "does_not_exist.json")
            .string();
    const std::string schemaPath = getRealSchemaPath();

    try
    {
        provider.load(nonExistentConfig, schemaPath);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::FILE_NOT_FOUND);
    }
}

TEST_F(ConfigurationProviderIntegrationTest,
       NonExistentSchemaFileThrowsFileNotFound)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    ConnixCore::Infrastructure::Configuration::ConfigurationProvider provider(
        fileReader, jsonValidator, jsonParser);

    const std::string configPath = getRealConfigPath();
    const std::string nonExistentSchema =
        (std::filesystem::path(m_tempDirPath) / "does_not_exist_schema.json")
            .string();

    try
    {
        provider.load(configPath, nonExistentSchema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::FILE_NOT_FOUND);
    }
}

TEST_F(ConfigurationProviderIntegrationTest,
       MalformedJsonFileThrowsSchemaValidationFailed)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    ConnixCore::Infrastructure::Configuration::ConfigurationProvider provider(
        fileReader, jsonValidator, jsonParser);

    const std::string malformedConfigPath =
        createTempFile("malformed.json", R"({ "name": "broken", )");
    const std::string schemaPath = getRealSchemaPath();

    try
    {
        provider.load(malformedConfigPath, schemaPath);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST_F(ConfigurationProviderIntegrationTest,
       SchemaMismatchThrowsSchemaValidationFailed)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    ConnixCore::Infrastructure::Configuration::ConfigurationProvider provider(
        fileReader, jsonValidator, jsonParser);

    // Missing required top-level "nodes", "timers", "filesystems", "actions"
    const std::string invalidConfigPath = createTempFile(
        "invalid_structure.json", R"({ "name": "Incomplete" })");
    const std::string schemaPath = getRealSchemaPath();

    try
    {
        provider.load(invalidConfigPath, schemaPath);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST_F(ConfigurationProviderIntegrationTest, ReloadingReplacesPreviousState)
{
    const ConnixCore::Infrastructure::Configuration::FileReader fileReader;
    const ConnixCore::Infrastructure::Configuration::JsonValidator
        jsonValidator;
    const ConnixCore::Infrastructure::Configuration::JsonParser jsonParser;

    ConnixCore::Infrastructure::Configuration::ConfigurationProvider provider(
        fileReader, jsonValidator, jsonParser);

    const std::string firstConfigPath = getRealConfigPath();
    const std::string schemaPath = getRealSchemaPath();

    provider.load(firstConfigPath, schemaPath);
    EXPECT_EQ(provider.getName(), "Test full workflow");
    EXPECT_EQ(provider.getServerNodes().size(), 4U);

    // Create a second valid minimal config
    const std::string secondConfigContent = R"({
        "name": "UpdatedConfig",
        "nodes": {
            "serverNodes": {},
            "clientNodes": {},
            "peerNodes": {
                "peer-updated": {
                    "transport": "TCP",
                    "endpoint": {
                        "address": "10.0.0.1",
                        "port": 9999
                    },
                    "frame": {
                        "type": "NONE"
                    }
                }
            }
        },
        "timers": {},
        "filesystems": {},
        "actions": {
            "action-updated": {
                "type": "RESPOND",
                "payload": {
                    "type": "BYTES",
                    "source": "AA BB CC"
                }
            }
        }
    })";

    const std::string secondConfigPath =
        createTempFile("updated_config.json", secondConfigContent);

    provider.load(secondConfigPath, schemaPath);

    EXPECT_EQ(provider.getName(), "UpdatedConfig");
    EXPECT_TRUE(provider.getServerNodes().empty());
    EXPECT_TRUE(provider.getClientNodes().empty());
    ASSERT_EQ(provider.getPeerNodes().size(), 1U);
    EXPECT_EQ(
        provider.getPeerNodes().at("peer-updated").getEndpoint().getAddress(),
        "10.0.0.1");
    EXPECT_TRUE(provider.getTimers().empty());
    EXPECT_TRUE(provider.getFilesystems().empty());
    ASSERT_EQ(provider.getActions().size(), 1U);
    EXPECT_EQ(provider.getActions().at("action-updated").getType(),
              ConnixCore::Infrastructure::Configuration::ActionType::RESPOND);
}

} // namespace IntegrationTest
} // namespace ConnixCore
