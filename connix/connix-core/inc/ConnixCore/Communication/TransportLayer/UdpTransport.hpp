#pragma once

#include "ConnixCore/Communication/TransportLayer/BaseTransport.hpp"

namespace ConnixCore {
namespace Communication {

class UdpTransport : public ConnixCore::Communication::BaseTransport
{
public:
    explicit UdpTransport(ConnixCore::Communication::TransportId id);

    virtual ~UdpTransport() override = default;
};

} // namespace Communication
} // namespace ConnixCore
