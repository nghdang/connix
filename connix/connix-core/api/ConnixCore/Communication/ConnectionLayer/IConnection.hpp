#pragma once

#include <memory>
#include <vector>

#include "ConnixCore/Communication/ConnectionLayer/ConnectionEndpoint.hpp"
#include "ConnixCore/Communication/ConnectionLayer/ConnectionId.hpp"
#include "ConnixCore/Communication/ConnectionLayer/ConnectionState.hpp"
#include "ConnixCore/Communication/ConnectionLayer/IConnectionSubscriber.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransportSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class IConnection : private ConnixCore::Communication::ITransportSubscriber
{
public:
    virtual ~IConnection() = default;

    virtual ConnixCore::Communication::ConnectionId getId() const = 0;

    virtual ConnixCore::Communication::ConnectionEndpoint
    getRemote() const = 0;

    virtual ConnixCore::Communication::ConnectionEndpoint getLocal() const = 0;

    virtual ConnixCore::Communication::ConnectionState getState() const = 0;

    virtual bool addSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) = 0;

    virtual bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) = 0;

    virtual bool send(const std::vector<std::uint8_t>& data) = 0;

    virtual void close() = 0;
};

} // namespace Communication
} // namespace ConnixCore
