#include "ConnixCore/Infrastructure/Configuration/Endpoint.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <utility>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

Endpoint::Endpoint(std::string address, std::optional<std::uint16_t> port)
    : m_address(std::move(address))
    , m_port(port)
{
}

const std::string& Endpoint::getAddress() const
{
    return m_address;
}

const std::optional<std::uint16_t>& Endpoint::getPort() const
{
    return m_port;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
