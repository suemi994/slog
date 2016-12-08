/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_LOGGER_FACTORY_H
#define SLOG_LOGGER_FACTORY_H

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include <atomic>

#include "slog/utils/no_copyable.h"

namespace slog {

class Logger;

class LogScheduler;

class Appender;

class Configuration;


/**
 * LoggerFactory会在系统启动前从配置文件实例化RootLogger和一些特殊配置的logger
 * 对于未具现化的logger，沿用RootLogger的配置（layout和filter），并使用懒加载
 * LoggerFactory需要保证在生成新的Logger时的线程安全
 * @author suemi
 */
class LoggerFactory {
public:
  static void Init(const Configuration &cfg);

  static std::shared_ptr<Logger> GetLogger(const std::string &name = "");

private:

  /**
   * IOC容器类
   */
  class Coordinator : public NoCopyable {
  public:

    Coordinator();

    ~Coordinator();

    /**
     * 使用Double-Check，在指名logger未实例化时构造插入新的Logger，保证线程安全
     * @param name
     * @return
     */
    std::shared_ptr<Logger> SafeGetLogger(const std::string &name);

    std::shared_ptr<LogScheduler> SafeGetScheduler();

    std::shared_ptr<Logger> SafeGetRootLogger();

    std::shared_ptr<Appender> SafeGetAppender(const std::string &name);

    void Reset();

    bool ready() const;

  private:

    void Initialize(const Configuration &cfg);

    std::unique_ptr<Configuration> configuration_;

    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;
    std::unordered_map<std::string, std::shared_ptr<Appender>> appenders_;
    std::shared_ptr<Logger> root_logger_;
    std::shared_ptr<LogScheduler> scheduler_;
    std::mutex mutex_;

    std::atomic_bool is_ready_;

    friend class LoggerFactory;
  };

  LoggerFactory() = delete;

  static Coordinator coordinator;
};

}

#endif 
