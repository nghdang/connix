#pragma once

#include "ConnixCore/Communication/NodeLayer/IServerConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

class ServerConfiguration
    : public ConnixCore::Communication::IServerConfiguration
{
public:
    ~ServerConfiguration() override = default;

    ConnixCore::Communication::TransportType getTransportType() const override;

    void setTransportType(
        const ConnixCore::Communication::TransportType transportType) override;

    std::uint32_t getMaximumConnections() const override;

    void
    setMaximumConnections(const std::uint32_t maximumConnections) override;

    std::uint32_t getMaximumRequestsPerSecond() const override;

    void setMaximumRequestsPerSecond(
        const std::uint32_t maximumRequestsPerSecond) override;

    std::uint32_t getBufferSize() const override;

    void setBufferSize(const std::uint32_t bufferSize) override;

private:
    ConnixCore::Communication::TransportType m_transportType;

    std::uint32_t m_maximumConnections;

    std::uint32_t m_maximumRequestsPerSecond;

    std::uint32_t m_bufferSize;
};

} // namespace Communication
} // namespace ConnixCore
