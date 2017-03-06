/*
* Created by suemi on 2016/12/7.
*/

#ifndef SLOG_CONFIGURATION_H
#define SLOG_CONFIGURATION_H

#include <string>
#include <unordered_map>
#include <memory>

#include "slog/utils/properties.h"

namespace slog {

class Filter;

class Logger;

class Appender;

class Layout;

class LogScheduler;

class Configurator {
public:
  using LoggerPtr = std::shared_ptr<Logger>;
  using FilterPtr = std::shared_ptr<Filter>;
  using AppenderPtr = std::shared_ptr<Appender>;
  using LayoutPtr = std::shared_ptr<Layout>;
  using SchedulerPtr = std::shared_ptr<LogScheduler>;

  using LoggerMap = std::unordered_map<std::string,LoggerPtr>;
  using AppenderMap = std::unordered_map<std::string,AppenderPtr>;

  std::string config_file_;
public:
  Configurator(const std::string &file_path);

  virtual void Configure() = 0;

  virtual LoggerMap &loggers() = 0;

  virtual AppenderMap &appenders() = 0;

  virtual SchedulerPtr scheduler() = 0;

  virtual LoggerPtr root_logger() = 0;

  virtual void Reset() = 0;


};

class PropertyConfigurator : public Configurator {
public:
  PropertyConfigurator(const std::string& file_path);

  PropertyConfigurator(const Properties& properties);

  PropertyConfigurator(std::istream& prop_stream);

  virtual void Configure() override;

  virtual void Reset() override;

  virtual LoggerMap &loggers() override;

  virtual AppenderMap &appenders() override;

  virtual SchedulerPtr scheduler() override;

  virtual LoggerPtr root_logger() override;

private:

  void Init();

  void ConfigureLoggers();

  void ConfigureAppenders();

  void ConfigureScheduler();

  std::vector<std::string> LoggerNames(const Properties& prop) const;

  std::unordered_map<std::string,std::string> AppenderNames(const Properties& prop) const;

  void ConfigureLogger(LoggerPtr logger, const Properties& prop);

  LoggerMap loggers_;
  AppenderMap appenders_;
  SchedulerPtr scheduler_;
  LoggerPtr root_logger_;

  Properties prop_;

};

}

#endif 
