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

std::string MethodName(const std::string& pretty_function) ;


#define __METHOD__ slog::MethodName(__PRETTY_FUNCTION__)

#define __THREAD__ std::this_thread::get_id()

#define LOG_TRACE(logger) LogEvent(slog::LogLevel::TRACE,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_DEBUG(logger) LogEvent(slog::LogLevel::DEBUG,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_INFO(logger) LogEvent(slog::LogLevel::INFO,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_WARN(logger) LogEvent(slog::LogLevel::WARN,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG_ERROR(logger) LogEvent(slog::LogLevel::ERROR,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)

#define LOG(logger,level) LogEvent(level,logger).Locate(__FILE__,__METHOD__,__LINE__,__THREAD__)
}

#endif 
