#include "gtest/gtest.h"

#include "ConnixCore/Communication/TransportLayer/BaseTransport.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

class DummyTransportSubscriber
    : public ConnixCore::Communication::ITransportSubscriber
{
public:
    ~DummyTransportSubscriber() override = default;
    void onStateChanged() override
    {
    }
    void onStatusChanged() override
    {
    }
    void onDataSent(std::size_t count) override
    {
    }
    void onDataReceived(std::vector<std::uint8_t>& receivedData,
                        std::vector<std::uint8_t>& responseData) override
    {
    }
};

TEST(BaseTransportTest, Methods)
{
    auto transport =
        std::make_shared<ConnixCore::Communication::BaseTransport>(
            ConnixCore::Communication::TransportType::TCP);
    EXPECT_EQ(transport->getType(),
              ConnixCore::Communication::TransportType::TCP);
    EXPECT_EQ(transport->getState(),
              ConnixCore::Communication::TransportState::DISCONNECTED);
    EXPECT_EQ(transport->getStatus(),
              ConnixCore::Communication::TransportStatus::IDLE);

    EXPECT_FALSE(transport->addSubscriber(nullptr));
    EXPECT_FALSE(transport->removeSubscriber(nullptr));
    EXPECT_FALSE(transport->send({}));
    transport->close();

    // Cover subscriber virtual destructor
    std::shared_ptr<ConnixCore::Communication::ITransportSubscriber> sub =
        std::make_shared<DummyTransportSubscriber>();
    EXPECT_NE(sub, nullptr);
    sub.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
