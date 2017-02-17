/*
* Created by suemi on 2017/2/14.
*/

#ifndef SLOG_LOG_GUARD_H
#define SLOG_LOG_GUARD_H

#include <string>
#include <mutex>
#include "slog/utils/no_copyable.h"

namespace slog {

class LogGuard : public NoCopyable {
public:

  static LogGuard* Instance();

  void Debug(const std::string& msg) const;

  void Debug(const char* msg) const;

  void Warn(const std::string& msg) const;

  void Warn(const char* msg) const;

  void Error(const std::string& msg, bool throw_flag = false) const;

  void Error(const char* msg, bool throw_flag = false) const;

  void Enable();

  void Disable();

private:

  LogGuard();

  ~LogGuard();

  void DoLog(const char* prefix, const char* msg, bool throw_flag = false) const;

  bool enable_internal_logging_;

  static LogGuard* volatile instance_;

  static std::mutex mutex_;

};

}

#endif 

