#pragma once

#include <cstdint>
#include <optional>

#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class FrameConfig
{
public:
    FrameConfig(FrameType type, std::optional<std::uint32_t> size,
                std::optional<std::uint32_t> lengthOffset,
                std::optional<std::uint32_t> lengthSize,
                std::optional<ByteOrder> byteOrder);

    FrameType getType() const;
    const std::optional<std::uint32_t>& getSize() const;
    const std::optional<std::uint32_t>& getLengthOffset() const;
    const std::optional<std::uint32_t>& getLengthSize() const;
    const std::optional<ByteOrder>& getByteOrder() const;

private:
    FrameType m_type;
    std::optional<std::uint32_t> m_size;
    std::optional<std::uint32_t> m_lengthOffset;
    std::optional<std::uint32_t> m_lengthSize;
    std::optional<ByteOrder> m_byteOrder;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
