/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_LOG_LEVEL_H
#define SLOG_LOG_LEVEL_H

#include <string>

namespace slog {
 enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR

  };

std::string ToString(LogLevel level){
  return ((const char* []){"TRACE","DEBUG","INFO","WARN","ERROR"})[static_cast<int>(level)];
}

}

#endif 
