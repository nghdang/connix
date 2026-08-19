#pragma once

namespace ConnixCore {
namespace Communication {

enum class TransportState {
    CONNECTING,
    CONNECTED,
    DISCONNECTING,
    DISCONNECTED
};

} // namespace Communication
} // namespace ConnixCore
