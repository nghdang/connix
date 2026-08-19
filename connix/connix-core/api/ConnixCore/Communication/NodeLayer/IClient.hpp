#pragma once

#include "ConnixCore/Communication/ConnectionLayer/ConnectionId.hpp"
#include "ConnixCore/Communication/NodeLayer/IClientConfiguration.hpp"
#include "ConnixCore/Communication/NodeLayer/IClientSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class IClient
{
public:
    virtual ~IClient() = default;

    virtual std::shared_ptr<
        const ConnixCore::Communication::IClientConfiguration>
    getConfiguration() const = 0;

    virtual bool
    addSubscriber(std::shared_ptr<ConnixCore::Communication::IClientSubscriber>
                      subscriber) = 0;

    virtual bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IClientSubscriber>
            subscriber) = 0;

    virtual bool connect(const std::string& address,
                         const std::uint16_t port) = 0;

    virtual bool disconnect() = 0;

    virtual bool
    send(const ConnixCore::Communication::ConnectionId connectionId,
         const std::vector<std::uint8_t>& data) = 0;

    virtual bool closeConnection(
        const ConnixCore::Communication::ConnectionId connectionId) = 0;
};

} // namespace Communication
} // namespace ConnixCore
