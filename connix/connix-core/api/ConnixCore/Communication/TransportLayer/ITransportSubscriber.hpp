#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ConnixCore {
namespace Communication {

class ITransportSubscriber
{
public:
    virtual ~ITransportSubscriber() = default;

    virtual void onStateChanged() = 0;

    virtual void onStatusChanged() = 0;

    virtual void onDataSent(std::size_t count) = 0;

    virtual void onDataReceived(std::vector<std::uint8_t>& receivedData,
                                std::vector<std::uint8_t>& responseData) = 0;
};

} // namespace Communication
} // namespace ConnixCore
