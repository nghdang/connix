#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

class ConnectionManager : public ConnixCore::Communication::IConnectionManager
{
public:
    explicit ConnectionManager();

    virtual ~ConnectionManager() override = default;

    std::size_t getConnectionCount() const override;

    bool addConnection(std::shared_ptr<ConnixCore::Communication::IConnection>
                           connection) const override;

    bool removeConnection(ConnixCore::Communication::ConnectionId id) override;

private:
    std::vector<std::shared_ptr<ConnixCore::Communication::IConnection>>
        m_connections;
};

} // namespace Communication
} // namespace ConnixCore
