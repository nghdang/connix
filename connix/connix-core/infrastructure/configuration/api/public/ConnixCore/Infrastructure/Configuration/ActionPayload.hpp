#pragma once

#include <string>

#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class ActionPayload
{
public:
    ActionPayload(PayloadType type, std::string source);

    PayloadType getType() const;
    const std::string& getSource() const;

private:
    PayloadType m_type;
    std::string m_source;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
