#pragma once

#include "ConnixCore/Communication/ConnectionLayer/IConnection.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransportSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class Connection : public ConnixCore::Communication::IConnection
{
public:
    explicit Connection(
        const ConnixCore::Communication::ConnectionId id,
        const ConnixCore::Communication::ConnectionEndpoint& remote,
        const ConnixCore::Communication::ConnectionEndpoint& local,
        std::shared_ptr<ConnixCore::Communication::ITransport> transport);

    ~Connection() override = default;

    ConnixCore::Communication::ConnectionId getId() const override;

    ConnixCore::Communication::ConnectionEndpoint getRemote() const override;

    ConnixCore::Communication::ConnectionEndpoint getLocal() const override;

    ConnixCore::Communication::ConnectionState getState() const override;

    bool addSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) override;

    bool removeSubscriber(
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>
            subscriber) override;

    bool send(const std::vector<std::uint8_t>& data) override;

    bool close() override;

private:
    class TransportSubscriber
        : public ConnixCore::Communication::ITransportSubscriber
    {
    public:
        void onStateChanged() override;

        void onStatusChanged() override;

        void onDataSent(std::size_t count) override;

        void onDataReceived(std::vector<std::uint8_t>& receivedData,
                            std::vector<std::uint8_t>& responseData) override;
    };

private:
    ConnixCore::Communication::ConnectionId m_id;

    ConnixCore::Communication::ConnectionEndpoint m_remote;

    ConnixCore::Communication::ConnectionEndpoint m_local;

    std::shared_ptr<ConnixCore::Communication::ITransport> m_transport;

    ConnixCore::Communication::ConnectionState m_state;

    std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
        m_transportSubscriber;

    std::vector<
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>
        m_connectionSubscribers;
};

} // namespace Communication
} // namespace ConnixCore
