#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"

#include <cstdint>
#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

TransportEndpoint::TransportEndpoint()
    : m_address()
    , m_port(0)
{
}

TransportEndpoint::TransportEndpoint(const std::string& address,
                                     std::uint16_t port)
    : m_address(address)
    , m_port(port)
{
}

TransportEndpoint::TransportEndpoint(const std::string& path)
    : m_address(path)
    , m_port(0)
{
}

const std::string& TransportEndpoint::getAddress() const
{
    return m_address;
}

std::uint16_t TransportEndpoint::getPort() const
{
    return m_port;
}

bool TransportEndpoint::isUnixDomain() const
{
    if (m_address.empty())
    {
        return false;
    }
    return m_port == 0 || m_address.front() == '/' || m_address.front() == '.';
}

std::string TransportEndpoint::toString() const
{
    if (m_address.empty() && m_port == 0)
    {
        return "";
    }
    if (isUnixDomain() && m_port == 0)
    {
        return "unix:" + m_address;
    }
    return m_address + ":" + std::to_string(m_port);
}

bool TransportEndpoint::operator==(const TransportEndpoint& other) const
{
    return m_address == other.m_address && m_port == other.m_port;
}

bool TransportEndpoint::operator!=(const TransportEndpoint& other) const
{
    return !(*this == other);
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
