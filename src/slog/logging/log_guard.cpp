/*
* Created by suemi on 2017/2/15.
*/

#include <iostream>
#include <stdexcept>
#include "slog/logging/log_guard.h"

namespace slog {

static char const PREFIX[] = "slog: ";
static char const ERR_PREFIX[] = "slog:ERROR ";
static char const WARN_PREFIX[] = "slog:WARN ";

LogGuard *volatile LogGuard::instance_ = nullptr;
std::mutex LogGuard::mutex_;

LogGuard *LogGuard::Instance() {
  if (instance_) return instance_;
  LogGuard *ptr = new LogGuard();
  bool success = true;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_ == nullptr) {
      instance_ = ptr;
    } else
      success = false;
  }
  if (success) return ptr;
  delete ptr;
  return Instance();
}

void LogGuard::Debug(const std::string &msg) const {
  Debug(msg.c_str());
}

void LogGuard::Debug(const char *msg) const {
  DoLog(PREFIX, msg, false);
}

void LogGuard::Warn(const std::string &msg) const {
  Warn(msg.c_str());
}

void LogGuard::Warn(const char *msg) const {
  DoLog(WARN_PREFIX, msg, false);
}

void LogGuard::Error(const std::string &msg, bool throw_flag) const {
  Error(msg.c_str(), throw_flag);
}

void LogGuard::Error(const char *msg, bool throw_flag) const {
  DoLog(ERR_PREFIX, msg, throw_flag);
}

void LogGuard::Enable() {
  enable_internal_logging_ = true;
}

void LogGuard::Disable() {
  enable_internal_logging_ = false;
}

LogGuard::LogGuard() : enable_internal_logging_(true) {}

LogGuard::~LogGuard() {
  LogGuard *volatile ptr = this;
  if (ptr == instance_)
    instance_ = nullptr;
}

void LogGuard::DoLog(const char *prefix, const char *msg, bool throw_flag) const {
  if (enable_internal_logging_) {
    auto str = std::string(prefix) + std::string(msg) + '\n';
    std::cout << str << std::flush;
  }
  if (throw_flag)
    throw std::runtime_error(msg);
}

}