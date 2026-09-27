#pragma once

#include <gmock/gmock.h>

#include <string>

#include "ConnixCore/Infrastructure/Configuration/IFileReader.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class MockIFileReader : public IFileReader
{
public:
    ~MockIFileReader() override = default;

    MOCK_METHOD(std::string, readAll, (const std::string& filePath),
                (const, override));
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
