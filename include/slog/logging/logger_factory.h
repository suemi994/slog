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

class Configurator;


/**
 * LoggerFactory会在系统启动前从配置文件实例化RootLogger和一些特殊配置的logger
 * 对于未具现化的logger，沿用RootLogger的配置（layout和filter），并使用懒加载
 * LoggerFactory需要保证在生成新的Logger时的线程安全
 * @author suemi
 */
class LoggerFactory {
public:
  static void Init(Configurator &cfg);

  static std::shared_ptr<Logger> GetLogger(const std::string &name = "root");

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

    void Initialize(Configurator &cfg);

    bool SafeSetScheduler(std::shared_ptr<LogScheduler> scheduler);

    bool SafeSetRootLogger(std::shared_ptr<Logger> logger);

    bool SafeSetLogger(const std::string &name, const std::shared_ptr<Logger> logger);

    bool SafeSetAppender(const std::string &name, const std::shared_ptr<Appender> appender);

    std::unique_ptr<Configurator> configuration_;

    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers_;
    std::unordered_map<std::string, std::shared_ptr<Appender>> appenders_;
    std::shared_ptr<LogScheduler> scheduler_;

    std::atomic_bool is_ready_;
    std::mutex loggers_mutex_;
    std::mutex init_mutex_;

    friend class LoggerFactory;
  };

  LoggerFactory() = delete;

  static Coordinator coordinator;
};

}

#endif 
