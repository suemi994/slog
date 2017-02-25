/*
* Created by suemi on 2016/12/13.
*/

#include "slog/logging/log_scheduler.h"

namespace slog {

LogScheduler::LogScheduler(int flush_interval) {

}

LogScheduler::~LogScheduler() {

}

std::shared_ptr<LogScheduler> LogScheduler::DefaultInstance() {
  return std::shared_ptr<LogScheduler>();
}

void LogScheduler::Append(const char *buf, int len) {

}

void LogScheduler::Flush() {

}

void LogScheduler::Start() {

}

void LogScheduler::Stop() {

}

void LogScheduler::AddAppender(std::shared_ptr<Appender> appender_ptr) {

}

void LogScheduler::RemoveAllAppenders() {

}

void LogScheduler::RemoveAppender(std::shared_ptr<Appender> appender_ptr) {

}

void LogScheduler::RemoveAppender(const std::string &appender_name) {

}

bool LogScheduler::is_running() const {
  return is_running_;
}

void LogScheduler::DaemonFunc() {

}

}

