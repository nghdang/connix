#pragma once

#include <cstdint>

namespace ConnixCore {
namespace Infrastructure {
namespace Configuration {

enum class NodeType : std::uint8_t {
    USER,
    SERVER,
    CLIENT
};

} // namespace Configuration
} // namespace Infrastructure
} // namespace ConnixCore
