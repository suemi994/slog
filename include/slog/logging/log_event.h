/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_LOG_EVENT_H
#define SLOG_LOG_EVENT_H

#include <memory>
#include <chrono>

#include "slog/logging/log_level.h"
#include "slog/utils/no_copyable.h"
#include "slog/base/fixed_buffer.h"

namespace slog {

class Logger;

class Formatter;

/**
 * 包含日志内容以及携带每条日志相关信息
 * @author suemi
 */
class LogEvent : public NoCopyable {
  using self = LogEvent;
public:
  using Buffer = FixedBuffer<detail::SMALL_BUFFER_SIZE>;

  LogEvent(LogLevel level_, std::shared_ptr<Logger> logger);

  ~LogEvent();

  self &operator<<(bool val);

  self &operator<<(short val);

  self &operator<<(unsigned short val);

  self &operator<<(int val);

  self &operator<<(unsigned int val);

  self &operator<<(long val);

  self &operator<<(unsigned long val);

  self &operator<<(long long val);

  self &operator<<(unsigned long long val);

  self &operator<<(char val);

  self &operator<<(const void *);

  self &operator<<(float v);

  self &operator<<(double);

  self &operator<<(const char *val);

  self &operator<<(const unsigned char *str);

  self &operator<<(const std::string &v);

  self &operator<<(const Buffer &v);

  void Append(const char *data, int len);

  const Buffer &get_buffer() const;

  void Reset();

private:

  void StaticCheck();

  template<typename T>
  void Format(T t);

  Buffer buffer_;
  LogLevel level_;
  std::weak_ptr<Logger> logger_;
};

LogEvent &operator<<(LogEvent &log_event, const Formatter &formatter);

}

#endif 
