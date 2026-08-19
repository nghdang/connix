#pragma once

#include <cstdint>

#include "ConnixCore/Communication/TransportLayer/TransportType.hpp"

namespace ConnixCore {
namespace Communication {

class IClientConfiguration
{
public:
    virtual ~IClientConfiguration() = default;

    virtual ConnixCore::Communication::TransportType
    getTransportType() const = 0;

    virtual void setTransportType(
        const ConnixCore::Communication::TransportType transportType) = 0;

    virtual std::uint32_t getConnectionTimeout() const = 0;

    virtual void
    setConnectionTimeout(const std::uint32_t connectionTimeout) = 0;

    virtual bool getKeepConnectionAlive() const = 0;

    virtual void setKeepConnectionAlive(const bool keepConnectionAlive) = 0;

    virtual std::uint32_t getBufferSize() const = 0;

    virtual void setBufferSize(const std::uint32_t bufferSize) = 0;
};

} // namespace Communication
} // namespace ConnixCore
