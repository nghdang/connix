#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/IClientConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

class MockIClientConfiguration
    : public ConnixCore::Communication::IClientConfiguration
{
public:
    MOCK_METHOD(ConnixCore::Communication::TransportType, getTransportType, (),
                (const, override));

    MOCK_METHOD(void, setTransportType,
                (const ConnixCore::Communication::TransportType transportType),
                (override));

    MOCK_METHOD(std::uint32_t, getConnectionTimeout, (), (const, override));

    MOCK_METHOD(void, setConnectionTimeout,
                (const std::uint32_t connectionTimeout), (override));

    MOCK_METHOD(bool, getKeepConnectionAlive, (), (const, override));

    MOCK_METHOD(void, setKeepConnectionAlive, (const bool keepConnectionAlive),
                (override));

    MOCK_METHOD(std::uint32_t, getBufferSize, (), (const, override));

    MOCK_METHOD(void, setBufferSize, (const std::uint32_t bufferSize),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockIClientConfiguration>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIClientConfiguration>>
    createStrict();

    static std::shared_ptr<::testing::NiceMock<
        ConnixCore::Communication::MockIClientConfiguration>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
