#include "gtest/gtest.h"

#include "ConnixCore/Communication/NodeLayer/Client.hpp"
#include "ConnixCore/Communication/NodeLayer/MockIClientConfiguration.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

class DummyClientSubscriber
    : public ConnixCore::Communication::IClientSubscriber
{
public:
    ~DummyClientSubscriber() override = default;
    void
    onConnection(const std::shared_ptr<ConnixCore::Communication::IConnection>&
                     connection) override
    {
    }
};

TEST(ClientTest, ClientMethods)
{
    auto mockConfig =
        ConnixCore::Communication::MockIClientConfiguration::create();
    auto client =
        std::make_shared<ConnixCore::Communication::Client>(mockConfig);
    ASSERT_NE(client, nullptr);
    EXPECT_EQ(client->getConfiguration(), mockConfig);

    EXPECT_FALSE(client->addSubscriber(nullptr));
    EXPECT_FALSE(client->removeSubscriber(nullptr));
    EXPECT_FALSE(client->connect("127.0.0.1", 8080));
    EXPECT_FALSE(client->disconnect());
    EXPECT_FALSE(client->send(1, {}));
    EXPECT_FALSE(client->closeConnection(1));

    // Cover subscriber virtual destructor
    std::shared_ptr<ConnixCore::Communication::IClientSubscriber> sub =
        std::make_shared<DummyClientSubscriber>();
    EXPECT_NE(sub, nullptr);
    sub.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
