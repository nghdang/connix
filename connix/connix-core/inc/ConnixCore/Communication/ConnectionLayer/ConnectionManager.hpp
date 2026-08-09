#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

class ConnectionManager : public ConnixCore::Communication::IConnectionManager
{
public:
    explicit ConnectionManager(
        const std::vector<
            std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>&
            subscribers);

    virtual ~ConnectionManager() override = default;

    bool addSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) override;

    bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) override;

    std::size_t getConnectionCount() const override;

    bool addConnection(std::shared_ptr<ConnixCore::Communication::IConnection>
                           connection) const override;

    bool removeConnection(ConnixCore::Communication::ConnectionId id) override;

private:
    std::vector<
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>
        m_subscribers;

    std::vector<std::shared_ptr<ConnixCore::Communication::IConnection>>
        m_connections;
};

} // namespace Communication
} // namespace ConnixCore
