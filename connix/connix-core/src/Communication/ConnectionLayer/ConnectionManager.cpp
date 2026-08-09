#include "ConnixCore/Communication/ConnectionLayer/ConnectionManager.hpp"

namespace ConnixCore {
namespace Communication {

ConnectionManager::ConnectionManager(
    const std::vector<
        std::shared_ptr<ConnixCore::Communication::IConnectionSubscriber>>&
        subscribers)
    : m_subscribers{ subscribers }
    , m_connections{}
{
}

} // namespace Communication
} // namespace ConnixCore
