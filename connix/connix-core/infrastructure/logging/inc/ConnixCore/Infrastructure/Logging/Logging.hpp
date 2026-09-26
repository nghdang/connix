#pragma once

#include <cassert>

#include "ConnixCore/Infrastructure/Logging/Logger.hpp"

#ifndef LOG_E
#define LOG_E(...) _LOG(ERROR, __VA_ARGS__)
#endif

#ifndef LOG_W
#define LOG_W(...) _LOG(WARN, __VA_ARGS__)
#endif

#ifndef LOG_I
#define LOG_I(...) _LOG(INFO, __VA_ARGS__)
#endif

#ifndef LOG_D
#define LOG_D(...) _LOG(DEBUG, __VA_ARGS__)
#endif

#ifndef _LOG
#define _LOG(level, ...)                                                      \
    ConnixCore::Infrastructure::Logging::Logger::getInstance().record(        \
        #level, __VA_ARGS__)
#endif

#ifndef LOG_ASSERT
#define LOG_ASSERT(condition, ...)                                            \
    [&]() {                                                                   \
        if (!(condition))                                                     \
        {                                                                     \
            ConnixCore::Infrastructure::Logging::Logger::getInstance()        \
                .record("ASSERT FAILED", __VA_ARGS__);                        \
            assert(condition);                                                \
        }                                                                     \
    }()
#endif
