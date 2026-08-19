#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/TransportLayer/ITransportFactory.hpp"

namespace ConnixCore {
namespace Communication {

class MockITransportFactory
    : public ConnixCore::Communication::ITransportFactory
{
public:
    MOCK_METHOD(std::shared_ptr<ConnixCore::Communication::ITransport>,
                createTransport,
                (ConnixCore::Communication::TransportType type), (override));

    static std::shared_ptr<ConnixCore::Communication::MockITransportFactory>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockITransportFactory>>
    createStrict();

    static std::shared_ptr<
        ::testing::NiceMock<ConnixCore::Communication::MockITransportFactory>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
