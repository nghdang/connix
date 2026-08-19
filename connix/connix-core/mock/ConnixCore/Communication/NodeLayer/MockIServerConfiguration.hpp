#pragma once

#include "gmock/gmock.h"

#include "ConnixCore/Communication/NodeLayer/IServerConfiguration.hpp"

namespace ConnixCore {
namespace Communication {

class MockIServerConfiguration
    : public ConnixCore::Communication::IServerConfiguration
{
public:
    MOCK_METHOD(ConnixCore::Communication::TransportType, getTransportType, (),
                (const, override));

    MOCK_METHOD(void, setTransportType,
                (const ConnixCore::Communication::TransportType transportType),
                (override));

    MOCK_METHOD(std::uint32_t, getMaximumConnections, (), (const, override));

    MOCK_METHOD(void, setMaximumConnections,
                (const std::uint32_t maximumConnections), (override));

    MOCK_METHOD(std::uint32_t, getMaximumRequestsPerSecond, (),
                (const, override));

    MOCK_METHOD(void, setMaximumRequestsPerSecond,
                (const std::uint32_t maximumRequestsPerSecond), (override));

    MOCK_METHOD(std::uint32_t, getBufferSize, (), (const, override));

    MOCK_METHOD(void, setBufferSize, (const std::uint32_t bufferSize),
                (override));

    static std::shared_ptr<ConnixCore::Communication::MockIServerConfiguration>
    create();

    static std::shared_ptr<::testing::StrictMock<
        ConnixCore::Communication::MockIServerConfiguration>>
    createStrict();

    static std::shared_ptr<::testing::NiceMock<
        ConnixCore::Communication::MockIServerConfiguration>>
    createNice();
};

} // namespace Communication
} // namespace ConnixCore
