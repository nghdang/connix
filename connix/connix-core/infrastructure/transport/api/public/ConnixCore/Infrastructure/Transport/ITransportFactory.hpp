#pragma once

#include <functional>
#include <memory>

#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Factory functor alias for creating ITransport instances.
 */
using TransportCreator = std::function<std::unique_ptr<ITransport>()>;

/**
 * @brief Abstract factory interface for creating and registering transports.
 */
class ITransportFactory
{
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~ITransportFactory() = default;

    /**
     * @brief Creates a concrete transport instance for the given protocol.
     * @param protocol TransportProtocol to instantiate.
     * @return Unique pointer to the instantiated ITransport.
     */
    virtual std::unique_ptr<ITransport>
    createTransport(TransportProtocol protocol) = 0;

    /**
     * @brief Registers a custom or mock transport creator for a protocol.
     * @param protocol TransportProtocol to associate with creator.
     * @param creator Factory functor producing ITransport instances.
     */
    virtual void registerTransport(TransportProtocol protocol,
                                   TransportCreator creator) = 0;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
