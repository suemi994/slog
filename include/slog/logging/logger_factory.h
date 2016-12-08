/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LOGGER_FACTORY_H
#define SLOG_LOGGER_FACTORY_H

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

namespace slog {

class Logger;
class LogScheduler;
class Configuration;

/**
 * LoggerFactory会在系统启动前从配置文件实例化RootLogger和一些特殊配置的logger
 * 对于未具现化的logger，沿用RootLogger的配置（layout和filter），并使用懒加载
 * LoggerFactory需要保证在生成新的Logger时的线程安全
 * @author suemi
 */
class LoggerFactory {
public:
  static LoggerFactory& Instance();
  static std::shared_ptr<Logger> GetLogger(const std::string& name = "");
  static std::shared_ptr<LogScheduler> Scheduler();
  static std::shared_ptr<Logger> Root();
private:
  LoggerFactory();
  LoggerFactory(const Configuration& cfg);

  /**
   * 使用Double-Check，在指名logger未实例化时构造插入新的Logger，保证线程安全
   * @param name
   * @return
   */
  std::shared_ptr<Logger> SafeGetLogger(const std::string& name);

  static LoggerFactory factory_;
  std::unordered_map<std::string,std::shared_ptr<Logger>> loggers_;
  std::shared_ptr<Logger> root_logger_;
  std::shared_ptr<LogScheduler> scheduler_;
  std::mutex mutex_;
};

}

#endif 
