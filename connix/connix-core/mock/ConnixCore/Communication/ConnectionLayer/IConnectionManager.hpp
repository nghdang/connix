#pragma once

#include <cstddef>
#include <memory>

#include "ConnixCore/Communication/ConnectionLayer/IConnection.hpp"
#include "ConnixCore/Communication/ConnectionLayer/IConnectionSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class IConnectionManager
{
public:
    virtual ~IConnectionManager() = default;

    virtual bool addSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) = 0;

    virtual bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) = 0;

    virtual std::size_t getConnectionCount() const = 0;

    virtual bool addConnection(
        std::shared_ptr<ConnixCore::Communication::IConnection> connection)
        const = 0;

    virtual bool
    removeConnection(ConnixCore::Communication::ConnectionId id) = 0;
};

} // namespace Communication
} // namespace ConnixCore
