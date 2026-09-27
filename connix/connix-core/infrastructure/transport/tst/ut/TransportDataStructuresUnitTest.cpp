#include "gtest/gtest.h"

#include <string>

#include "ConnixCore/Infrastructure/Transport/SocketDomain.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketOption.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/SocketType.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportEndpoint.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportProtocol.hpp"
#include "ConnixCore/Infrastructure/Transport/TransportState.hpp"

using namespace ConnixCore::Infrastructure::Transport;

namespace ConnixCore {
namespace UnitTest {

TEST(TransportDataStructuresTest, TransportProtocolEnumValues)
{
    EXPECT_NE(TransportProtocol::TCP, TransportProtocol::UDP);
    EXPECT_NE(TransportProtocol::UDS_STREAM, TransportProtocol::UDS_DATAGRAM);
    EXPECT_NE(TransportProtocol::TCP, TransportProtocol::UDS_STREAM);
}

TEST(TransportDataStructuresTest, TransportStateEnumValues)
{
    EXPECT_NE(TransportState::CLOSED, TransportState::CONNECTED);
    EXPECT_NE(TransportState::BOUND, TransportState::LISTENING);
    EXPECT_NE(TransportState::CONNECTING, TransportState::DISCONNECTED);
}

TEST(TransportDataStructuresTest, TransportErrorCodeEnumValues)
{
    EXPECT_NE(TransportErrorCode::SOCKET_CREATION_FAILED,
              TransportErrorCode::CONNECT_FAILED);
    EXPECT_NE(TransportErrorCode::OPERATION_TIMEOUT,
              TransportErrorCode::CONNECTION_CLOSED);
    EXPECT_NE(TransportErrorCode::UNSUPPORTED_PROTOCOL,
              TransportErrorCode::INVALID_ADDRESS);
}

TEST(TransportDataStructuresTest, TransportExceptionHandling)
{
    const TransportException exception(TransportErrorCode::OPERATION_TIMEOUT,
                                       "Connection timed out after 5000 ms");

    EXPECT_EQ(exception.getErrorCode(), TransportErrorCode::OPERATION_TIMEOUT);
    EXPECT_STREQ(exception.what(), "Connection timed out after 5000 ms");

    try
    {
        throw TransportException(TransportErrorCode::OPERATION_TIMEOUT,
                                 "Connection timed out after 5000 ms");
    } catch (const TransportException& ex)
    {
        EXPECT_EQ(ex.getErrorCode(), TransportErrorCode::OPERATION_TIMEOUT);
        EXPECT_STREQ(ex.what(), "Connection timed out after 5000 ms");
    }
}

TEST(TransportDataStructuresTest, TransportEndpointDefaultConstructor)
{
    const TransportEndpoint endpoint;

    EXPECT_TRUE(endpoint.getAddress().empty());
    EXPECT_EQ(endpoint.getPort(), 0);
    EXPECT_FALSE(endpoint.isUnixDomain());
    EXPECT_EQ(endpoint.toString(), "");
}

TEST(TransportDataStructuresTest, TransportEndpointIpConstructors)
{
    const TransportEndpoint ipv4("127.0.0.1", 8080);
    EXPECT_EQ(ipv4.getAddress(), "127.0.0.1");
    EXPECT_EQ(ipv4.getPort(), 8080);
    EXPECT_FALSE(ipv4.isUnixDomain());
    EXPECT_EQ(ipv4.toString(), "127.0.0.1:8080");

    const TransportEndpoint ipv6("::1", 9000);
    EXPECT_EQ(ipv6.getAddress(), "::1");
    EXPECT_EQ(ipv6.getPort(), 9000);
    EXPECT_FALSE(ipv6.isUnixDomain());
    EXPECT_EQ(ipv6.toString(), "::1:9000");
}

TEST(TransportDataStructuresTest, TransportEndpointUdsConstructor)
{
    const TransportEndpoint uds("/tmp/connix.sock");
    EXPECT_EQ(uds.getAddress(), "/tmp/connix.sock");
    EXPECT_EQ(uds.getPort(), 0);
    EXPECT_TRUE(uds.isUnixDomain());
    EXPECT_EQ(uds.toString(), "unix:/tmp/connix.sock");

    const TransportEndpoint relativeUds("./local.sock");
    EXPECT_TRUE(relativeUds.isUnixDomain());
    EXPECT_EQ(relativeUds.toString(), "unix:./local.sock");
}

TEST(TransportDataStructuresTest, TransportEndpointEquality)
{
    const TransportEndpoint ep1("192.168.1.1", 5000);
    const TransportEndpoint ep2("192.168.1.1", 5000);
    const TransportEndpoint ep3("192.168.1.1", 5001);
    const TransportEndpoint ep4("192.168.1.2", 5000);

    EXPECT_TRUE(ep1 == ep2);
    EXPECT_FALSE(ep1 != ep2);

    EXPECT_FALSE(ep1 == ep3);
    EXPECT_TRUE(ep1 != ep3);

    EXPECT_FALSE(ep1 == ep4);
    EXPECT_TRUE(ep1 != ep4);
}

TEST(TransportDataStructuresTest, InternalSocketEnums)
{
    EXPECT_NE(SocketDomain::IPV4, SocketDomain::IPV6);
    EXPECT_NE(SocketDomain::IPV6, SocketDomain::UNIX);

    EXPECT_NE(SocketType::STREAM, SocketType::DATAGRAM);

    EXPECT_NE(SocketProtocol::DEFAULT, SocketProtocol::TCP);
    EXPECT_NE(SocketProtocol::TCP, SocketProtocol::UDP);

    EXPECT_NE(SocketOption::REUSE_ADDRESS, SocketOption::NON_BLOCKING);
    EXPECT_NE(SocketOption::RECEIVE_TIMEOUT, SocketOption::SEND_TIMEOUT);
}

} // namespace UnitTest
} // namespace ConnixCore
