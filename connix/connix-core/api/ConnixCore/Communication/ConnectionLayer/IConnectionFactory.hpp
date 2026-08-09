#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"

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
        ConnixCore::Communication::ConnectionId id,
        const ConnixCore::Communication::ConnectionEndpoint& remote,
        const ConnixCore::Communication::ConnectionEndpoint& local,
        const std::vector<
            std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>&
            subscribers) = 0;
};

} // namespace Communication
} // namespace ConnixCore
