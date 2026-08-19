#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"

namespace ConnixCore {
namespace Communication {

class IConnectionFactory
{
public:
    virtual ~IConnectionFactory() = default;

    virtual std::shared_ptr<ConnixCore::Communication::IConnectionManager>
    createConnectionManager() = 0;

    virtual std::shared_ptr<ConnixCore::Communication::IConnection>
    createConnection(
        const ConnixCore::Communication::ConnectionEndpoint& remote,
        const ConnixCore::Communication::ConnectionEndpoint& local,
        std::shared_ptr<ConnixCore::Communication::ITransport> transport) = 0;
};

} // namespace Communication
} // namespace ConnixCore
