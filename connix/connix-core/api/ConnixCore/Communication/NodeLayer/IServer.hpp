#pragma once

#include "ConnixCore/Communication/ConnectionLayer/ConnectionId.hpp"
#include "ConnixCore/Communication/NodeLayer/IServerConfiguration.hpp"
#include "ConnixCore/Communication/NodeLayer/IServerSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class IServer
{
public:
    virtual ~IServer() = default;

    virtual std::shared_ptr<
        const ConnixCore::Communication::IServerConfiguration>
    getConfiguration() const = 0;

    virtual bool
    addSubscriber(std::shared_ptr<ConnixCore::Communication::IServerSubscriber>
                      subscriber) = 0;

    virtual bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IServerSubscriber>
            subscriber) = 0;

    virtual bool start(const std::string& address,
                       const std::uint16_t port) = 0;

    virtual bool stop() = 0;

    virtual bool
    send(const ConnixCore::Communication::ConnectionId connectionId,
         const std::vector<std::uint8_t>& data) = 0;

    virtual bool closeConnection(
        const ConnixCore::Communication::ConnectionId connectionId) = 0;
};

} // namespace Communication
} // namespace ConnixCore
