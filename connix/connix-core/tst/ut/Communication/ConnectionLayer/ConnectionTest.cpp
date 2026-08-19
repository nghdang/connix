#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "ConnixCore/Communication/ConnectionLayer/Connection.hpp"
#include "ConnixCore/Communication/TransportLayer/MockITransport.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

class DummyConnectionSubscriber
    : public ConnixCore::Communication::IConnectionSubscriber
{
public:
    ~DummyConnectionSubscriber() override = default;
    void onStateChanged() override
    {
    }
    void onDataSent(std::size_t count) override
    {
    }
    bool onDataReceived(std::vector<std::uint8_t>& receivedData,
                        std::vector<std::uint8_t>& responseData) override
    {
        return false;
    }
};

TEST(ConnectionTest, ConnectionAndTransportSubscriber)
{
    ConnixCore::Communication::ConnectionEndpoint remote{ "127.0.0.1", 8080 };
    ConnixCore::Communication::ConnectionEndpoint local{ "127.0.0.1", 9090 };

    std::shared_ptr<ConnixCore::Communication::ITransportSubscriber>
        capturedSubscriber;
    auto mockTransport = ConnixCore::Communication::MockITransport::create();

    // Verify constructor calls addSubscriber on transport
    EXPECT_CALL(*mockTransport, addSubscriber(_))
        .WillOnce(DoAll(SaveArg<0>(&capturedSubscriber), Return(true)));

    auto connection = std::make_shared<ConnixCore::Communication::Connection>(
        1, remote, local, mockTransport);
    EXPECT_NE(connection, nullptr);

    // Verify getter methods
    EXPECT_EQ(connection->getId(), 1);
    EXPECT_EQ(connection->getRemote().address, "127.0.0.1");
    EXPECT_EQ(connection->getRemote().port, 8080);
    EXPECT_EQ(connection->getLocal().address, "127.0.0.1");
    EXPECT_EQ(connection->getLocal().port, 9090);
    EXPECT_EQ(connection->getState(),
              ConnixCore::Communication::ConnectionState::CLOSED);

    // Verify other stubbed-out methods of Connection
    EXPECT_FALSE(connection->addSubscriber(nullptr));
    EXPECT_FALSE(connection->removeSubscriber(nullptr));
    EXPECT_FALSE(connection->send({}));
    EXPECT_FALSE(connection->close());

    // Verify TransportSubscriber callbacks inside Connection
    ASSERT_NE(capturedSubscriber, nullptr);
    capturedSubscriber->onStateChanged();
    capturedSubscriber->onStatusChanged();
    capturedSubscriber->onDataSent(42);
    std::vector<std::uint8_t> rec, resp;
    capturedSubscriber->onDataReceived(rec, resp);

    // Cover virtual destructor of IConnectionSubscriber using a dummy sub
    std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber> sub =
        std::make_shared<DummyConnectionSubscriber>();
    EXPECT_NE(sub, nullptr);
    sub.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
