#pragma once

#include <memory>

#include "ConnixCore/Common/IConnixConfiguration.hpp"

namespace ConnixCore {
namespace Common {

class CommonFactory
{
public:
    virtual ~CommonFactory() = default;

    static std::shared_ptr<ConnixCore::Common::IConnixConfiguration>
    createConfiguration();
};

} // namespace Common
} // namespace ConnixCore
