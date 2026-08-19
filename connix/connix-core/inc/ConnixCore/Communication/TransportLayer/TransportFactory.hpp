#pragma once

#include <memory>

#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransportFactory.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportType.hpp"

namespace ConnixCore {
namespace Communication {

class TransportFactory : public ConnixCore::Communication::ITransportFactory
{
public:
    explicit TransportFactory() = default;

    ~TransportFactory() override = default;

    std::shared_ptr<ConnixCore::Communication::ITransport>
    createTransport(ConnixCore::Communication::TransportType type) override;
};

} // namespace Communication
} // namespace ConnixCore
