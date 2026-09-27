#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

class ITransport
{
public:
    virtual ~ITransport() = default;

    virtual void bind(const TransportEndpoint& endpoint) = 0;
    virtual void listen(std::uint32_t backlog) = 0;
    virtual std::unique_ptr<ITransport> accept(std::uint32_t timeoutMs) = 0;
    virtual void connect(const TransportEndpoint& endpoint,
                         std::uint32_t timeoutMs) = 0;
    virtual std::size_t send(const std::vector<std::uint8_t>& data,
                             std::uint32_t timeoutMs) = 0;
    virtual std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                              std::uint32_t timeoutMs) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual TransportState getState() const = 0;
    virtual TransportProtocol getProtocol() const = 0;
    virtual const TransportEndpoint& getLocalEndpoint() const = 0;
    virtual const TransportEndpoint& getRemoteEndpoint() const = 0;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
