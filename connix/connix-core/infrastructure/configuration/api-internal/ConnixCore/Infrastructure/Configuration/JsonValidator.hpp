#pragma once

#include <string>

#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class JsonValidator : public IJsonValidator
{
public:
    JsonValidator() = default;
    ~JsonValidator() override = default;

    void validate(const std::string& jsonStr,
                  const std::string& schemaStr) const override;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
