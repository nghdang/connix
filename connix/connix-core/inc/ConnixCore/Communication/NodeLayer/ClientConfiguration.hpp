#pragma once

#include "ConnixCore/Communication/NodeLayer/IClientConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

class ClientConfiguration
    : public ConnixCore::Communication::IClientConfiguration
{
public:
    ~ClientConfiguration() override = default;

    ConnixCore::Communication::TransportType getTransportType() const override;

    void setTransportType(
        const ConnixCore::Communication::TransportType transportType) override;

    std::uint32_t getConnectionTimeout() const override;

    void setConnectionTimeout(const std::uint32_t connectionTimeout) override;

    bool getKeepConnectionAlive() const override;

    void setKeepConnectionAlive(const bool keepConnectionAlive) override;

    std::uint32_t getBufferSize() const override;

    void setBufferSize(const std::uint32_t bufferSize) override;

private:
    ConnixCore::Communication::TransportType m_transportType;

    std::uint32_t m_connectionTimeout;

    bool m_keepConnectionAlive;

    std::uint32_t m_bufferSize;
};

} // namespace Communication
} // namespace ConnixCore
