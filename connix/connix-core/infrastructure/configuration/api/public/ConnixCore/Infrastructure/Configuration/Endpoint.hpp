#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class Endpoint
{
public:
    Endpoint(std::string address, std::optional<std::uint16_t> port);

    const std::string& getAddress() const;
    const std::optional<std::uint16_t>& getPort() const;

private:
    std::string m_address;
    std::optional<std::uint16_t> m_port;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
