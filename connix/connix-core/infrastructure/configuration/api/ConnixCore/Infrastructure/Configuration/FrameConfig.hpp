#pragma once

#include <cstdint>

#include "ConnixCore/Infrastructure/Configuration/ByteOrder.hpp"
#include "ConnixCore/Infrastructure/Configuration/FrameType.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

class FrameConfig
{
public:
    FrameConfig();
    FrameConfig(FrameType type, std::uint32_t size, std::uint32_t lengthOffset,
                std::uint32_t lengthSize, ByteOrder byteOrder);

    FrameType getType() const;
    std::uint32_t getSize() const;
    std::uint32_t getLengthOffset() const;
    std::uint32_t getLengthSize() const;
    ByteOrder getByteOrder() const;

private:
    FrameType m_type;
    std::uint32_t m_size;
    std::uint32_t m_lengthOffset;
    std::uint32_t m_lengthSize;
    ByteOrder m_byteOrder;
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
