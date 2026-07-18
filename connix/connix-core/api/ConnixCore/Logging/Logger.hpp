#pragma once

#include <iomanip>
#include <mutex>
#include <sstream>

namespace ConnixCore {
namespace Logging {

class Logger
{
public:
    static Logger& getInstance();

public:
    template <typename... Args>
    void record(const std::string& level, Args... args)
    {
        const std::lock_guard<std::mutex> lock{ m_accessMutex };

        std::ostringstream oss;
        oss << composeHeading() << std::setw(5) << level << " ";
        doRecord(oss, args...);
    }

private:
    Logger() = default;

    template <typename T, typename... Args>
    void doRecord(std::ostringstream& oss, T first, Args... args)
    {
        oss << (first);
        doRecord(oss, args...);
    }

    void doRecord(std::ostringstream& oss);

    std::string composeHeading();

private:
    std::mutex m_accessMutex;
};

} // namespace Logging
} // namespace ConnixCore
