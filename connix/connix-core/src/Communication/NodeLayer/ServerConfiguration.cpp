#include "ConnixCore/Communication/NodeLayer/ServerConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

ConnixCore::Communication::TransportType
ServerConfiguration::getTransportType() const
{
    return m_transportType;
}

void ServerConfiguration::setTransportType(
    const ConnixCore::Communication::TransportType transportType)
{
    m_transportType = transportType;
}

std::uint32_t ServerConfiguration::getMaximumConnections() const
{
    return m_maximumConnections;
}

void ServerConfiguration::setMaximumConnections(
    const std::uint32_t maximumConnections)
{
    m_maximumConnections = maximumConnections;
}

std::uint32_t ServerConfiguration::getMaximumRequestsPerSecond() const
{
    return m_maximumRequestsPerSecond;
}

void ServerConfiguration::setMaximumRequestsPerSecond(
    const std::uint32_t maximumRequestsPerSecond)
{
    m_maximumRequestsPerSecond = maximumRequestsPerSecond;
}

std::uint32_t ServerConfiguration::getBufferSize() const
{
    return m_bufferSize;
}

void ServerConfiguration::setBufferSize(const std::uint32_t bufferSize)
{
    m_bufferSize = bufferSize;
}

} // namespace Communication
} // namespace ConnixCore
