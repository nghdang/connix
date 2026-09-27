#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"

#include <cstdint>

#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

FrameConfig::FrameConfig()
    : m_type(FrameType::NONE)
    , m_size(0)
    , m_lengthOffset(0)
    , m_lengthSize(0)
    , m_byteOrder(ByteOrder::BIG_ENDIAN)
{
}

FrameConfig::FrameConfig(FrameType type, std::uint32_t size,
                         std::uint32_t lengthOffset, std::uint32_t lengthSize,
                         ByteOrder byteOrder)
    : m_type(type)
    , m_size(size)
    , m_lengthOffset(lengthOffset)
    , m_lengthSize(lengthSize)
    , m_byteOrder(byteOrder)
{
}

FrameType FrameConfig::getType() const
{
    return m_type;
}

std::uint32_t FrameConfig::getSize() const
{
    return m_size;
}

std::uint32_t FrameConfig::getLengthOffset() const
{
    return m_lengthOffset;
}

std::uint32_t FrameConfig::getLengthSize() const
{
    return m_lengthSize;
}

ByteOrder FrameConfig::getByteOrder() const
{
    return m_byteOrder;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
