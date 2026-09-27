#pragma once

#include <functional>
#include <memory>

#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

using TransportCreator = std::function<std::unique_ptr<ITransport>()>;

class ITransportFactory
{
public:
    virtual ~ITransportFactory() = default;

    virtual std::unique_ptr<ITransport>
    createTransport(TransportProtocol protocol) = 0;
    virtual void registerTransport(TransportProtocol protocol,
                                   TransportCreator creator) = 0;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
