#pragma once

#include <stdexcept>
#include <string>

#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

class TransportException : public std::runtime_error
{
public:
    TransportException(TransportErrorCode errorCode,
                       const std::string& message);
    ~TransportException() override = default;

    TransportErrorCode getErrorCode() const noexcept;

private:
    TransportErrorCode m_errorCode;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
