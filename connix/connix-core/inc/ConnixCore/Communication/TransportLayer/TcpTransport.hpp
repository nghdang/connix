#pragma once

#include "ConnixCore/Communication/TransportLayer/BaseTransport.hpp"

namespace ConnixCore {
namespace Communication {

class TcpTransport : public ConnixCore::Communication::BaseTransport
{
public:
    explicit TcpTransport();

    ~TcpTransport() override = default;
};

} // namespace Communication
} // namespace ConnixCore
