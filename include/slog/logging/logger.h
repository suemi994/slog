/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LOGGER_H
#define SLOG_LOGGER_H

#include <string>
#include <memory>

#include "slog/logging/log_level.h"
#include "slog/utils/no_copyable.h"

namespace slog {

class LogEvent;
class LogScheduler;
class Layout;
class Filter;

class Logger : public NoCopyable {
public:


  LogLevel log_level() const ;
  void set_log_level(LogLevel level);

  const std::string& get_name() const;
  bool IsEnableFor(LogLevel level) const ;

  void Submit(LogEvent& log);

private:
  std::shared_ptr<LogScheduler> scheduler_;
  std::string name_;
  volatile LogLevel level_;
  std::shared_ptr<Layout> layout_;
  std::shared_ptr<Filter> filter_;
};

}

#endif 
