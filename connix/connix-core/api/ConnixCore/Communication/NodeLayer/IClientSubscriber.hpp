#pragma once

#include <memory>

#include "ConnixCore/Communication/ConnectionLayer/IConnection.hpp"

namespace ConnixCore {
namespace Communication {

class IClientSubscriber
{
public:
    virtual ~IClientSubscriber() = default;

    virtual void
    onConnection(const std::shared_ptr<ConnixCore::Communication::IConnection>&
                     connection) = 0;
};

} // namespace Communication
} // namespace ConnixCore
