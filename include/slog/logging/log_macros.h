/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_LOG_MACROS_H
#define SLOG_LOG_MACROS_H

#include "slog/logging/log_event.h"
#include "slog/logging/logger.h"

namespace slog {

inline std::string MethodName(const std::string& pretty_function){
  size_t end = pretty_function.find("(");
  size_t start = pretty_function.find(" ")+1;
  return pretty_function.substr(start,end-start);
}

#define __METHOD__ MethodName(__PRETTY_FUNCTION__)

}

#endif 
