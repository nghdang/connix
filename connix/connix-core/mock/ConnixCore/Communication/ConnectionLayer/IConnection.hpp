#pragma once

#include <vector>

#include "ConnixCore/Communication/ConnectionLayer/ConnectionEndpoint.hpp"
#include "ConnixCore/Communication/ConnectionLayer/ConnectionId.hpp"
#include "ConnixCore/Communication/ConnectionLayer/ConnectionState.hpp"

namespace ConnixCore {
namespace Communication {

class IConnection
{
public:
    virtual ~IConnection() = default;

    virtual ConnixCore::Communication::ConnectionId getId() const = 0;

    virtual ConnixCore::Communication::ConnectionState getState() const = 0;

    virtual ConnixCore::Communication::ConnectionEndpoint
    getRemote() const = 0;

    virtual ConnixCore::Communication::ConnectionEndpoint getLocal() const = 0;

    virtual bool send(const std::vector<std::uint8_t>& data) = 0;

    virtual void close() = 0;
};

} // namespace Communication
} // namespace ConnixCore
