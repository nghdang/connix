#pragma once

#include <gmock/gmock.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Configuration/ActionConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ClientNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/FilesystemConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IConfigurationProvider.hpp"
#include "ConnixCore/Infrastructure/Configuration/PeerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/ServerNodeConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/TimerConfig.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class MockIConfigurationProvider : public IConfigurationProvider
{
public:
    MockIConfigurationProvider() = default;
    ~MockIConfigurationProvider() override = default;

    MOCK_METHOD(void, load,
                (const std::string& configPath, const std::string& schemaPath),
                (override));
    MOCK_METHOD(const std::string&, getName, (), (const, override));
    MOCK_METHOD((const std::unordered_map<std::string, ServerNodeConfig>&),
                getServerNodes, (), (const, override));
    MOCK_METHOD((const std::unordered_map<std::string, ClientNodeConfig>&),
                getClientNodes, (), (const, override));
    MOCK_METHOD((const std::unordered_map<std::string, PeerNodeConfig>&),
                getPeerNodes, (), (const, override));
    MOCK_METHOD((const std::unordered_map<std::string, TimerConfig>&),
                getTimers, (), (const, override));
    MOCK_METHOD((const std::unordered_map<std::string, FilesystemConfig>&),
                getFilesystems, (), (const, override));
    MOCK_METHOD((const std::unordered_map<std::string, ActionConfig>&),
                getActions, (), (const, override));

    static std::shared_ptr<MockIConfigurationProvider> create();
    static std::shared_ptr<::testing::NiceMock<MockIConfigurationProvider>>
    createNice();
    static std::shared_ptr<::testing::StrictMock<MockIConfigurationProvider>>
    createStrict();
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
