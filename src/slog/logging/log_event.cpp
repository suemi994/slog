/*
* Created by suemi on 2016/12/12.
*/

#include <limits>

#include "slog/logging/log_event.h"
#include "slog/logging/logger.h"
#include "slog/logging/log_guard.h"

namespace slog {

namespace detail {

const char digits[] = "9876543210123456789";
const char *zero = digits + 9;

static_assert(sizeof(digits) == 20, "digits size err!");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex) == 17, "hex digits err!");

template<typename T>
size_t Convert(char buf[], T val) {
  T tmp = val;
  char *p = buf;

  do {
    int current = static_cast<int>(tmp % 10);
    tmp /= 10;
    *p++ = zero[current];
  } while (tmp != 0);

  if (val < 0)
    *p++ = '-';
  *p = '\0';

  std::reverse(buf, p);

  return p - buf;
}

size_t ConvertAsHex(char buf[], uintptr_t val) {
  uintptr_t tmp = val;
  char *p = buf;

  do {
    int current = static_cast<int>(tmp % 16);
    tmp /= 16;
    *p++ = digitsHex[current];
  } while (tmp != 0);

  *p = '\0';

  std::reverse(buf, p);

  return p - buf;
}
}
LogEvent::LogEvent(LogLevel level,const std::shared_ptr<Logger>& logger, LogEvent::Time time) :
    level_(level), logger_(logger), time_(time) {
}

LogEvent::LogEvent(LogLevel level, const std::shared_ptr<Logger>& logger) : LogEvent(level, logger,
                                                                              std::chrono::system_clock::now()) {

}

LogEvent::LogEvent(LogLevel level) : LogEvent(level, nullptr) {

}

LogEvent::~LogEvent() {
  try {
    if (auto logger_ptr = logger_.lock()) {
      logger_ptr->Submit(*this);
    }
  } catch (...) {
    LogGuard::Instance()->Error("Fail to submit a log!");
  }
}

LogEvent::self &LogEvent::operator<<(bool val) {
  buffer_.Append(val ? "1" : "0", 1);
  return *this;
}

LogEvent::self &LogEvent::operator<<(short val) {
  *this << static_cast<int>(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(unsigned short val) {
  *this << static_cast<int>(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(int val) {
  NumberAsString(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(unsigned int val) {
  NumberAsString(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(long val) {
  NumberAsString(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(unsigned long val) {
  NumberAsString(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(long long val) {
  NumberAsString(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(unsigned long long val) {
  NumberAsString(val);
  return *this;
}

LogEvent::self &LogEvent::operator<<(char val) {
  buffer_.Append(&val, 1);
  return *this;
}

LogEvent::self &LogEvent::operator<<(const void *val) {
  if (buffer_.avail() > max_numeric_size) {
    uintptr_t ptr = reinterpret_cast<uintptr_t>(val);
    char *buf = buffer_.current();
    *buf++ = '0';
    *buf++ = 'x';
    size_t written = detail::ConvertAsHex(buf, ptr);
    buffer_.Forward(written + 2);
  }
  return *this;
}

LogEvent::self &LogEvent::operator<<(float v) {
  *this << static_cast<double>(v);
  return *this;
}

LogEvent::self &LogEvent::operator<<(double val) {
  if (buffer_.avail() > max_numeric_size) {
    auto written = snprintf(buffer_.current(), max_numeric_size, "%.12g", val);
    buffer_.Forward(written);
  }
  return *this;
}

LogEvent::self &LogEvent::operator<<(const char *val) {
  if (val)
    buffer_.Append(val, std::strlen(val));
  else
    buffer_.Append("(null)", 6);
  return *this;
}

LogEvent::self &LogEvent::operator<<(const unsigned char *str) {
  *this << reinterpret_cast<const char *>(str);
  return *this;
}

LogEvent::self &LogEvent::operator<<(const std::string &v) {
  buffer_.Append(v.c_str(), v.size());
  return *this;
}

LogEvent::self &
LogEvent::Locate(const std::string &file, const std::string &method, int line, std::thread::id tid) {
  location_.source_file = file;
  location_.method = method;
  location_.line = line;
  location_.thread_id = tid;
  return *this;
}

LogEvent::self &LogEvent::operator<<(const LogEvent::Buffer &v) {
  buffer_.Append(v.data(), v.length());
  return *this;
}

void LogEvent::Append(const char *data, int len) {
  buffer_.Append(data, len);
}

LogEvent::Buffer &LogEvent::buffer() {
  return buffer_;
}

const LogEvent::Buffer &LogEvent::buffer() const {
  return buffer_;
}

void LogEvent::Reset() {
  buffer_.Reset();
}

LogLevel LogEvent::log_level() const {
  return level_;
}

Location &LogEvent::location() {
  return location_;
}

const Location &LogEvent::location() const {
  return location_;
}

std::string LogEvent::message() const {
  return buffer_.ToString();
}

std::shared_ptr<Logger> LogEvent::logger() const {
  return logger_.lock();
}

const LogEvent::Time &LogEvent::time() const {
  return time_;
}

LogEvent &LogEvent::operator<<(const LogEvent &log) {
  return operator<<(log.buffer());
}

void LogEvent::StaticCheck() {
  static_assert(max_numeric_size - 10 > std::numeric_limits<double>::digits10, "numeric limits exceed");
  static_assert(max_numeric_size - 10 > std::numeric_limits<long double>::digits10, "numeric limits exceed");
  static_assert(max_numeric_size - 10 > std::numeric_limits<long>::digits10, "numeric limits exceed");
  static_assert(max_numeric_size - 10 > std::numeric_limits<long long>::digits10, "numeric limits exceed");
}

template<typename T>
void LogEvent::NumberAsString(T t) {
  if (buffer_.avail() > max_numeric_size) {
    auto written = detail::Convert(buffer_.current(), t);
    buffer_.Forward(written);
  }
}

LogEvent &operator<<(LogEvent &log, const Formatter &formatter) {
  log.Append(formatter.data(), formatter.length());
  return log;
}

}