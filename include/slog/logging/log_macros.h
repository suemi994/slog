/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_LOG_MACROS_H
#define SLOG_LOG_MACROS_H

#include <thread>

#include "slog/logging/log_event.h"
#include "slog/logging/logger.h"
#include "slog/logging/log_level.h"

namespace slog {

inline std::string MethodName(const std::string& pretty_function){
  size_t end = pretty_function.find("(");
  size_t start = pretty_function.find(" ")+1;
  return pretty_function.substr(start,end-start);
}

#define __METHOD__ MethodName(__PRETTY_FUNCTION__)

#define __THREAD__ std::this_thread::get_id()

#define LOG_TRACE(logger) LogEvent(LogLevel::TRACE,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_DEBUG(logger) LogEvent(LogLevel::DEBUG,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_INFO(logger) LogEvent(LogLevel::INFO,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_WARN(logger) LogEvent(LogLevel::WARN,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_ERROR(logger) LogEvent(LogLevel::ERROR,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG(logger,level) LogEvent(level,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)
}

#endif 
