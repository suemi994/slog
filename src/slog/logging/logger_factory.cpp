/*
* Created by suemi on 2016/12/12.
*/

#include "slog/logging/logger_factory.h"
#include "slog/logging/log_scheduler.h"
#include "slog/appender/appender.h"
#include "slog/logging/logger.h"

namespace slog {

void LoggerFactory::Init(const Configuration &cfg) {
  coordinator.Initialize(cfg);
}

std::shared_ptr<Logger> LoggerFactory::GetLogger(const std::string &name) {
  return coordinator.SafeGetLogger(name);
}

LoggerFactory::Coordinator::Coordinator() : is_ready_(false) {

}

LoggerFactory::Coordinator::~Coordinator() {

}

std::shared_ptr<Logger> LoggerFactory::Coordinator::SafeGetLogger(const std::string &name) {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<Logger> ptr;
  {
    boost::shared_lock<boost::shared_mutex> read_lock(loggers_mutex_);
    auto it = loggers_.find(name);
    if (it != loggers_.end())
      ptr = it->second;
  }
  if (ptr != nullptr) return ptr;

  std::shared_ptr<Logger> root = SafeGetRootLogger();
  if (root == nullptr)
    return nullptr;
  ptr = std::make_shared<Logger>(name);
  ptr->set_log_level(root->log_level());
  ptr->set_layout(root->layout());
  ptr->set_filter(root->filter());

  bool res = SafeSetLogger(name, ptr);
  if (res) return ptr;
  else return SafeGetLogger(name);
}

std::shared_ptr<LogScheduler> LoggerFactory::Coordinator::SafeGetScheduler() {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<LogScheduler> ptr = scheduler_.load();
  if (ptr != nullptr) return ptr;
  ptr = LogScheduler::DefaultInstance();
  bool res = SafeSetScheduler(ptr);
  return scheduler_.load();
}

std::shared_ptr<Logger> LoggerFactory::Coordinator::SafeGetRootLogger() {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<Logger> ptr = root_logger_.load();
  if (ptr != nullptr) return ptr;
  ptr = std::make_shared<Logger>(std::string());
  bool res = SafeSetRootLogger(ptr);
  return root_logger_.load();
}

std::shared_ptr<Appender> LoggerFactory::Coordinator::SafeGetAppender(const std::string &name) {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<Appender> ptr;
  {
    boost::shared_lock<boost::shared_mutex> read_lock(appenders_mutex_);
    auto it = appenders_.find(name);
    if (it != appenders_.end())
      ptr = it->second;
  }
  if (ptr != nullptr) return ptr;

  ptr = Appender::DefaultInstance();

  bool res = SafeSetAppender(name, ptr);
  if (res) return ptr;
  else return SafeGetAppender(name);
}

void LoggerFactory::Coordinator::Reset() {
  boost::unique_lock<boost::shared_mutex> appenders_lock(appenders_mutex_);
  {
    boost::unique_lock<boost::shared_mutex> loggers_lock(loggers_mutex_);
    appenders_.clear();
    loggers_.clear();
    scheduler_.load()->Stop();
    scheduler_.store(nullptr);
    root_logger_.store(nullptr);
  }
}

bool LoggerFactory::Coordinator::ready() const {
  return is_ready_.load();
}

void LoggerFactory::Coordinator::Initialize(const Configuration &cfg) {
  // TODO
}

bool LoggerFactory::Coordinator::SafeSetScheduler(std::shared_ptr<LogScheduler> scheduler) {
  std::shared_ptr<LogScheduler> ptr = nullptr;
  bool res = std::atomic_compare_exchange_strong(&scheduler_, &ptr, scheduler);
  if (res)
    scheduler_.load()->Start();
  return res;
}

bool LoggerFactory::Coordinator::SafeSetRootLogger(std::shared_ptr<Logger> logger) {
  std::shared_ptr<Logger> ptr = nullptr;
  return std::atomic_compare_exchange_strong(&root_logger_, &ptr, logger);
}

bool LoggerFactory::Coordinator::SafeSetLogger(const std::string &name, const std::shared_ptr<Logger> logger) {
  boost::unique_lock<boost::shared_mutex> lock(loggers_mutex_);
  auto it = loggers_.find(name);
  if (it != loggers_.end())
    return false;
  loggers_.insert({name, logger});
  return true;
}

bool LoggerFactory::Coordinator::SafeSetAppender(const std::string &name, const std::shared_ptr<Appender> appender) {
  boost::unique_lock<boost::shared_mutex> lock(appenders_mutex_);
  auto it = appenders_.find(name);
  if (it != appenders_.end())
    return false;
  appenders_.insert({name, appender});
  return true;
}

}