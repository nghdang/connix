#include "ConnixCore/Infrastructure/Timer/TimerException.hpp"

#include <stdexcept>
#include <string>

#include "ConnixCore/Infrastructure/Timer/TimerErrorCode.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

TimerException::TimerException(TimerErrorCode errorCode,
                               const std::string& message)
    : std::runtime_error(message)
    , m_errorCode(errorCode)
{
}

TimerErrorCode TimerException::getErrorCode() const noexcept
{
    return m_errorCode;
}

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
