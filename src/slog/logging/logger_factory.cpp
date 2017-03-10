/*
* Created by suemi on 2016/12/12.
*/

#include "slog/logging/logger_factory.h"
#include "slog/logging/log_scheduler.h"
#include "slog/appender/appender.h"
#include "slog/logging/logger.h"
#include "slog/base/configurator.h"

namespace slog {

void LoggerFactory::Init(Configurator &cfg) {
  coordinator.Initialize(cfg);
}

std::shared_ptr<Logger> LoggerFactory::GetLogger(const std::string &name) {
  return coordinator.SafeGetLogger(name);
}

LoggerFactory::Coordinator LoggerFactory::coordinator;

LoggerFactory::Coordinator::Coordinator() : is_ready_(false) {}

LoggerFactory::Coordinator::~Coordinator() {}

std::shared_ptr<Logger> LoggerFactory::Coordinator::SafeGetLogger(const std::string &name) {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<Logger> ptr;
  {
    std::lock_guard<std::mutex> logger_lock(loggers_mutex_);
    auto it = loggers_.find(name);
    if (it != loggers_.end())
      ptr = it->second;
  }
  if (ptr != nullptr) return ptr;

  std::shared_ptr<Logger> root = SafeGetRootLogger();
  if (root == nullptr)
    return nullptr;
  ptr = std::make_shared<Logger>(name);
  ptr->set_layout(root->layout());
  ptr->set_filter(root->filter());
  ptr->set_scheduler(root->scheduler());
  bool res = SafeSetLogger(name, ptr);
  if (res) return ptr;
  else return SafeGetLogger(name);
}

std::shared_ptr<LogScheduler> LoggerFactory::Coordinator::SafeGetScheduler() {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<LogScheduler> ptr;
  {
    std::lock_guard<std::mutex> read_lock(init_mutex_);
    ptr = scheduler_;
  }
  return ptr;
}

std::shared_ptr<Logger> LoggerFactory::Coordinator::SafeGetRootLogger() {
  return SafeGetLogger("root");
}

std::shared_ptr<Appender> LoggerFactory::Coordinator::SafeGetAppender(const std::string &name) {
  if (!is_ready_.load())
    return nullptr;
  std::shared_ptr<Appender> ptr;
  {
    std::lock_guard<std::mutex> init_lock(init_mutex_);
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
  std::lock_guard<std::mutex> init_lock(init_mutex_);
  {
    std::lock_guard<std::mutex> loggers_lock(loggers_mutex_);
    appenders_.clear();
    loggers_.clear();
    scheduler_->Stop();
    scheduler_.reset();
  }
}

bool LoggerFactory::Coordinator::ready() const {
  return is_ready_.load();
}

void LoggerFactory::Coordinator::Initialize(Configurator &cfg) {
  if(is_ready_.load()) Reset();
  is_ready_.store(false);

  {
    std::lock_guard<std::mutex> init_lock(init_mutex_);
    std::lock_guard<std::mutex> loggers_lock(loggers_mutex_);
    scheduler_ = cfg.scheduler();
    auto & appenders = cfg.appenders();
    for(auto it=appenders.begin();it!=appenders.end();++it)
      appenders_.insert(*it);
    loggers_.insert({"root",cfg.root_logger()});
    auto & loggers = cfg.loggers();
    for(auto it=loggers.begin();it!=loggers.end();++it)
      loggers_.insert(*it);

    scheduler_->Start();
  }
  is_ready_.store(true);

  cfg.Reset();
}

bool LoggerFactory::Coordinator::SafeSetScheduler(std::shared_ptr<LogScheduler> scheduler) {
  std::lock_guard<std::mutex> lock(init_mutex_);
  std::shared_ptr<LogScheduler> ptr = scheduler;
  if(ptr!= nullptr) return false;
  scheduler_ = scheduler;
  return true;
}

bool LoggerFactory::Coordinator::SafeSetRootLogger(std::shared_ptr<Logger> logger) {
  return SafeSetLogger("root",logger);
}

bool LoggerFactory::Coordinator::SafeSetLogger(const std::string &name, const std::shared_ptr<Logger> logger) {
  std::lock_guard<std::mutex> lock(loggers_mutex_);
  auto it = loggers_.find(name);
  if (it != loggers_.end())
    return false;
  loggers_.insert({name, logger});
  return true;
}

bool LoggerFactory::Coordinator::SafeSetAppender(const std::string &name, const std::shared_ptr<Appender> appender) {
  std::lock_guard<std::mutex> lock(init_mutex_);
  auto it = appenders_.find(name);
  if (it != appenders_.end())
    return false;
  appenders_.insert({name, appender});
  return true;
}

}