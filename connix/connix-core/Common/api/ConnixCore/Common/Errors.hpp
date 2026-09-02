#pragma once

#include <stdexcept>
#include <string>

namespace ConnixCore {
namespace Common {

class KeyError : public std::runtime_error
{
public:
    explicit KeyError(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace Common
} // namespace ConnixCore
