#pragma once

#include <stdexcept>
#include <string>

#include "ConnixCore/Infrastructure/Timer/TimerErrorCode.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Timer {

/**
 * @brief Exception thrown upon timer management and execution failures.
 */
class TimerException : public std::runtime_error
{
public:
    /**
     * @brief Constructs a new TimerException.
     * @param errorCode Strongly typed error code identifying the failure.
     * @param message Human-readable diagnostic description of the error.
     */
    TimerException(TimerErrorCode errorCode, const std::string& message);

    /**
     * @brief Default virtual destructor.
     */
    ~TimerException() override = default;

    /**
     * @brief Retrieves the associated timer error code.
     * @return The strongly typed TimerErrorCode.
     */
    TimerErrorCode getErrorCode() const noexcept;

private:
    TimerErrorCode m_errorCode;
};

} // namespace Timer
} // namespace Infrastructure
} // namespace ConnixCore
