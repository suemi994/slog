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

/**
 * 负责过滤筛选日志事件并格式化日志
 * @author suemi
 */
class Logger : public NoCopyable {
public:
  Logger(const std::string& name, std::shared_ptr<LogScheduler> scheduler);

  Logger(const std::string& name);

  const std::string& name() const;

  void Submit(LogEvent& log);


  std::shared_ptr<Layout> layout() const;
  void set_layout(const std::shared_ptr<Layout> layout);
  std::shared_ptr<Filter> filter() const;
  void set_filter(const std::shared_ptr<Filter> filter);
  const std::shared_ptr<LogScheduler>& scheduler() const;
  void set_scheduler(const std::shared_ptr<LogScheduler>& scheduler);

private:
  std::shared_ptr<LogScheduler> scheduler_;
  std::string name_;
  std::shared_ptr<Layout> layout_;
  std::shared_ptr<Filter> filter_;
};

}

#endif 
