/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_LOG_LEVEL_H
#define SLOG_LOG_LEVEL_H

#include <string>
#include <unordered_map>
#include <algorithm>

namespace slog {
 enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR

  };

std::string ToString(LogLevel level);

LogLevel FromString(const std::string& level);

}

#endif 
