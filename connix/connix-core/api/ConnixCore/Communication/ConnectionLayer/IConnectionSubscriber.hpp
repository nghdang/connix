#pragma once

#include <vector>

#include "ConnixCore/Communication/ConnectionLayer/ConnectionId.hpp"

namespace ConnixCore {
namespace Communication {

class IConnectionSubscriber
{
public:
    virtual ~IConnectionSubscriber() = default;

    virtual void
    onStateChanged(ConnixCore::Communication::ConnectionId id) = 0;

    virtual bool onDataReceived(ConnixCore::Communication::ConnectionId id,
                                std::vector<std::uint8_t>& receivedData,
                                std::vector<std::uint8_t>& responseData) = 0;
};

} // namespace Communication
} // namespace ConnixCore
