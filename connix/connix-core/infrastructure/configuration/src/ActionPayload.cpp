#include "ConnixCore/Infrastructure/Configuration/ActionPayload.hpp"

#include <string>
#include <utility>

#include "ConnixCore/Infrastructure/Configuration/PayloadType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

ActionPayload::ActionPayload(PayloadType type, std::string source)
    : m_type(type)
    , m_source(std::move(source))
{
}

PayloadType ActionPayload::getType() const
{
    return m_type;
}

const std::string& ActionPayload::getSource() const
{
    return m_source;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
