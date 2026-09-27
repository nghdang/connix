#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

class ISocket
{
public:
    virtual ~ISocket() = default;

    virtual void open(SocketDomain domain, SocketType type,
                      SocketProtocol protocol) = 0;
    virtual void bind(const TransportEndpoint& endpoint) = 0;
    virtual void listen(std::int32_t backlog) = 0;
    virtual std::unique_ptr<ISocket> accept(std::uint32_t timeoutMs) = 0;
    virtual void connect(const TransportEndpoint& endpoint,
                         std::uint32_t timeoutMs) = 0;
    virtual std::size_t send(const std::vector<std::uint8_t>& data,
                             std::uint32_t timeoutMs) = 0;
    virtual std::vector<std::uint8_t> receive(std::size_t maxBytes,
                                              std::uint32_t timeoutMs) = 0;
    virtual std::size_t sendTo(const std::vector<std::uint8_t>& data,
                               const TransportEndpoint& destination,
                               std::uint32_t timeoutMs) = 0;
    virtual std::vector<std::uint8_t> receiveFrom(std::size_t maxBytes,
                                                  TransportEndpoint& source,
                                                  std::uint32_t timeoutMs) = 0;
    virtual void setOption(SocketOption option, bool enable) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual std::int32_t getNativeHandle() const = 0;
    virtual const TransportEndpoint& getLocalEndpoint() const = 0;
    virtual const TransportEndpoint& getRemoteEndpoint() const = 0;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
