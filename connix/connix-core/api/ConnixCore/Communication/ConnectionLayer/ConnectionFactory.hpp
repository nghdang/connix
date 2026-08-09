#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionFactory.hpp"

namespace ConnixCore {
namespace Communication {

class ConnectionFactory : public ConnixCore::Communication::IConnectionFactory
{
public:
    explicit ConnectionFactory();

    virtual ~ConnectionFactory() override = default;

    std::shared_ptr<ConnixCore::Communication::IConnectionManager>
    createConnectionManager() override;

    std::shared_ptr<ConnixCore::Communication::IConnection> createConnection(
        ConnixCore::Communication::ConnectionId id,
        const ConnixCore::Communication::ConnectionEndpoint& remote,
        const ConnixCore::Communication::ConnectionEndpoint& local,
        const std::vector<
            std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>&
            subscribers) override;

private:
    ConnixCore::Communication::ConnectionId m_connectionIdGenerator;
};

} // namespace Communication
} // namespace ConnixCore
