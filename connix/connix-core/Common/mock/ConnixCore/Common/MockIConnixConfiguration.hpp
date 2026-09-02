#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Common/IConnixConfiguration.hpp"

namespace ConnixCore {
namespace Common {

class MockIConnixConfiguration
    : public ConnixCore::Common::IConnixConfiguration
{
public:
    MOCK_METHOD(void, getConfig, (const std::string& name, std::string& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::uint8_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::uint16_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::uint32_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::uint64_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig, (const std::string& name, std::int8_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::int16_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::int32_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig,
                (const std::string& name, std::int64_t& value),
                (const, override));

    MOCK_METHOD(void, getConfig, (const std::string& name, bool& value),
                (const, override));

    MOCK_METHOD(void, getConfig, (const std::string& name, float& value),
                (const, override));

    MOCK_METHOD(void, getConfig, (const std::string& name, double& value),
                (const, override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::string& value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::uint8_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::uint16_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::uint32_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::uint64_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::int8_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::int16_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::int32_t value),
                (override));

    MOCK_METHOD(void, setConfig,
                (const std::string& name, const std::int64_t value),
                (override));

    MOCK_METHOD(void, setConfig, (const std::string& name, const bool value),
                (override));

    MOCK_METHOD(void, setConfig, (const std::string& name, const float value),
                (override));

    MOCK_METHOD(void, setConfig, (const std::string& name, const double value),
                (override));

    static std::shared_ptr<ConnixCore::Common::MockIConnixConfiguration>
    create();

    static std::shared_ptr<
        ::testing::StrictMock<ConnixCore::Common::MockIConnixConfiguration>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Common::MockIConnixConfiguration>>
    createNice();
};

} // namespace Common
} // namespace ConnixCore
