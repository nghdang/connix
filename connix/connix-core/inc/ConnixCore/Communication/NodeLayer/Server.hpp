#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionFactory.hpp"
#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"
#include "ConnixCore/Communication/NodeLayer/IServer.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransportFactory.hpp"

namespace ConnixCore {
namespace Communication {

class Server : public ConnixCore::Communication::IServer
{
public:
    explicit Server(
        std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
            configuration);

    ~Server() override = default;

    std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
    getConfiguration() const override;

    bool
    addSubscriber(std::shared_ptr<ConnixCore::Communication::IServerSubscriber>
                      subscriber) override;

    bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IServerSubscriber>
            subscriber) override;

    bool start(const std::string& address, const std::uint16_t port) override;

    bool stop() override;

    bool send(const ConnixCore::Communication::ConnectionId connectionId,
              const std::vector<std::uint8_t>& data) override;

    bool closeConnection(
        const ConnixCore::Communication::ConnectionId connectionId) override;

private:
    std::shared_ptr<const ConnixCore::Communication::IServerConfiguration>
        m_configuration;

    std::vector<std::shared_ptr<ConnixCore::Communication::IServerSubscriber>>
        m_subscribers;

    std::shared_ptr<ConnixCore::Communication::IConnectionFactory>
        m_connectionFactory;

    std::shared_ptr<ConnixCore::Communication::IConnectionManager>
        m_connectionManager;

    std::shared_ptr<ConnixCore::Communication::ITransportFactory>
        m_transportFactory;
};

} // namespace Communication
} // namespace ConnixCore
