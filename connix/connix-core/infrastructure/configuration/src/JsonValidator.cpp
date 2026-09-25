#include "ConnixCore/Infrastructure/Configuration/JsonValidator.hpp"

#include <exception>
#include <nlohmann/json-schema.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

void JsonValidator::validate(const std::string& jsonStr,
                             const std::string& schemaStr) const
{
    try
    {
        const nlohmann::json schemaJson = nlohmann::json::parse(schemaStr);
        nlohmann::json_schema::json_validator validator;
        validator.set_root_schema(schemaJson);

        const nlohmann::json docJson = nlohmann::json::parse(jsonStr);
        validator.validate(docJson);
    } catch (const std::exception& ex)
    {
        throw ConfigurationException(
            ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED, ex.what());
    }
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
