#include "ConnixCore/Infrastructure/Configuration/FrameConfig.hpp"

#include <cstdint>
#include <optional>

#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

FrameConfig::FrameConfig(FrameType type, std::optional<std::uint32_t> size,
                         std::optional<std::uint32_t> lengthOffset,
                         std::optional<std::uint32_t> lengthSize,
                         std::optional<ByteOrder> byteOrder)
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

const std::optional<std::uint32_t>& FrameConfig::getSize() const
{
    return m_size;
}

const std::optional<std::uint32_t>& FrameConfig::getLengthOffset() const
{
    return m_lengthOffset;
}

const std::optional<std::uint32_t>& FrameConfig::getLengthSize() const
{
    return m_lengthSize;
}

const std::optional<ByteOrder>& FrameConfig::getByteOrder() const
{
    return m_byteOrder;
}

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
