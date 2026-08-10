#pragma once

#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"

namespace ConnixCore {
namespace Communication {

class BaseTransport : public ConnixCore::Communication::ITransport
{
public:
    explicit BaseTransport(ConnixCore::Communication::TransportType type);

    virtual ~BaseTransport() override = default;

    ConnixCore::Communication::TransportType getType() const override;

    ConnixCore::Communication::TransportState getState() const override;

    ConnixCore::Communication::TransportStatus getStatus() const override;

    bool addSubscriber(
        std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
            subscriber) override;

    bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
            subscriber) override;

    bool send(const std::vector<std::uint8_t>& data) override;

    void close() override;

protected:
    ConnixCore::Communication::TransportState m_state;

    ConnixCore::Communication::TransportStatus m_status;

    std::vector<
        std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>>
        m_subscribers;

    std::vector<std::uint8_t> m_sendBuffer;

    std::vector<std::uint8_t> m_recvBuffer;

private:
    ConnixCore::Communication::TransportType m_type;
};

} // namespace Communication
} // namespace ConnixCore
