#pragma once

#include <cstdint>
#include <vector>

#include "ConnixCore/Communication/TransportLayer/TransportId.hpp"

namespace ConnixCore {
namespace Communication {

class ITransportSubscriber
{
public:
    virtual ~ITransportSubscriber() = default;

    virtual void onStateChanged(ConnixCore::Communication::TransportId id) = 0;

    virtual void
    onStatusChanged(ConnixCore::Communication::TransportId id) = 0;

    virtual bool onDataReceived(ConnixCore::Communication::TransportId id,
                                std::vector<std::uint8_t>& receivedData,
                                std::vector<std::uint8_t>& responseData) = 0;
};

} // namespace Communication
} // namespace ConnixCore
