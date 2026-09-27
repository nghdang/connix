#pragma once

#include <string>

#include "ConnixCore/Infrastructure/Configuration/ConnixConfig.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonParser.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class JsonParser : public IJsonParser
{
public:
    JsonParser() = default;
    ~JsonParser() override = default;

    ConnixConfig parse(const std::string& jsonStr) const override;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
