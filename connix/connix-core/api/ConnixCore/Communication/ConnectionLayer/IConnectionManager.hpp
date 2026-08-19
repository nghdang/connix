#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnection.hpp"

namespace ConnixCore {
namespace Communication {

class IConnectionManager
{
public:
    virtual ~IConnectionManager() = default;

    virtual std::size_t getConnectionCount() const = 0;

    virtual bool addConnection(
        std::shared_ptr<ConnixCore::Communication::IConnection> connection)
        const = 0;

    virtual bool
    removeConnection(ConnixCore::Communication::ConnectionId id) = 0;
};

} // namespace Communication
} // namespace ConnixCore
