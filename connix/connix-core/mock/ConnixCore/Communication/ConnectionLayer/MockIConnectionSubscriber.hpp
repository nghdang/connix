#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/ConnectionLayer/IConnectionSubscriber.hpp"

namespace ConnixCore {
namespace Communication {

class MockIConnectionSubscriber
    : public ConnixCore::Communication::IConnectionSubscriber
{
public:
    MOCK_METHOD(void, onStateChanged, (), (override));

    MOCK_METHOD(void, onDataSent, (std::size_t count), (override));

    MOCK_METHOD(bool, onDataReceived,
                (std::vector<std::uint8_t> & receivedData,
                 std::vector<std::uint8_t>& responseData),
                (override));

    static std::shared_ptr<
        ConnixCore::Communication::MockIConnectionSubscriber>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIConnectionSubscriber>>
    createStrict();

    static std::shared_ptr<::testing::NiceMock<
        ConnixCore::Communication::MockIConnectionSubscriber>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
