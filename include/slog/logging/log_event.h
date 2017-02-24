/*
* Created by suemi on 2016/12/8.
*/

#ifndef SLOG_LOG_EVENT_H
#define SLOG_LOG_EVENT_H

#include <memory>
#include <chrono>
#include <thread>

#include "slog/logging/log_level.h"
#include "slog/utils/no_copyable.h"
#include "slog/base/fixed_buffer.h"

namespace slog {

class Logger;

class Formatter;


/*
 * 描述日志发生的位置信息
 */
struct Location {
  std::string line;
  std::string method;
  std::string source_file;
  std::thread::id thread_id_;
};

/**
 * 包含日志内容以及携带每条日志相关信息
 * @author suemi
 */
class LogEvent : public NoCopyable {
  using self = LogEvent;
  using Time = std::chrono::system_clock::time_point;
public:
  using Buffer = FixedBuffer<detail::SMALL_BUFFER_SIZE>;

  LogEvent(LogLevel level, std::shared_ptr<Logger> logger,Time time);

  LogEvent(LogLevel level,std::shared_ptr<Logger> logger);

  LogEvent(LogLevel level);

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

  self &operator<<(const void * val);

  self &operator<<(float val);

  self &operator<<(double val);

  self &operator<<(const char *val);

  self &operator<<(const unsigned char *str);

  self &operator<<(const std::string &v);

  self &operator<<(const Buffer &v);

  self& Locate(const std::string& file, const std::string& method, const std::string& line,std::thread::id tid);

  void Append(const char *data, int len);

  Buffer &buffer() const;

  void Reset();

  LogLevel log_level() const;

  Location& location() const;

  const Time& time() const;

  LogEvent &operator<<(const LogEvent &log);

private:
  void StaticCheck();

  /**
   * 将基本数值类型转换为char添加到buffer中
   * @tparam T
   * @param t
   */
  template<typename T>
  void NumberAsString(T t);

  static const int max_numeric_size = 32;

  Buffer buffer_;
  LogLevel level_;
  Time time_;
  Location location_;
  std::weak_ptr<Logger> logger_;
};

LogEvent &operator<<(LogEvent &log, const Formatter &formatter);

}

#endif 
