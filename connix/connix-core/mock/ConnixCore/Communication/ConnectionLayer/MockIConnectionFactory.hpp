#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/ConnectionLayer/IConnectionFactory.hpp"

namespace ConnixCore {
namespace Communication {

class MockIConnectionFactory
    : public ConnixCore::Communication::IConnectionFactory
{
public:
    MOCK_METHOD(std::shared_ptr<ConnixCore::Communication::IConnectionManager>,
                createConnectionManager, (), (override));

    MOCK_METHOD(
        std::shared_ptr<ConnixCore::Communication::IConnection>,
        createConnection,
        (const ConnixCore::Communication::ConnectionEndpoint& remote,
         const ConnixCore::Communication::ConnectionEndpoint& local,
         std::shared_ptr<ConnixCore::Communication::ITransport> transport),
        (override));

    static std::shared_ptr<ConnixCore::Communication::MockIConnectionFactory>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIConnectionFactory>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockIConnectionFactory>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
