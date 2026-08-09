#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "ConnixCore/Communication/TransportLayer/ITransportSubscriber.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportId.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportState.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportStatus.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportType.hpp"

namespace ConnixCore {
namespace Communication {

class ITransport
{
public:
    virtual ~ITransport() = default;

    virtual ConnixCore::Communication::TransportId getId() const = 0;

    virtual ConnixCore::Communication::TransportType getType() const = 0;

    virtual ConnixCore::Communication::TransportState getState() const = 0;

    virtual ConnixCore::Communication::TransportStatus getStatus() const = 0;

    virtual bool addSubscriber(
        std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
            subscriber) = 0;

    virtual bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
            subscriber) = 0;

    virtual bool send(const std::vector<std::uint8_t>& data) = 0;

    virtual void close() = 0;
};

} // namespace Communication
} // namespace ConnixCore
