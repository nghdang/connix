#pragma once

#include <memory>

#include "ConnixCore/Communication/TransportLayer/ITransport.hpp"
#include "ConnixCore/Communication/TransportLayer/ITransportFactory.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportId.hpp"
#include "ConnixCore/Communication/TransportLayer/TransportType.hpp"

namespace ConnixCore {
namespace Communication {

class TransportFactory : public ConnixCore::Communication::ITransportFactory
{
public:
    explicit TransportFactory();

    virtual ~TransportFactory() override = default;

    std::shared_ptr<ConnixCore::Communication::ITransport>
    createTransport(ConnixCore::Communication::TransportType type) override;

private:
    ConnixCore::Communication::TransportId m_transportIdGenerator;
};

} // namespace Communication
} // namespace ConnixCore
