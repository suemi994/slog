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

  self& operator<<(char val);

private:
  Buffer buffer_;
  LogLevel level_;
  std::weak_ptr<Logger> logger_;
};

}

#endif 
