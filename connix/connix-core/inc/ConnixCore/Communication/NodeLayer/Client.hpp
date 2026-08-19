#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionFactory.hpp"
#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"
#include "ConnixCore/Communication/NodeLayer/IClient.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransportFactory.hpp"

namespace ConnixCore {
namespace Communication {

class Client : public ConnixCore::Communication::IClient
{
public:
    explicit Client(
        std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
            configuration);

    ~Client() override = default;

    std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
    getConfiguration() const override;

    bool
    addSubscriber(std::shared_ptr<ConnixCore::Communication::IClientSubscriber>
                      subscriber) override;

    bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IClientSubscriber>
            subscriber) override;

    bool connect(const std::string& address,
                 const std::uint16_t port) override;

    bool disconnect() override;

    bool send(const ConnixCore::Communication::ConnectionId connectionId,
              const std::vector<std::uint8_t>& data) override;

    bool closeConnection(
        const ConnixCore::Communication::ConnectionId connectionId) override;

private:
    std::shared_ptr<const ConnixCore::Communication::IClientConfiguration>
        m_configuration;

    std::vector<std::shared_ptr<ConnixCore::Communication::IClientSubscriber>>
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
