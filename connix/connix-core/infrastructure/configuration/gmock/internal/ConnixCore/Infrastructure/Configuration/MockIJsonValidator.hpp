#pragma once

#include <gmock/gmock.h>

#include <string>

#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class MockIJsonValidator : public IJsonValidator
{
public:
    ~MockIJsonValidator() override = default;

    MOCK_METHOD(void, validate,
                (const std::string& jsonStr, const std::string& schemaStr),
                (const, override));
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
