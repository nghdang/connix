#include "gtest/gtest.h"

#include <memory>
#include <string>

#include "ConnixCore/Infrastructure/Configuration/ConfigurationException.hpp"
#include "ConnixCore/Infrastructure/Configuration/IJsonValidator.hpp"
#include "ConnixCore/Infrastructure/Configuration/JsonValidator.hpp"

namespace ConnixCore {
namespace UnitTest {

TEST(JsonValidatorTest, ValidJsonMatchesSchema)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({
        "type": "object",
        "properties": {
            "name": { "type": "string" },
            "count": { "type": "integer" }
        },
        "required": ["name", "count"]
    })";
    const std::string json = R"({
        "name": "Connix",
        "count": 42
    })";

    EXPECT_NO_THROW(validator.validate(json, schema));
}

TEST(JsonValidatorTest, ValidJsonWithDefinitions)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({
        "type": "object",
        "properties": {
            "endpoint": { "$ref": "#/$defs/endpointDef" }
        },
        "required": ["endpoint"],
        "$defs": {
            "endpointDef": {
                "type": "object",
                "properties": {
                    "address": { "type": "string" },
                    "port": { "type": "integer", "minimum": 1, "maximum": 65535 }
                },
                "required": ["address", "port"]
            }
        }
    })";
    const std::string json = R"({
        "endpoint": {
            "address": "127.0.0.1",
            "port": 8080
        }
    })";

    EXPECT_NO_THROW(validator.validate(json, schema));
}

TEST(JsonValidatorTest, ThrowsWhenJsonSyntaxIsInvalid)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({ "type": "object" })";
    const std::string invalidJson = "{ not a valid json }";

    try
    {
        validator.validate(invalidJson, schema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, ThrowsWhenSchemaSyntaxIsInvalid)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string invalidSchema = "invalid schema {";
    const std::string json = R"({ "name": "Connix" })";

    try
    {
        validator.validate(json, invalidSchema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, ThrowsWhenSchemaSemanticsAreInvalid)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string invalidSchema = R"({ "type": "unsupported_type_name" })";
    const std::string json = R"({ "name": "Connix" })";

    try
    {
        validator.validate(json, invalidSchema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, ThrowsWhenRequiredFieldIsMissing)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({
        "type": "object",
        "properties": {
            "name": { "type": "string" },
            "mandatory": { "type": "string" }
        },
        "required": ["name", "mandatory"]
    })";
    const std::string json = R"({ "name": "Connix" })";

    try
    {
        validator.validate(json, schema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, ThrowsWhenFieldTypeMismatches)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({
        "type": "object",
        "properties": {
            "port": { "type": "integer" }
        },
        "required": ["port"]
    })";
    const std::string json = R"({ "port": "not-an-integer" })";

    try
    {
        validator.validate(json, schema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, ThrowsWhenConstraintViolated)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({
        "type": "object",
        "properties": {
            "code": { "type": "string", "pattern": "^[0-9]{3}$" }
        },
        "required": ["code"]
    })";
    const std::string json = R"({ "code": "ABC" })";

    try
    {
        validator.validate(json, schema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, ThrowsWhenAdditionalPropertiesForbidden)
{
    const ConnixCore::Infrastructure::Configuration::JsonValidator validator;
    const std::string schema = R"({
        "type": "object",
        "properties": {
            "name": { "type": "string" }
        },
        "additionalProperties": false
    })";
    const std::string json = R"({ "name": "Connix", "unexpected": true })";

    try
    {
        validator.validate(json, schema);
        FAIL() << "Expected ConfigurationException";
    } catch (const ConnixCore::Infrastructure::Configuration::
                 ConfigurationException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(),
                  ConnixCore::Infrastructure::Configuration::
                      ConfigurationErrorCode::SCHEMA_VALIDATION_FAILED);
    }
}

TEST(JsonValidatorTest, PolymorphismViaInterface)
{
    const std::unique_ptr<
        ConnixCore::Infrastructure::Configuration::IJsonValidator>
        validator = std::make_unique<
            ConnixCore::Infrastructure::Configuration::JsonValidator>();
    const std::string schema = R"({ "type": "object" })";
    const std::string json = R"({})";

    EXPECT_NO_THROW(validator->validate(json, schema));
}

} // namespace UnitTest
} // namespace ConnixCore
