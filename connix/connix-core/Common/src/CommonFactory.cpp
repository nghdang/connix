#include "ConnixCore/Common/CommonFactory.hpp"

#include "ConnixCore/Common/ConnixConfiguration.hpp"

namespace ConnixCore {
namespace Common {

std::shared_ptr<ConnixCore::Common::IConnixConfiguration>
CommonFactory::createConfiguration()
{
    return std::make_shared<ConnixCore::Common::ConnixConfiguration>();
}

} // namespace Common
} // namespace ConnixCore
