#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "ConnixCore/Infrastructure/Transport/ISocket.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransport.hpp"
#include "ConnixCore/Infrastructure/Transport/ITransportFactory.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Factory functor alias for creating ISocket instances.
 */
using SocketCreator = std::function<std::unique_ptr<ISocket>()>;

/**
 * @brief Concrete factory for creating and registering ITransport instances.
 *
 * Implements ITransportFactory to instantiate concrete transports (TCP, UDP,
 * UDS Stream, UDS Datagram) and injects underlying socket abstractions.
 * Supports custom transport registration for protocol extensibility.
 */
class TransportFactory : public ITransportFactory
{
public:
    /**
     * @brief Constructs a TransportFactory with an optional socket creator.
     * @param socketCreator Functor producing ISocket instances; defaults to
     * concrete POSIX Socket instantiation if null.
     */
    explicit TransportFactory(SocketCreator socketCreator = nullptr);

    /**
     * @brief Default virtual destructor.
     */
    ~TransportFactory() override = default;

    /**
     * @brief Deleted copy constructor.
     */
    TransportFactory(const TransportFactory&) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    TransportFactory& operator=(const TransportFactory&) = delete;

    /**
     * @brief Move constructor.
     */
    TransportFactory(TransportFactory&&) noexcept = default;

    /**
     * @brief Move assignment operator.
     */
    TransportFactory& operator=(TransportFactory&&) noexcept = default;

    std::unique_ptr<ITransport>
    createTransport(TransportProtocol protocol) override;

    void registerTransport(TransportProtocol protocol,
                           TransportCreator creator) override;

private:
    SocketCreator m_socketCreator;
    std::unordered_map<TransportProtocol, TransportCreator> m_creators;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
