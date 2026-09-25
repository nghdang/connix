#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <memory>
#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/IFileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIFileReader.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIJsonParser.hpp"
#include "ConnixCore/Infrastructure/Configuration/MockIJsonValidator.hpp"

using namespace testing;
using namespace ConnixCore::Infrastructure::Configuration;

namespace ConnixCore {
namespace UnitTest {

TEST(ConfigurationInterfacesTest, FileReaderPolymorphism)
{
    const std::unique_ptr<IFileReader> reader =
        std::make_unique<MockIFileReader>();
    auto* mockReader = dynamic_cast<MockIFileReader*>(reader.get());
    ASSERT_NE(mockReader, nullptr);

    EXPECT_CALL(*mockReader, readAll("test_path.json"))
        .WillOnce(Return("content of test_path.json"));

    EXPECT_EQ(reader->readAll("test_path.json"), "content of test_path.json");
}

TEST(ConfigurationInterfacesTest, JsonValidatorPolymorphism)
{
    const std::unique_ptr<IJsonValidator> validator =
        std::make_unique<MockIJsonValidator>();
    auto* mockValidator = dynamic_cast<MockIJsonValidator*>(validator.get());
    ASSERT_NE(mockValidator, nullptr);

    EXPECT_CALL(*mockValidator, validate("{}", "{}")).Times(1);

    EXPECT_NO_THROW(validator->validate("{}", "{}"));
}

TEST(ConfigurationInterfacesTest, JsonParserPolymorphism)
{
    const std::unique_ptr<IJsonParser> parser =
        std::make_unique<MockIJsonParser>();
    auto* mockParser = dynamic_cast<MockIJsonParser*>(parser.get());
    ASSERT_NE(mockParser, nullptr);

    EXPECT_CALL(*mockParser, parse("{}"))
        .WillOnce(
            Return(ConnixConfig("parsed_config", {}, {}, {}, {}, {}, {})));

    const ConnixConfig config = parser->parse("{}");
    EXPECT_EQ(config.getName(), "parsed_config");
}

TEST(ConfigurationInterfacesTest, ConfigurationProviderPolymorphism)
{
    const std::unique_ptr<IConfigurationProvider> provider =
        std::make_unique<MockIConfigurationProvider>();
    auto* mockProvider =
        dynamic_cast<MockIConfigurationProvider*>(provider.get());
    ASSERT_NE(mockProvider, nullptr);

    std::string expectedName = "mock_provider";
    const std::unordered_map<std::string, ServerNodeConfig> emptyServers;

    EXPECT_CALL(*mockProvider, load("config.json", "schema.json")).Times(1);
    EXPECT_CALL(*mockProvider, getName()).WillOnce(ReturnRef(expectedName));
    EXPECT_CALL(*mockProvider, getServerNodes())
        .WillOnce(ReturnRef(emptyServers));

    EXPECT_NO_THROW(provider->load("config.json", "schema.json"));
    EXPECT_EQ(provider->getName(), "mock_provider");
    EXPECT_TRUE(provider->getServerNodes().empty());
}

TEST(ConfigurationInterfacesTest, MockConfigurationProviderFactoryMethods)
{
    const auto normalMock = MockIConfigurationProvider::create();
    ASSERT_NE(normalMock, nullptr);

    const auto niceMock = MockIConfigurationProvider::createNice();
    ASSERT_NE(niceMock, nullptr);

    const auto strictMock = MockIConfigurationProvider::createStrict();
    ASSERT_NE(strictMock, nullptr);

    std::string name = "test";
    EXPECT_CALL(*normalMock, getName()).WillOnce(ReturnRef(name));
    EXPECT_EQ(normalMock->getName(), "test");
}

} // namespace UnitTest
} // namespace ConnixCore
