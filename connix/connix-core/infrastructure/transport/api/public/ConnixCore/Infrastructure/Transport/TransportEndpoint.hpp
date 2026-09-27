#pragma once

#include <cstdint>
#include <string>

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

class TransportEndpoint
{
public:
    TransportEndpoint();
    TransportEndpoint(const std::string& address, std::uint16_t port);
    explicit TransportEndpoint(const std::string& path);
    ~TransportEndpoint() = default;

    const std::string& getAddress() const;
    std::uint16_t getPort() const;
    bool isUnixDomain() const;
    std::string toString() const;

    bool operator==(const TransportEndpoint& other) const;
    bool operator!=(const TransportEndpoint& other) const;

private:
    std::string m_address;
    std::uint16_t m_port;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
