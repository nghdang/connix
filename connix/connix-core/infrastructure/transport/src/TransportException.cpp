#include "ConnixCore/Infrastructure/Transport/TransportException.hpp"

#include <stdexcept>
#include <string>

#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

TransportException::TransportException(TransportErrorCode errorCode,
                                       const std::string& message)
    : std::runtime_error(message)
    , m_errorCode(errorCode)
{
}

TransportErrorCode TransportException::getErrorCode() const noexcept
{
    return m_errorCode;
}

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
