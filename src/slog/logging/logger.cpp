/*
* Created by suemi on 2016/12/12.
*/

#include "slog/logging/logger.h"
#include "slog/logging/log_event.h"
#include "slog/filter/filter.h"
#include "slog/format/layout.h"
#include "slog/logging/log_scheduler.h"

namespace slog {

Logger::Logger(const std::string &name, std::shared_ptr<LogScheduler> scheduler) :
    name_(name), scheduler_(scheduler) {}

Logger::Logger(const std::string &name) : name_(name) {

}

const std::string &Logger::name() const {
  return name_;
}

void Logger::Submit(LogEvent &log) {
  if (scheduler_ == nullptr) return;
  if (filter_ != nullptr && filter_->Decide(log) == Filter::Result::DENY)
    return;
  if (layout_ != nullptr) layout_->Reformat(log);
  auto &buf = log.buffer();
  scheduler_->Append(buf.data(), buf.length());
}


std::shared_ptr<Layout> Logger::layout() const {
  return layout_;
}

void Logger::set_layout(const std::shared_ptr<Layout> layout) {
  layout_ = layout;
}

std::shared_ptr<Filter> Logger::filter() const {
  return filter_;
}

void Logger::set_filter(const std::shared_ptr<Filter> filter) {
  filter_ = filter;
}

}
