#pragma once

#include <stdexcept>
#include <string>

#include "ConnixCore/Infrastructure/Transport/TransportErrorCode.hpp"

namespace ConnixCore {
namespace Infrastructure {
namespace Transport {

/**
 * @brief Exception thrown upon transport and network communication failures.
 */
class TransportException : public std::runtime_error
{
public:
    /**
     * @brief Constructs a new TransportException.
     * @param errorCode Strongly typed error code identifying the failure.
     * @param message Human-readable diagnostic description of the error.
     */
    TransportException(TransportErrorCode errorCode,
                       const std::string& message);

    /**
     * @brief Default virtual destructor.
     */
    ~TransportException() override = default;

    /**
     * @brief Retrieves the associated transport error code.
     * @return The strongly typed TransportErrorCode.
     */
    TransportErrorCode getErrorCode() const noexcept;

private:
    TransportErrorCode m_errorCode;
};

} // namespace Transport
} // namespace Infrastructure
} // namespace ConnixCore
