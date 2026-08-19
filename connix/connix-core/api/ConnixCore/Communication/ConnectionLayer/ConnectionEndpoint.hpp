#pragma once

#include <cstdint>
#include <string>

namespace ConnixCore {
namespace Communication {

struct ConnectionEndpoint {
    std::string address;

    std::uint16_t port;
};

} // namespace Communication
} // namespace ConnixCore
