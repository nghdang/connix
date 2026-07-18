#include "ConnixCore/Logging/Logger.hpp"

#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

namespace ConnixCore {
namespace Logging {

Logger& Logger::getInstance()
{
    static Logger logger;
    return logger;
}

void Logger::doRecord(std::ostringstream& oss)
{
    std::cout << oss.str() << std::endl;
}

std::string Logger::composeHeading()
{
    const std::chrono::system_clock::time_point now{ std::chrono::system_clock::now() };
    const std::time_t nowTime{ std::chrono::system_clock::to_time_t(now) };
    const std::tm* timeInfo{ std::localtime(&nowTime) };
    std::array<char, 100> buffer{};
    std::strftime(buffer.data(), sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);

    std::chrono::duration epoch{ now.time_since_epoch() };
    auto value{ std::chrono::duration_cast<std::chrono::microseconds>(epoch) };
    long long microseconds{ value.count() % 1000000 };

    std::ostringstream ossThread;
    ossThread << std::setfill('0') << std::setw(8) << std::hex << std::this_thread::get_id();

    std::ostringstream oss;
    oss << getpid() << " " << std::string(buffer.data()) << "." << std::setfill('0') << std::setw(6) << microseconds
        << " 0x" << ossThread.str() << " ";

    return oss.str();
}

} // namespace Logging
} // namespace ConnixCore
