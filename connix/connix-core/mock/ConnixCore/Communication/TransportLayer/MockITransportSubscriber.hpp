#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/TransportLayer/ITransportSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class MockITransportSubscriber
    : public ConnixCore::Communication::ITransportSubscriber
{
public:
    MOCK_METHOD(void, onStateChanged, (), (override));

    MOCK_METHOD(void, onStatusChanged, (), (override));

    MOCK_METHOD(void, onDataSent, (std::size_t count), (override));

    MOCK_METHOD(void, onDataReceived,
                (std::vector<std::uint8_t> & receivedData,
                 std::vector<std::uint8_t>& responseData),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockITransportSubscriber>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockITransportSubscriber>>
    createStrict();

    static std::shared_ptr<::testing::NiceMock<
        ConnixCore::Communication::MockITransportSubscriber>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
