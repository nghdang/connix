#pragma once

#include "ConnixCore/Communication/TransportLayer/BaseTransport.hpp"

namespace ConnixCore {
namespace Communication {

class UdsTransport : public ConnixCore::Communication::BaseTransport
{
public:
    explicit UdsTransport();

    ~UdsTransport() override = default;
};

} // namespace Communication
} // namespace ConnixCore
