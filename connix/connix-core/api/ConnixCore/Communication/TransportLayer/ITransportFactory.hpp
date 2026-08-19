#pragma once

#include <memory>

#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportType.hpp"

namespace ConnixCore {
namespace Communication {

class ITransportFactory
{
public:
    virtual ~ITransportFactory() = default;

    virtual std::shared_ptr<ConnixCore::Communication::ITransport>
    createTransport(ConnixCore::Communication::TransportType type) = 0;
};

} // namespace Communication
} // namespace ConnixCore
