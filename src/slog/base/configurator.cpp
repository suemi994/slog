/*
* Created by suemi on 2017/2/27.
*/

#include "slog/base/configurator.h"
#include "slog/logging/log_scheduler.h"
#include "slog/logging/logger.h"
#include "slog/format/layout_factory.h"
#include "slog/filter/filter_factory.h"
#include "slog/appender/appender_factory.h"

namespace slog {

Configurator::Configurator(const std::string &file_path) : config_file_(file_path){}

PropertyConfigurator::PropertyConfigurator(const std::string &file_path) : Configurator(file_path) {
  prop_ = Properties(file_path);
  Init();
}

PropertyConfigurator::PropertyConfigurator(const Properties &properties) : Configurator(std::string()),prop_(properties) {
  Init();
}

PropertyConfigurator::PropertyConfigurator(std::istream &prop_stream) : Configurator(std::string()),prop_(prop_stream) {
  Init();
}

void PropertyConfigurator::Configure() {
  ConfigureAppenders();
  ConfigureScheduler();
  ConfigureLoggers();
}

void PropertyConfigurator::Reset() {
  root_logger_ = nullptr;
  scheduler_ = nullptr;
  appenders_.clear();
  loggers_.clear();
}

Configurator::LoggerMap &PropertyConfigurator::loggers() {
  return loggers_;
}

Configurator::AppenderMap &PropertyConfigurator::appenders() {
  return appenders_;
}

Configurator::SchedulerPtr PropertyConfigurator::scheduler() {
  return scheduler_;
}

Configurator::LoggerPtr PropertyConfigurator::root_logger() {
  return root_logger_;
}

void PropertyConfigurator::Init() {
  prop_ = prop_.GetPropertySubset("slog.");
}

void PropertyConfigurator::ConfigureLoggers() {
  auto prop = prop_.GetPropertySubset("logger.");
  auto logger_names = LoggerNames(prop);
  root_logger_ = LoggerPtr(new Logger("root",scheduler_));
  if(prop.ExistPrefix("root."))
    ConfigureLogger(root_logger_,prop.GetPropertySubset("root."));


  for(auto & logger_name:logger_names){
    LoggerPtr logger(new Logger(logger_name,scheduler_));
    ConfigureLogger(logger,prop.GetPropertySubset(logger_name+"."));
    loggers_.insert({logger_name,logger});
  }
}

void PropertyConfigurator::ConfigureAppenders() {
  auto prop = prop_.GetPropertySubset("appender.");
  auto appender_names = AppenderNames(prop);

  for(auto it=appender_names.begin();it!=appender_names.end();++it){
    auto appender_prop = prop.GetPropertySubset(it->first+".");
    appender_prop.SetProperty("name",it->first);
    AppenderPtr appender = AppenderFactory::CreateInstance(it->second,appender_prop);
    appenders_.insert({it->first,appender});
  }
}

void PropertyConfigurator::ConfigureScheduler() {
  scheduler_ = std::make_shared<LogScheduler>(prop_.GetPropertySubset("scheduler."));
  for(auto it=appenders_.begin();it!=appenders_.end();++it)
    scheduler_->AddAppender(it->second);
}

void PropertyConfigurator::ConfigureLogger(LoggerPtr logger, const Properties &prop) {
  auto layout_clazz = prop.GetProperty("layout");
  LayoutPtr layout = LayoutFactory::CreateInstance(layout_clazz,prop.GetPropertySubset("layout."));

  auto filter_clazz = prop.GetProperty("filter");
  FilterPtr filter = FilterFactory::CreateInstance(filter_clazz,prop.GetPropertySubset("filter."));

  logger->set_layout(layout);
  logger->set_filter(filter);

}

std::vector<std::string> PropertyConfigurator::LoggerNames(const Properties& prop) const {
  std::vector<std::string> res;

  auto keys = prop.PropertyNames();
  std::transform(keys.begin(),keys.end(),res.begin(),[](auto & key){
    auto pos = key.find_first_of(".");
    return key.substr(0, pos == std::string::npos ? key.length() : pos);
  });

  res.erase(std::remove_if(res.begin(),res.end(),[](std::string& str){
    return str=="root";
  }),res.end());

  std::sort(res.begin(),res.end());
  res.erase(std::unique(res.begin(),res.end()),res.end());
  return res;
}

std::unordered_map<std::string, std::string> PropertyConfigurator::AppenderNames(const Properties &prop) const {
  return prop.PropertyWithoutSuffix();
}


}