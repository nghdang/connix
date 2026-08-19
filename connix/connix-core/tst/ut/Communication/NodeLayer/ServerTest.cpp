#include "gtest/gtest.h"

#include "ConnixCore/Communication/NodeLayer/MockIServerConfiguration.hpp"
#include "ConnixCore/Communication/NodeLayer/Server.hpp"

using namespace testing;

namespace ConnixCore {
namespace UnitTest {

class DummyServerSubscriber
    : public ConnixCore::Communication::IServerSubscriber
{
public:
    ~DummyServerSubscriber() override = default;
    void
    onConnection(const std::shared_ptr<ConnixCore::Communication::IConnection>&
                     connection) override
    {
    }
};

TEST(ServerTest, ServerMethods)
{
    auto mockConfig =
        ConnixCore::Communication::MockIServerConfiguration::create();
    auto server =
        std::make_shared<ConnixCore::Communication::Server>(mockConfig);
    ASSERT_NE(server, nullptr);
    EXPECT_EQ(server->getConfiguration(), mockConfig);

    EXPECT_FALSE(server->addSubscriber(nullptr));
    EXPECT_FALSE(server->removeSubscriber(nullptr));
    EXPECT_FALSE(server->start("127.0.0.1", 8080));
    EXPECT_FALSE(server->stop());
    EXPECT_FALSE(server->send(1, {}));
    EXPECT_FALSE(server->closeConnection(1));

    // Cover subscriber virtual destructor
    std::shared_ptr<ConnixCore::Communication::IServerSubscriber> sub =
        std::make_shared<DummyServerSubscriber>();
    EXPECT_NE(sub, nullptr);
    sub.reset();
}

} // namespace UnitTest
} // namespace ConnixCore
