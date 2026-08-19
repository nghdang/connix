#include "ConnixCore/Communication/NodeLayer/MockINodeFactory.hpp"

namespace ConnixCore {
namespace Communication {

std::shared_ptr<ConnixCore::Communication::MockINodeFactory>
MockINodeFactory::create()
{
    return std::make_shared<ConnixCore::Communication::MockINodeFactory>();
}

std::shared_ptr<
    ::testing::StrictMock<ConnixCore::Communication::MockINodeFactory>>
MockINodeFactory::createStrict()
{
    return std::make_shared<
        ::testing::StrictMock<ConnixCore::Communication::MockINodeFactory>>();
}

std::shared_ptr<
    ::testing::NiceMock<ConnixCore::Communication::MockINodeFactory>>
MockINodeFactory::createNice()
{
    return std::make_shared<
        ::testing::NiceMock<ConnixCore::Communication::MockINodeFactory>>();
}

} // namespace Communication
} // namespace ConnixCore
