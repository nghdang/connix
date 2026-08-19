#pragma once

#include <cstdint>

#include "ConnixCore/Communication/TransportLayer/TransportType.hpp"

namespace ConnixCore {
namespace Communication {

class IServerConfiguration
{
public:
    virtual ~IServerConfiguration() = default;

    virtual ConnixCore::Communication::TransportType
    getTransportType() const = 0;

    virtual void setTransportType(
        const ConnixCore::Communication::TransportType transportType) = 0;

    virtual std::uint32_t getMaximumConnections() const = 0;

    virtual void
    setMaximumConnections(const std::uint32_t maximumConnections) = 0;

    virtual std::uint32_t getMaximumRequestsPerSecond() const = 0;

    virtual void setMaximumRequestsPerSecond(
        const std::uint32_t maximumRequestsPerSecond) = 0;

    virtual std::uint32_t getBufferSize() const = 0;

    virtual void setBufferSize(const std::uint32_t bufferSize) = 0;
};

} // namespace Communication
} // namespace ConnixCore
