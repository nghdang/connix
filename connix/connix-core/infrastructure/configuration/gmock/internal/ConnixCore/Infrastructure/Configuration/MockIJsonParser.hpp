#pragma once

#include <gmock/gmock.h>

#include <string>

#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonParser.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class MockIJsonParser : public IJsonParser
{
public:
    ~MockIJsonParser() override = default;

    MOCK_METHOD(ConnixConfig, parse, (const std::string& jsonStr),
                (const, override));
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
