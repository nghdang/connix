#pragma once

#include <cstdint>

#if defined(__linux__) || defined(__GLIBC__)
#include <endian.h>
#endif

#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

#ifdef LITTLE_ENDIAN
#undef LITTLE_ENDIAN
#endif

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class ByteOrder : std::uint8_t {
    BIG_ENDIAN,
    LITTLE_ENDIAN
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
