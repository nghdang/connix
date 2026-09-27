#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class PayloadType : std::uint8_t {
    BYTES,
    FILE
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
