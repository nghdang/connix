#include "ConnixCore/Communication/NodeLayer/ClientConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

ConnixCore::Communication::TransportType
ClientConfiguration::getTransportType() const
{
    return m_transportType;
}

void ClientConfiguration::setTransportType(
    const ConnixCore::Communication::TransportType transportType)
{
    m_transportType = transportType;
}

std::uint32_t ClientConfiguration::getConnectionTimeout() const
{
    return m_connectionTimeout;
}

void ClientConfiguration::setConnectionTimeout(
    const std::uint32_t connectionTimeout)
{
    m_connectionTimeout = connectionTimeout;
}

bool ClientConfiguration::getKeepConnectionAlive() const
{
    return m_keepConnectionAlive;
}

void ClientConfiguration::setKeepConnectionAlive(
    const bool keepConnectionAlive)
{
    m_keepConnectionAlive = keepConnectionAlive;
}

std::uint32_t ClientConfiguration::getBufferSize() const
{
    return m_bufferSize;
}

void ClientConfiguration::setBufferSize(const std::uint32_t bufferSize)
{
    m_bufferSize = bufferSize;
}

} // namespace Communication
} // namespace ConnixCore
